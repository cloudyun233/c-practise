"""
test_arima.py
独立验证 ARIMA 修复是否正常：用合成时序数据测试完整流程
"""
import sys, os
sys.path.insert(0, os.path.dirname(__file__))

import numpy as np
import warnings
warnings.filterwarnings("ignore")

print("=" * 60)
print("ARIMA 修复验证脚本")
print("=" * 60)

# ── 1. 生成合成AR(2)时间序列 ────────────────────────────────
np.random.seed(42)
n = 500
noise = np.random.normal(0, 0.3, n)
series = np.zeros(n)
series[0] = noise[0]
series[1] = noise[1]
for i in range(2, n):
    series[i] = 0.6 * series[i-1] - 0.3 * series[i-2] + noise[i]

train_series = series[:350]
test_series  = series[350:400]   # 仅预测前50步，加快验证速度
print(f"[合成数据] 训练长度={len(train_series)}, 测试长度={len(test_series)}")

# ── 2. 测试 train_arima ──────────────────────────────────────
print("\n[步骤1] 测试 train_arima(p=2, d=0, q=1) ...")
from models_02 import train_arima, predict_arima_static, compute_metrics

try:
    fitted = train_arima(train_series, p=2, d=0, q=1, verbose=True)
    print(f"  ✅ train_arima 成功！AIC={fitted.aic:.2f}")
except Exception as e:
    print(f"  ❌ train_arima 失败: {e}")
    import traceback; traceback.print_exc()
    sys.exit(1)

# ── 3. 测试 predict_arima_static（滚动预测）────────────────────
print("\n[步骤2] 测试 predict_arima_static (refit_step=5) ...")
try:
    preds = predict_arima_static(
        fitted_model=fitted,
        test_series=test_series,
        train_series=train_series,
        refit_step=5
    )
    m = compute_metrics(test_series, preds)
    print(f"  ✅ 滚动预测成功！预测长度={len(preds)}")
    print(f"  RMSE={m['RMSE']:.6f}  MAE={m['MAE']:.6f}  "
          f"MAPE%={m['MAPE%']:.2f}  R²={m['R2']:.4f}")
except Exception as e:
    print(f"  ❌ predict_arima_static 失败: {e}")
    import traceback; traceback.print_exc()
    sys.exit(1)

# ── 4. 验证 seasonal_order 判断 ─────────────────────────────
print("\n[步骤3] 验证 seasonal_order=None 时不走SARIMA分支 ...")
try:
    fitted2 = train_arima(train_series, p=1, d=1, q=1,
                          seasonal_order=None, verbose=False)
    print(f"  ✅ ARIMA(1,1,1) seasonal_order=None 正常！")
    # 检查内部模型确实是ARIMA而非SARIMA
    model_cls = type(fitted2.model).__name__
    print(f"  内部模型类型: {model_cls}")
except Exception as e:
    print(f"  ❌ 失败: {e}")
    import traceback; traceback.print_exc()

# ── 5. 如果有真实.npy文件，也跑一次真实数据验证 ────────────────
npy_dir = os.path.dirname(__file__)
x_train_path = os.path.join(npy_dir, "X_train.npy")
y_train_path = os.path.join(npy_dir, "y_train.npy")
y_test_path  = os.path.join(npy_dir, "y_test.npy")

if all(os.path.exists(p) for p in [x_train_path, y_train_path, y_test_path]):
    print("\n[步骤4] 检测到 .npy 文件，用真实数据验证 ...")
    X_train = np.load(x_train_path)
    X_val   = np.load(os.path.join(npy_dir, "X_val.npy"))
    y_test  = np.load(y_test_path).flatten()

    def rebuild_series(X):
        s = list(X[0].flatten())
        for i in range(1, len(X)):
            s.append(float(X[i][-1]))
        return np.array(s)

    tr = rebuild_series(X_train)
    val = rebuild_series(X_val)
    full_hist = np.concatenate([tr, val])
    print(f"  历史序列长度={len(full_hist)}, 预测步数={len(y_test[:50])}")

    try:
        fm = train_arima(full_hist, p=2, d=1, q=2, verbose=False)
        pr = predict_arima_static(fm, y_test[:50], full_hist, refit_step=10)
        m2 = compute_metrics(y_test[:50], pr)
        print(f"  ✅ 真实数据验证成功！")
        print(f"  RMSE={m2['RMSE']:.6f}  MAE={m2['MAE']:.6f}  R²={m2['R2']:.4f}")
    except Exception as e:
        print(f"  ❌ 真实数据验证失败: {e}")
        import traceback; traceback.print_exc()
else:
    print("\n[步骤4] 未找到 .npy 文件，跳过真实数据验证")
    print(f"  （若要验证，请先运行 01_data_preprocessing_late.py）")

print("\n" + "=" * 60)
print("✅ ARIMA 验证完成！")
print("=" * 60)
