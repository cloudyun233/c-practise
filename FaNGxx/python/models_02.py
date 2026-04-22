"""
models_02.py
基于网格搜索的短时交通量预测研究 —— 模型构建模块
LSTM、Bi-GRU、ARIMA 三类模型定义与训练封装
作者：方义凯   学号：632210040410
"""

import numpy as np
import warnings
warnings.filterwarnings("ignore")
from sklearn.metrics import mean_squared_error, mean_absolute_error, r2_score

# ──────────────────────────────────────────────
# 评估指标
# ──────────────────────────────────────────────
def compute_metrics(y_true: np.ndarray, y_pred: np.ndarray) -> dict:
    """计算 RMSE / MAE / MAPE / R²"""
    from sklearn.metrics import mean_squared_error, mean_absolute_error, r2_score
    y_true, y_pred = y_true.flatten(), y_pred.flatten()
    rmse  = np.sqrt(mean_squared_error(y_true, y_pred))
    mae   = mean_absolute_error(y_true, y_pred)
    mask  = y_true != 0
    mape  = np.mean(np.abs((y_true[mask] - y_pred[mask]) / y_true[mask])) * 100
    r2    = r2_score(y_true, y_pred)
    return {"RMSE": round(rmse, 6), "MAE": round(mae, 6),
            "MAPE%": round(mape, 4), "R2": round(r2, 6)}


# ──────────────────────────────────────────────
# LSTM 模型
# ──────────────────────────────────────────────
def build_lstm(look_back: int = 30,
               units: int = 64,
               lr: float = 0.001,
               dropout: float = 0.2,
               use_mixed_precision: bool = False):
    """
    构建单层 LSTM 模型。
    use_mixed_precision: 是否使用混合精度训练（FP16），可加速GPU计算
    """
    import tensorflow as tf
    from tensorflow.keras import layers
    
    # 启用混合精度训练（可选）
    if use_mixed_precision:
        try:
            policy = tf.keras.mixed_precision.Policy('mixed_float16')
            tf.keras.mixed_precision.set_global_policy(policy)
        except Exception:
            pass  # 如果失败则使用默认精度

    model = tf.keras.Sequential([
        layers.Input(shape=(look_back, 1)),
        layers.LSTM(units, return_sequences=False),
        layers.Dropout(dropout),
        layers.Dense(1, dtype='float32')  # 输出层使用float32保证精度
    ], name="LSTM_model")

    model.compile(optimizer=tf.keras.optimizers.Adam(learning_rate=lr),
                  loss="mse")
    return model


def train_lstm(X_train, y_train, X_val, y_val,
               units=64, lr=0.001, dropout=0.2, batch_size=32,
               epochs=50, look_back=30, verbose=0):
    """训练 LSTM；返回 (model, history)。"""
    import tensorflow as tf
    # reshape to (N, T, 1)
    Xt = X_train.reshape(-1, look_back, 1)
    Xv = X_val.reshape(-1, look_back, 1)

    model = build_lstm(look_back, units, lr, dropout)
    es = tf.keras.callbacks.EarlyStopping(monitor="val_loss", patience=5, restore_best_weights=True)
    history = model.fit(Xt, y_train, validation_data=(Xv, y_val),
                        batch_size=batch_size, epochs=epochs,
                        callbacks=[es], verbose=verbose)
    return model, history


# ──────────────────────────────────────────────
# Bi-GRU 模型
# ──────────────────────────────────────────────
def build_bigru(look_back: int = 30,
                units: int = 64,
                lr: float = 0.001,
                dropout: float = 0.2):
    """构建双向 GRU 模型。"""
    import tensorflow as tf
    from tensorflow.keras import layers

    model = tf.keras.Sequential([
        layers.Input(shape=(look_back, 1)),
        layers.Bidirectional(layers.GRU(units, return_sequences=False)),
        layers.Dropout(dropout),
        layers.Dense(1)
    ], name="BiGRU_model")

    model.compile(optimizer=tf.keras.optimizers.Adam(learning_rate=lr),
                  loss="mse")
    return model


def train_bigru(X_train, y_train, X_val, y_val,
                units=64, lr=0.001, dropout=0.2, batch_size=32,
                epochs=100, look_back=30, verbose=0):
    """训练 Bi-GRU；返回 (model, history)。"""
    import tensorflow as tf
    Xt = X_train.reshape(-1, look_back, 1)
    Xv = X_val.reshape(-1, look_back, 1)

    model = build_bigru(look_back, units, lr, dropout)
    es = tf.keras.callbacks.EarlyStopping(monitor="val_loss", patience=5, restore_best_weights=True)
    history = model.fit(Xt, y_train, validation_data=(Xv, y_val),
                        batch_size=batch_size, epochs=epochs,
                        callbacks=[es], verbose=verbose)
    return model, history




# ──────────────────────────────────────────────
# ARIMA 模型
# ──────────────────────────────────────────────
def train_arima(train_series: np.ndarray, p: int = 2, d: int = 1, q: int = 2,
                seasonal_order=None, verbose: bool = True):
    """
    训练ARIMA/SARIMA模型
    :param train_series: 训练序列
    :param p: AR阶数
    :param d: 差分阶数
    :param q: MA阶数
    :param seasonal_order: 季节性参数 (P,D,Q,s)，None表示非季节性
    :param verbose: 是否打印详细信息
    :return: 拟合后的模型对象
    """
    from statsmodels.tsa.arima.model import ARIMA
    from statsmodels.tsa.statespace.sarimax import SARIMAX

    if seasonal_order is not None and any(v != 0 for v in seasonal_order[:3]):
        # SARIMA模型（至少有一个季节性参数非零才使用SARIMA）
        model = SARIMAX(train_series, order=(p, d, q), seasonal_order=seasonal_order,
                        enforce_stationarity=False, enforce_invertibility=False)
        fitted_model = model.fit(disp=False, warn_convergence=False)
    else:
        # 普通ARIMA模型
        model = ARIMA(train_series, order=(p, d, q))
        fitted_model = model.fit()

    if verbose:
        print("\n" + "=" * 50)
        print(f"ARIMA({p},{d},{q}) 拟合成功")
        print(f"AIC={fitted_model.aic:.2f} | BIC={fitted_model.bic:.2f}")
        print("=" * 50 + "\n")

    return fitted_model


def predict_arima_static(fitted_model, test_series: np.ndarray, train_series: np.ndarray,
                         refit_step: int = 10) -> np.ndarray:
    """
    静态滚动预测（单步预测）
    使用真实值更新历史窗口，避免误差累积
    
    :param fitted_model: 初始拟合的ARIMA模型
    :param test_series: 测试集真实值序列
    :param train_series: 训练集序列
    :param refit_step: 重新拟合模型的步长间隔
    :return: 预测值数组
    """
    history = list(train_series)
    predictions = []
    
    # 获取模型阶数
    p = fitted_model.model.order[0]
    d = fitted_model.model.order[1]
    q = fitted_model.model.order[2]
    # 只有在真正含有非零季节性参数时才传入seasonal_order
    _so = getattr(fitted_model.model, 'seasonal_order', None)
    if _so is not None and any(v != 0 for v in _so[:3]):
        seasonal_order = _so
    else:
        seasonal_order = None
    
    for i in range(len(test_series)):
        # 定期重新拟合模型
        if i % refit_step == 0:
            current_model = train_arima(
                np.array(history), p, d, q, 
                seasonal_order=seasonal_order, verbose=False
            )
        
        # 单步预测
        yhat = current_model.forecast(steps=1)[0]
        predictions.append(yhat)
        
        # 用真实值更新历史窗口（静态预测核心）
        history.append(test_series[i])
        
        # 限制历史窗口长度，防止内存溢出
        if len(history) > 2000:
            history = history[-2000:]
    
    return np.array(predictions)


def predict_arima(fitted_model, n_steps: int = 1) -> np.ndarray:
    """
    一次性多步预测（仅用于短步长预测）
    :param fitted_model: 拟合好的ARIMA模型
    :param n_steps: 预测步数
    :return: 预测值数组
    """
    forecast = fitted_model.forecast(steps=n_steps)
    return np.array(forecast)


# ──────────────────────────────────────────────
# 时序检验工具函数
# ──────────────────────────────────────────────
def adf_stationary_test(series: np.ndarray, sig_level: float = 0.05) -> tuple:
    """
    ADF平稳性检验
    :param series: 时间序列
    :param sig_level: 显著性水平
    :return: (是否平稳, 最优差分阶数)
    """
    from statsmodels.tsa.stattools import adfuller
    max_d = 1  # 交通流强制1阶差分，禁止过度差分
    d = 0
    current_series = series.copy()

    for d_test in range(0, max_d + 1):
        if d_test > 0:
            current_series = np.diff(current_series, n=1)
        adf_result = adfuller(current_series)
        p_value = adf_result[1]
        if p_value < sig_level:
            print(f"[ADF检验] {d_test}阶差分后序列平稳，p值={p_value:.6f} < {sig_level}")
            return True, d_test
    print(f"[WARN] 序列经{max_d}阶差分后仍不平稳")
    return False, max_d


def ljung_box_white_noise_test(series: np.ndarray, lags: int = 10, sig_level: float = 0.05,
                               is_residual: bool = False) -> bool:
    """
    Ljung-Box白噪声检验
    :param series: 待检验序列
    :param lags: 滞后阶数
    :param sig_level: 显著性水平
    :param is_residual: 是否为残差序列
    :return: 是否为白噪声
    """
    from statsmodels.stats.diagnostic import acorr_ljungbox
    lb_result = acorr_ljungbox(series, lags=[lags], return_df=True)
    p_value = lb_result["lb_pvalue"].values[0]
    is_white_noise = p_value > sig_level

    # 区分原始序列/残差，修复提示逻辑
    if is_residual:
        if is_white_noise:
            print(f"[残差检验] ✅ 白噪声(p={p_value:.6f})，模型拟合完美")
        else:
            print(f"[残差检验] ❌ 非白噪声(p={p_value:.6f})，模型有优化空间")
    else:
        if is_white_noise:
            print(f"[原始序列] ❌ 白噪声(p={p_value:.6f})，无建模价值")
        else:
            print(f"[原始序列] ✅ 非白噪声(p={p_value:.6f})，可建模")
    return is_white_noise


def get_pq_range_by_acf_pacf(series: np.ndarray, max_lag: int = 10, sig_level: float = 0.05) -> tuple:
    """
    基于ACF/PACF图确定p和q的候选范围
    :param series: 平稳时间序列
    :param max_lag: 最大滞后阶数
    :param sig_level: 显著性水平
    :return: (p候选列表, q候选列表)
    """
    from statsmodels.tsa.stattools import acf, pacf
    conf_int = 1.96 / np.sqrt(len(series))
    acf_vals = acf(series, nlags=max_lag)
    pacf_vals = pacf(series, nlags=max_lag)

    q_candidate = [i for i in range(1, 4) if abs(acf_vals[i]) < conf_int][:3] or [1, 2, 3]
    p_candidate = [i for i in range(1, 4) if abs(pacf_vals[i]) < conf_int][:3] or [1, 2, 3]
    print(f"[ACF/PACF定阶] p={p_candidate}, q={q_candidate}")
    return p_candidate, q_candidate


# ──────────────────────────────────────────────
# 通用预测接口
# ──────────────────────────────────────────────
def predict_dl(model, X_test: np.ndarray, look_back: int = 30) -> np.ndarray:
    """对深度学习模型（LSTM/Bi-GRU）进行预测。"""
    Xt = X_test.reshape(-1, look_back, 1)
    return model.predict(Xt, verbose=0).flatten()
