"""
03_grid_search.py
基于网格搜索的短时交通量预测研究 —— 网格搜索参数优化模块
作者：方义凯   学号：632210040410
修改：添加结果保存功能、修复TensorFlow资源泄漏问题
"""

import numpy as np
import itertools
import time
import json
import gc
from sklearn.model_selection import KFold

from models_02 import (build_lstm, build_bigru, train_arima,
                       predict_arima, compute_metrics,
                       adf_stationary_test, ljung_box_white_noise_test,
                       get_pq_range_by_acf_pacf, predict_arima_static)

def _kfold_score_dl(model_fn, build_kwargs, train_kwargs,
                    X_full, y_full, n_splits=5, look_back=30):
    from sklearn.metrics import mean_squared_error
    import tensorflow as tf
    
    kf = KFold(n_splits=n_splits, shuffle=False)
    rmse_list = []
    
    for fold, (tr_idx, val_idx) in enumerate(kf.split(X_full)):
        Xtr, ytr = X_full[tr_idx], y_full[tr_idx]
        Xv,  yv  = X_full[val_idx], y_full[val_idx]
        Xtr3 = Xtr.reshape(-1, look_back, 1)
        Xv3  = Xv.reshape(-1,  look_back, 1)
        
        # 清除之前的会话和模型
        tf.keras.backend.clear_session()
        gc.collect()
        
        model = model_fn(**build_kwargs)
        model.fit(Xtr3, ytr, validation_data=(Xv3, yv),
                  epochs=train_kwargs.get("epochs", 30),
                  batch_size=train_kwargs.get("batch_size", 32),
                  verbose=0)
        pred = model.predict(Xv3, verbose=0).flatten()
        rmse = np.sqrt(mean_squared_error(yv.flatten(), pred))
        rmse_list.append(rmse)
        
        # 删除模型释放内存
        del model
        
    return np.mean(rmse_list)


LSTM_PARAM_GRID = {
    "units":      [64, 128],
    "lr":         [0.001, 0.01],
    "dropout":    [0.1, 0.2],
    "batch_size": [64, 128],
}

def lstm_grid_search(X_train, y_train, look_back=30, n_splits=3,
                     param_grid=None, verbose=True):
    """
    LSTM网格搜索（优化版：更快的搜索策略）
    n_splits: 交叉验证折数，默认3折（平衡速度和准确性）
    """
    if param_grid is None:
        param_grid = LSTM_PARAM_GRID
    keys   = list(param_grid.keys())
    combos = list(itertools.product(*param_grid.values()))
    total  = len(combos)
    print(f"[LSTM 网格搜索] 参数组合总数：{total}")
    best_rmse, best_params, all_results = np.inf, None, []
    
    for i, combo in enumerate(combos):
        params = dict(zip(keys, combo))
        t0 = time.time()
        try:
            rmse = _kfold_score_dl(
                model_fn=build_lstm,
                build_kwargs={"look_back": look_back,
                              "units": params["units"],
                              "lr": params["lr"],
                              "dropout": params["dropout"]},
                train_kwargs={"batch_size": params["batch_size"],
                              "epochs": 15},  # 减少epochs加快搜索
                X_full=X_train, y_full=y_train,
                n_splits=n_splits, look_back=look_back
            )
        except Exception as e:
            rmse = np.inf
            if verbose:
                print(f"  [{i+1}/{total}] {params} → 出错: {e}")
            all_results.append({**params, "cv_rmse": float('inf')})
            continue
        elapsed = time.time() - t0
        all_results.append({**params, "cv_rmse": round(float(rmse), 6)})
        if verbose:
            print(f"  [{i+1}/{total}] {params} → CV-RMSE={rmse:.6f}  ({elapsed:.1f}s)")
        if rmse < best_rmse:
            best_rmse   = rmse
            best_params = params.copy()
    
    print(f"\n[LSTM 最优参数] {best_params}  CV-RMSE={best_rmse:.6f}")
    return best_params, all_results


BIGRU_PARAM_GRID = {
    "units":      [64, 128],
    "lr":         [0.001, 0.01],
    "epochs":     [50, 100],
    "batch_size": [64],
}

def bigru_grid_search(X_train, y_train, look_back=30, n_splits=3,
                      param_grid=None, verbose=True):
    """
    Bi-GRU网格搜索（优化版）
    n_splits: 交叉验证折数，默认3折
    """
    if param_grid is None:
        param_grid = BIGRU_PARAM_GRID
    keys   = list(param_grid.keys())
    combos = list(itertools.product(*param_grid.values()))
    total  = len(combos)
    print(f"[Bi-GRU 网格搜索] 参数组合总数：{total}")
    best_rmse, best_params, all_results = np.inf, None, []
    
    for i, combo in enumerate(combos):
        params = dict(zip(keys, combo))
        t0 = time.time()
        try:
            rmse = _kfold_score_dl(
                model_fn=build_bigru,
                build_kwargs={"look_back": look_back,
                              "units": params["units"],
                              "lr": params["lr"],
                              "dropout": 0.2},
                train_kwargs={"batch_size": params["batch_size"],
                              "epochs": params["epochs"]},
                X_full=X_train, y_full=y_train,
                n_splits=n_splits, look_back=look_back
            )
        except Exception as e:
            rmse = np.inf
            all_results.append({**params, "cv_rmse": float('inf')})
            continue
        elapsed = time.time() - t0
        all_results.append({**params, "cv_rmse": round(float(rmse), 6)})
        if verbose:
            print(f"  [{i+1}/{total}] {params} → CV-RMSE={rmse:.6f}  ({elapsed:.1f}s)")
        if rmse < best_rmse:
            best_rmse   = rmse
            best_params = params.copy()
    
    print(f"\n[Bi-GRU 最优参数] {best_params}  CV-RMSE={best_rmse:.6f}")
    return best_params, all_results


ARIMA_PARAM_GRID = {
    "p": [1, 2, 3],
    "d": [0, 1],
    "q": [1, 2, 3],
}


def arima_grid_search(train_series: np.ndarray,
                      val_series: np.ndarray,
                      auto_pq: bool = True,
                      param_grid=None,
                      verbose=True):
    """
    ARIMA网格搜索定阶（简化版）
    """
    from statsmodels.tsa.arima.model import ARIMA
    from sklearn.metrics import mean_squared_error
    import itertools

    # 步骤1：平稳性检验
    print("\n" + "=" * 50)
    print("步骤1：序列平稳性检验与差分阶数d确定")
    is_stationary, best_d = adf_stationary_test(train_series)
    d_candidate = [best_d]

    # 步骤2：白噪声检验
    print("\n步骤2：序列白噪声检验")
    is_white_noise = ljung_box_white_noise_test(train_series, is_residual=False)
    if is_white_noise:
        print("[WARN] ⚠️ 输入序列为白噪声，ARIMA建模无意义！")
        return (0, 0, 0), []

    # 步骤3：ACF/PACF定阶
    print("\n步骤3：ACF/PACF初步定阶")
    if best_d > 0:
        stationary_series = np.diff(train_series, n=best_d)
    else:
        stationary_series = train_series.copy()

    if auto_pq:
        p_candidate, q_candidate = get_pq_range_by_acf_pacf(stationary_series)
    else:
        p_candidate = [1, 2, 3]
        q_candidate = [1, 2, 3]

    # 生成参数网格
    if param_grid is None:
        param_grid = {
            "p": p_candidate,
            "d": d_candidate,
            "q": q_candidate
        }

    keys = list(param_grid.keys())
    combos = list(itertools.product(*param_grid.values()))
    total = len(combos)
    print(f"\n[ARIMA 网格搜索] 参数组合总数：{total}")
    best_aic, best_rmse, best_order, all_results = np.inf, np.inf, None, []

    # 步骤4：网格搜索
    for i, combo in enumerate(combos):
        p, d, q = combo
        try:
            # 拟合ARIMA模型（最简形式）
            model = ARIMA(train_series, order=(p, d, q))
            fitted_model = model.fit()
            
            # 单步滚动预测验证集
            pred = predict_arima_static(fitted_model, val_series, train_series, refit_step=20)
            
            # 计算指标
            rmse = np.sqrt(mean_squared_error(val_series, pred))
            aic = fitted_model.aic
            bic = fitted_model.bic
            
        except Exception as e:
            import traceback
            if verbose:
                print(f"  [{i + 1}/{total}] ARIMA({p},{d},{q}) → 失败: {e}")
                traceback.print_exc()
            all_results.append({"p": p, "d": d, "q": q, "rmse": float('inf'), 
                              "aic": float('inf'), "bic": float('inf')})
            continue

        all_results.append({
            "p": p, "d": d, "q": q,
            "rmse": round(float(rmse), 6),
            "aic": round(float(aic), 2),
            "bic": round(float(bic), 2)
        })
        if verbose:
            print(f"  [{i + 1}/{total}] ARIMA({p},{d},{q}) → RMSE={rmse:.6f}, AIC={aic:.2f}")

        # 选择最优（优先AIC，参考RMSE）
        if aic < best_aic and rmse < best_rmse * 1.1:
            best_aic = aic
            best_rmse = rmse
            best_order = (p, d, q)

    print(f"\n[ARIMA 最优参数] order={best_order}  AIC={best_aic:.2f}  RMSE={best_rmse:.6f}")
    return best_order, all_results


if __name__ == "__main__":
    import sys, os
    import tensorflow as tf
    
    sys.path.insert(0, os.path.dirname(__file__))
    
    # 限制GPU显存增长，避免内存溢出
    gpus = tf.config.experimental.list_physical_devices('GPU')
    if gpus:
        try:
            for gpu in gpus:
                tf.config.experimental.set_memory_growth(gpu, True)
            print("[INFO] GPU显存动态分配已启用")
        except RuntimeError as e:
            print(f"[WARN] GPU配置失败: {e}")

    # 加载预处理数据
    X_train = np.load("X_train.npy")
    y_train = np.load("y_train.npy")
    X_val   = np.load("X_val.npy")
    y_val   = np.load("y_val.npy")
    
    # 【关键修复】为ARIMA重建连续的Flow时间序列
    # X_train形状: (样本数, look_back)，每个样本是连续30个时间步
    flow_train_full = []
    flow_train_full.extend(X_train[0].flatten().tolist())
    for i in range(1, len(X_train)):
        flow_train_full.append(X_train[i][-1])
    
    flow_val_full = []
    flow_val_full.extend(X_val[0].flatten().tolist())
    for i in range(1, len(X_val)):
        flow_val_full.append(X_val[i][-1])
    
    train_series = np.array(flow_train_full)
    val_series = np.array(flow_val_full)
    print(f"[INFO] ARIMA训练序列长度: {len(train_series)}, 验证序列长度: {len(val_series)}")

    print("=" * 50)
    print("开始 LSTM 网格搜索...")
    print(f"[INFO] 参数组合数: {len(list(itertools.product(*LSTM_PARAM_GRID.values())))}")
    print(f"[INFO] 交叉验证折数: 3")
    print(f"[INFO] 训练轮数: 15 (搜索阶段)")
    best_lstm, lstm_results = lstm_grid_search(X_train, y_train, look_back=30, n_splits=3)
    
    # 确保所有值都是可序列化的
    lstm_results_serializable = []
    for r in lstm_results:
        serializable_r = {}
        for k, v in r.items():
            if isinstance(v, (np.floating, float)):
                serializable_r[k] = float(v)
            elif isinstance(v, (np.integer, int)):
                serializable_r[k] = int(v)
            else:
                serializable_r[k] = v
        lstm_results_serializable.append(serializable_r)
    
    with open("lstm_grid_results.json", "w", encoding="utf-8") as f:
        json.dump(lstm_results_serializable, f, indent=2, ensure_ascii=False)
    print("LSTM 搜索结果已保存至 lstm_grid_results.json")
    
    print("\n" + "=" * 50)
    print("开始 Bi-GRU 网格搜索...")
    print(f"[INFO] 参数组合数: {len(list(itertools.product(*BIGRU_PARAM_GRID.values())))}")
    
    # 清除Keras会话
    tf.keras.backend.clear_session()
    gc.collect()
    
    best_bigru, bigru_results = bigru_grid_search(X_train, y_train, look_back=30, n_splits=3)
    
    # 确保所有值都是可序列化的
    bigru_results_serializable = []
    for r in bigru_results:
        serializable_r = {}
        for k, v in r.items():
            if isinstance(v, (np.floating, float)):
                serializable_r[k] = float(v)
            elif isinstance(v, (np.integer, int)):
                serializable_r[k] = int(v)
            else:
                serializable_r[k] = v
        bigru_results_serializable.append(serializable_r)
    
    with open("bigru_grid_results.json", "w", encoding="utf-8") as f:
        json.dump(bigru_results_serializable, f, indent=2, ensure_ascii=False)
    print("Bi-GRU 搜索结果已保存至 bigru_grid_results.json")

    print("\n" + "=" * 50)
    print("开始 ARIMA 网格搜索...")
    best_order, arima_results = arima_grid_search(train_series, val_series)
    
    # 确保所有值都是可序列化的
    arima_results_serializable = []
    for r in arima_results:
        serializable_r = {}
        for k, v in r.items():
            if isinstance(v, float):
                # 处理无穷大和NaN值
                if np.isinf(v):
                    serializable_r[k] = 999999.99  # 用一个大数代替无穷大
                elif np.isnan(v):
                    serializable_r[k] = -999999.99  # 用一个大负数代替NaN
                else:
                    serializable_r[k] = round(float(v), 2)
            elif isinstance(v, (np.floating,)):
                serializable_r[k] = round(float(v), 2)
            elif isinstance(v, (np.integer, int)):
                serializable_r[k] = int(v)
            else:
                serializable_r[k] = v
        arima_results_serializable.append(serializable_r)
    
    with open("arima_grid_results.json", "w", encoding="utf-8") as f:
        json.dump(arima_results_serializable, f, indent=2, ensure_ascii=False)
    print("ARIMA 搜索结果已保存至 arima_grid_results.json")

    # 打印最优参数汇总
    print("\n" + "=" * 50)
    print("最优参数汇总：")
    print(f"LSTM:   {best_lstm}")
    print(f"Bi-GRU: {best_bigru}")
    print(f"ARIMA:  {best_order}")