# -*- coding: utf-8 -*-
"""
01_data_preprocessing.py
基于网格搜索的短时交通量预测研究 —— 数据预处理模块
作者：方义凯   学号：632210040410
指导教师：赵磊娜
"""

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib
from sklearn.preprocessing import MinMaxScaler
import warnings
warnings.filterwarnings("ignore")

matplotlib.rcParams["font.family"] = "SimHei"
matplotlib.rcParams["axes.unicode_minus"] = False

# ==================== 用户配置区域 ====================
DATA_PATH = r'D:\IDE\pycharm projects\Python\thesis_code\datasets\PEMS04.npz'  # .npz 文件路径
SENSOR_INDEX = 0                     # 选择第几个传感器（从0开始）
FEATURE_INDEX = 0                     # 选择哪个特征：0=flow, 1=occupy, 2=speed（当前只用flow）
LOOK_BACK = 30                         # 输入时间步数（2.5小时）
HORIZON = 1                            # 预测步数（5分钟）
# =====================================================


# ──────────────────────────────────────────────
# 1. 数据加载（适配 PEMS04.npz）
# ──────────────────────────────────────────────
def load_pems_data(filepath: str, sensor_idx: int = 0, feature_idx: int = 0) -> pd.DataFrame:
    """
    从 .npz 文件中加载 PEMS 数据。
    假设数组形状为 (时间步数, 传感器数, 特征数)，特征顺序为 flow, occupy, speed。
    返回 DataFrame，包含 Flow, Speed, Occupancy 三列，索引为模拟时间戳。
    """
    #if not os.path.exists(filepath):
        #raise FileNotFoundError(f"数据文件不存在：{filepath}")

    data = np.load(filepath)
    print(f"[INFO] 已加载 NPZ 文件，keys: {list(data.keys())}")

    # 假设数据存储在 'data' 键中
    if 'data' in data:
        arr = data['data']
    else:
        # 如果没有 data 键，取第一个数组
        arr = data[data.files[0]]

    print(f"[INFO] 原始数组形状: {arr.shape}")

    # 提取指定传感器的三个特征
    if arr.ndim == 3:
        sensor_data = arr[:, sensor_idx, :]   # 形状 (时间步, 3)
        flow    = sensor_data[:, 0]
        occupy  = sensor_data[:, 1]
        speed   = sensor_data[:, 2]
    else:
        raise ValueError("数组维度不是3，请检查数据格式")

    # 生成模拟时间索引（5分钟粒度，从2024-01-01开始）
    timestamps = pd.date_range(start="2024-01-01", periods=len(flow), freq="5min")

    df = pd.DataFrame({
        "Flow": flow,
        "Speed": speed,
        "Occupancy": occupy
    }, index=timestamps)

    print(f"[INFO] 已提取传感器 {sensor_idx} 的数据，共 {len(df)} 条记录。")
    return df


# ──────────────────────────────────────────────
# 2. 缺失值处理（保持不变）
# ──────────────────────────────────────────────
def handle_missing_values(df: pd.DataFrame) -> pd.DataFrame:
    """线性插值填补缺失值；连续缺失段用滑动平均修复。"""
    before = df.isnull().sum().sum()
    df = df.interpolate(method="linear", limit_direction="both")
    for col in ["Flow", "Speed", "Occupancy"]:
        if df[col].isnull().any():
            df[col] = df[col].fillna(df[col].rolling(12, min_periods=1).mean())
    after = df.isnull().sum().sum()
    print(f"[INFO] 缺失值处理：修复前 {before} 个，修复后 {after} 个。")
    return df


# ──────────────────────────────────────────────
# 3. 异常值检测与替换（3σ 原则）（保持不变）
# ──────────────────────────────────────────────
def remove_outliers(df: pd.DataFrame, cols=None) -> pd.DataFrame:
    """基于 3σ 原则识别异常值，用相邻时段中位数替换。"""
    if cols is None:
        cols = ["Flow", "Speed", "Occupancy"]
    total_outliers = 0
    for col in cols:
        mu, sigma = df[col].mean(), df[col].std()
        mask = (df[col] - mu).abs() > 3 * sigma
        count = mask.sum()
        total_outliers += count
        if count > 0:
            # 用前后 6 个时步的中位数替换
            idx = df.index[mask]
            for i in idx:
                pos = df.index.get_loc(i)
                window = df[col].iloc[max(0, pos-6): min(len(df), pos+7)]
                df.at[i, col] = window.median()
    print(f"[INFO] 异常值处理：共替换 {total_outliers} 个异常点。")
    return df


# ──────────────────────────────────────────────
# 4. 归一化（保持不变）
# ──────────────────────────────────────────────
def normalize_data(df: pd.DataFrame, feature_cols=None):
    """将数值特征归一化至 [0,1] 区间。"""
    if feature_cols is None:
        feature_cols = ["Flow", "Speed", "Occupancy"]
    scaler = MinMaxScaler()
    df[feature_cols] = scaler.fit_transform(df[feature_cols])
    print(f"[INFO] 数据已归一化，特征列：{feature_cols}")
    return df, scaler


# ──────────────────────────────────────────────
# 5. 构建滚动窗口样本（单变量版本，仅使用 Flow）
# ──────────────────────────────────────────────
def build_rolling_window(series: np.ndarray, look_back: int = 30,
                          horizon: int = 1):
    """
    滚动窗口法构建监督学习样本。
    X: (N, look_back)，y: (N, horizon)
    """
    X, y = [], []
    for i in range(len(series) - look_back - horizon + 1):
        X.append(series[i: i + look_back])
        y.append(series[i + look_back: i + look_back + horizon])
    return np.array(X), np.array(y)


# ──────────────────────────────────────────────
# 6. 数据集划分 7:2:1（保持不变）
# ──────────────────────────────────────────────
def split_dataset(X: np.ndarray, y: np.ndarray,
                  train_ratio=0.7, val_ratio=0.2):
    """按时序顺序划分训练集、验证集、测试集 (7:2:1)。"""
    n = len(X)
    n_train = int(n * train_ratio)
    n_val   = int(n * val_ratio)

    X_train, y_train = X[:n_train], y[:n_train]
    X_val,   y_val   = X[n_train: n_train + n_val], y[n_train: n_train + n_val]
    X_test,  y_test  = X[n_train + n_val:], y[n_train + n_val:]

    print(f"[INFO] 数据集划分 — 训练: {len(X_train)}, 验证: {len(X_val)}, 测试: {len(X_test)}")
    return X_train, X_val, X_test, y_train, y_val, y_test


# ──────────────────────────────────────────────
# 7. 可视化（保持不变）
# ──────────────────────────────────────────────
def plot_flow(df: pd.DataFrame, n_days: int = 7, save_path: str = None):
    """绘制前 n_days 天的交通流量时序图。"""
    steps = n_days * 288  # 每天 288 个 5 分钟粒度
    subset = df.iloc[:steps]
    plt.figure(figsize=(14, 4))
    plt.plot(subset.index, subset["Flow"], linewidth=0.8, color="#2E86AB")
    plt.title(f"交通流量时序图（前 {n_days} 天）", fontsize=14)
    plt.xlabel("时间")
    plt.ylabel("归一化流量")
    plt.tight_layout()
    if save_path:
        plt.savefig(save_path, dpi=150)
        print(f"[INFO] 图表已保存：{save_path}")
    plt.show()


# ──────────────────────────────────────────────
# 主流程
# ──────────────────────────────────────────────
if __name__ == "__main__":
    import os

    # Step 1 ~ 4
    df = load_pems_data(DATA_PATH, sensor_idx=SENSOR_INDEX, feature_idx=FEATURE_INDEX)
    df = handle_missing_values(df)
    df = remove_outliers(df)
    df, scaler = normalize_data(df)

    # Step 5 ~ 6
    flow_series = df["Flow"].values
    X, y = build_rolling_window(flow_series, look_back=LOOK_BACK, horizon=HORIZON)
    X_train, X_val, X_test, y_train, y_val, y_test = split_dataset(X, y)

    # 保存处理后数据
    np.save("5min粒度/X_train.npy", X_train); np.save("5min粒度/y_train.npy", y_train)
    np.save("5min粒度/X_val.npy", X_val);   np.save("5min粒度/y_val.npy", y_val)
    np.save("5min粒度/X_test.npy", X_test);  np.save("5min粒度/y_test.npy", y_test)
    print("[INFO] 预处理数据已保存为 .npy 文件。")

    # 可视化
    plot_flow(df, n_days=7, save_path="5min粒度/flow_timeseries.png")