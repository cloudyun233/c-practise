"""
05_bayesian_optimization.py
使用贝叶斯优化对 LSTM 进行超参数调优
作者：方义凯
依赖：scikit-optimize (pip install scikit-optimize)
"""

import numpy as np
from skopt import gp_minimize
from skopt.space import Integer, Real, Categorical
from skopt.utils import use_named_args
from sklearn.metrics import mean_squared_error
from sklearn.model_selection import KFold
import time

# 需要从 models_02 导入构建函数和训练函数
from models_02 import build_lstm

# 模块级默认值，objective() 闭包引用此变量；__main__ 块可覆盖
LOOK_BACK = 30
X_train = None
y_train = None

# 定义 K 折交叉验证评分函数（复制自 grid_search，避免循环导入）
def _kfold_score_dl(model_fn, build_kwargs, train_kwargs, X_full, y_full, n_splits=5, look_back=30):
    """对深度学习模型做 K 折交叉验证，返回平均 RMSE"""
    kf = KFold(n_splits=n_splits, shuffle=False)
    rmse_list = []
    for tr_idx, val_idx in kf.split(X_full):
        Xtr, ytr = X_full[tr_idx], y_full[tr_idx]
        Xv,  yv  = X_full[val_idx], y_full[val_idx]
        Xtr3 = Xtr.reshape(-1, look_back, 1)
        Xv3  = Xv.reshape(-1, look_back, 1)
        model = model_fn(**build_kwargs)
        model.fit(Xtr3, ytr, validation_data=(Xv3, yv),
                  epochs=train_kwargs.get("epochs", 30),
                  batch_size=train_kwargs.get("batch_size", 32),
                  verbose=0)
        pred = model.predict(Xv3, verbose=0).flatten()
        rmse = np.sqrt(mean_squared_error(yv.flatten(), pred))
        rmse_list.append(rmse)
    return np.mean(rmse_list)

# ==================== 参数空间定义 ====================
space = [
    Integer(32, 128, name='units'),
    Real(1e-4, 1e-1, prior='log-uniform', name='lr'),
    Real(0.1, 0.3, name='dropout'),
    Categorical([16, 32, 64], name='batch_size')
]

@use_named_args(space)
def objective(**params):
    """目标函数：返回 CV-RMSE"""
    build_kwargs = {
        'look_back': LOOK_BACK,
        'units': int(params['units']),  # 确保整数
        'lr': float(params['lr']),
        'dropout': float(params['dropout'])
    }
    train_kwargs = {
        'batch_size': params['batch_size'],
        'epochs': 30  # 固定 epochs 为 30，也可作为超参数，但为控制时间暂不优化
    }
    rmse = _kfold_score_dl(build_lstm, build_kwargs, train_kwargs,
                           X_train, y_train, n_splits=5, look_back=LOOK_BACK)
    return rmse

if __name__ == "__main__":
    import gc
    import tensorflow as tf

    # 加载训练数据（使用 5 分钟粒度）
    LOOK_BACK = 30
    try:
        X_train = np.load("X_train.npy")
        y_train = np.load("y_train.npy")
    except FileNotFoundError:
        print("请先运行 01_data_preprocessing_late.py 生成训练数据")
        exit(1)

    print(f"[INFO] 训练集形状: X={X_train.shape}, y={y_train.shape}")
    print("[INFO] 开始贝叶斯优化，共 30 次迭代...")
    start_time = time.time()
    res = gp_minimize(objective, space, n_calls=30, random_state=42, verbose=True)
    elapsed = time.time() - start_time

    best_params = {
        "units":      res.x[0],
        "lr":         res.x[1],
        "dropout":    res.x[2],
        "batch_size": res.x[3],
    }

    print("\n========== 贝叶斯优化结果 ==========")
    print(f"最优参数: units={best_params['units']}, lr={best_params['lr']:.6f}, "
          f"dropout={best_params['dropout']:.3f}, batch_size={best_params['batch_size']}")
    print(f"最优 CV-RMSE: {res.fun:.6f}")
    print(f"总耗时: {elapsed/60:.1f} 分钟")

    # 保存结果，供 04_evaluation.py 读取
    np.save("bayesian_results.npy", {"best_params": best_params, "best_rmse": res.fun})
    print("[INFO] 贝叶斯优化结果已保存至 bayesian_results.npy")