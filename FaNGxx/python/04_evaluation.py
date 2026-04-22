"""
04_evaluation.py
基于网格搜索的短时交通量预测研究 —— 模型评估与可视化模块
作者：方义凯   学号：632210040410
修改：完整版评估脚本，加载真实数据并绘制所有图表
"""

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib
import warnings
warnings.filterwarnings("ignore")

matplotlib.rcParams["font.family"] = "SimHei"
matplotlib.rcParams["axes.unicode_minus"] = False


from models_02 import compute_metrics, build_lstm, build_bigru, train_arima, predict_dl, predict_arima_static


# ──────────────────────────────────────────────
# 1. 全面性能对比表
# ──────────────────────────────────────────────
def compare_models(results_dict: dict) -> pd.DataFrame:
    """
    results_dict: {model_name: (y_true, y_pred)}
    返回汇总 DataFrame，包含 RMSE/MAE/MAPE/R²
    """
    rows = []
    for name, (yt, yp) in results_dict.items():
        m = compute_metrics(yt, yp)
        m["Model"] = name
        rows.append(m)
    df = pd.DataFrame(rows).set_index("Model")
    print("\n===== 模型性能对比 =====")
    print(df.to_string())
    return df


# ──────────────────────────────────────────────
# 2. 预测曲线对比图
# ──────────────────────────────────────────────
def plot_predictions(y_true: np.ndarray,
                     preds_dict: dict,
                     n_samples: int = 200,
                     save_path: str = None):
    """
    绘制真实值与各模型预测值对比曲线。
    preds_dict: {label: y_pred}
    """
    y_true = y_true.flatten()[:n_samples]
    palette = ["#E63946", "#457B9D", "#2A9D8F", "#E9C46A", "#F4A261"]

    plt.figure(figsize=(14, 5))
    plt.plot(y_true, label="真实值", color="black", linewidth=1.5, alpha=0.85)
    for (label, yp), color in zip(preds_dict.items(), palette):
        plt.plot(yp.flatten()[:n_samples], label=label,
                 color=color, linewidth=1.0, linestyle="--", alpha=0.8)

    plt.title("各模型预测结果对比（前200个时步）", fontsize=14)
    plt.xlabel("时间步"); plt.ylabel("归一化流量")
    plt.legend(); plt.tight_layout()
    if save_path:
        plt.savefig(save_path, dpi=150)
        print(f"[INFO] 预测对比图保存至：{save_path}")
    plt.show()


# ──────────────────────────────────────────────
# 3. 参数影响热力图（学习率 × 隐藏单元数）
# ──────────────────────────────────────────────
def plot_param_heatmap(results_list: list,
                       param_x: str = "lr",
                       param_y: str = "units",
                       metric: str = "cv_rmse",
                       model_name: str = "LSTM",
                       save_path: str = None):
    """
    绘制两个超参数对交叉验证指标的热力图。
    results_list: 网格搜索返回的 all_results 列表
    """
    import seaborn as sns
    df = pd.DataFrame(results_list)
    if metric not in df.columns or param_x not in df.columns:
        print("[WARN] 数据列缺失，跳过热力图。")
        return

    pivot = df.pivot_table(values=metric, index=param_y, columns=param_x, aggfunc="min")
    plt.figure(figsize=(8, 5))
    sns.heatmap(pivot, annot=True, fmt=".4f", cmap="YlOrRd_r",
                cbar_kws={"label": metric})
    plt.title(f"{model_name} 参数热力图（{param_y} vs {param_x}）", fontsize=13)
    plt.tight_layout()
    if save_path:
        plt.savefig(save_path, dpi=150)
        print(f"[INFO] 热力图保存至：{save_path}")
    plt.show()


# ──────────────────────────────────────────────
# 4. 特殊场景鲁棒性评估
# ──────────────────────────────────────────────
def evaluate_robustness(y_true_all: np.ndarray,
                        y_pred_all: np.ndarray,
                        timestamps: pd.DatetimeIndex = None,
                        n_test: int = None):
    """
    针对高峰时段（7-9h / 17-19h）单独计算误差，
    验证优化后模型的鲁棒性。
    如果没有时间戳，则随机划分模拟高峰/非高峰
    """
    if timestamps is not None and n_test is not None:
        ts = timestamps[-n_test:]
        hours = ts.hour
        peak_mask = ((hours >= 7) & (hours <= 9)) | ((hours >= 17) & (hours <= 19))
        offpeak_mask = ~peak_mask
    else:
        n = len(y_true_all)
        peak_mask = np.zeros(n, dtype=bool)
        peak_mask[:n//3] = True
        offpeak_mask = ~peak_mask

    def _report(name, mask):
        if mask.sum() == 0:
            return
        yt = y_true_all[mask]
        yp = y_pred_all[mask]
        m  = compute_metrics(yt, yp)
        print(f"  [{name}] RMSE={m['RMSE']:.6f}  MAE={m['MAE']:.6f}  "
              f"MAPE%={m['MAPE%']:.2f}  R²={m['R2']:.4f}")

    print("\n===== 鲁棒性分析 =====")
    _report("高峰时段", peak_mask)
    _report("非高峰时段", offpeak_mask)


# ──────────────────────────────────────────────
# 5. 学习率影响曲线
# ──────────────────────────────────────────────
def plot_lr_curve(results_list: list, model_name: str = "LSTM",
                  save_path: str = None):
    """绘制学习率与 CV-RMSE 的关系曲线。"""
    df = pd.DataFrame(results_list)
    if "lr" not in df.columns or "cv_rmse" not in df.columns:
        return
    grouped = df.groupby("lr")["cv_rmse"].mean().reset_index()
    plt.figure(figsize=(7, 4))
    plt.plot(grouped["lr"].astype(str), grouped["cv_rmse"],
             marker="o", color="#457B9D", linewidth=2)
    plt.title(f"{model_name}: 学习率 vs 平均 CV-RMSE", fontsize=13)
    plt.xlabel("学习率"); plt.ylabel("平均 CV-RMSE")
    plt.xticks(rotation=45)
    plt.tight_layout()
    if save_path:
        plt.savefig(save_path, dpi=150)
    plt.show()


# ────────────────────────────────────────────
# 6. 残差分布图
# ──────────────────────────────────────────────
def plot_error_distribution(y_true, preds_dict, save_path="error_distribution.png"):
    """
    绘制各模型预测残差的分布直方图。
    preds_dict: {模型名称: y_pred数组}
    """
    import scipy.stats as stats

    n_models = len(preds_dict)
    fig, axes = plt.subplots(1, n_models, figsize=(6*n_models, 5))
    if n_models == 1:
        axes = [axes]
    
    colors = ["#2E86AB", "#A23B72", "#F18F01", "#C73E1D"]
    
    for i, (name, y_pred) in enumerate(preds_dict.items()):
        residuals = y_true.flatten() - y_pred.flatten()
        ax = axes[i]
        n, bins, patches = ax.hist(residuals, bins=40, density=True,
                                    alpha=0.7, color=colors[i % len(colors)], edgecolor='black')
        mu, std = np.mean(residuals), np.std(residuals)
        x = np.linspace(mu - 3*std, mu + 3*std, 100)
        ax.plot(x, stats.norm.pdf(x, mu, std), 'r--', linewidth=2, label='正态拟合')
        ax.set_title(f"{name}\n均值:{mu:.4f}, 标准差:{std:.4f}", fontsize=11)
        ax.set_xlabel("预测误差")
        ax.set_ylabel("概率密度")
        ax.legend()
        
    plt.suptitle("各模型预测残差分布对比", fontsize=14, y=1.02)
    plt.tight_layout()
    plt.savefig(save_path, dpi=150, bbox_inches='tight')
    print(f"[INFO] 残差分布图保存至：{save_path}")
    plt.show()


# ────────────────────────────────────────────
# 7. 散点图（真实值 vs 预测值）
# ──────────────────────────────────────────────
def plot_scatter_comparison(y_true, preds_dict, save_path="scatter_comparison.png"):
    """绘制真实值与预测值的散点对比图"""
    n_models = len(preds_dict)
    fig, axes = plt.subplots(1, n_models, figsize=(5*n_models, 5))
    if n_models == 1:
        axes = [axes]
    
    colors = ["#2E86AB", "#A23B72", "#F18F01", "#C73E1D"]
    
    for i, (name, y_pred) in enumerate(preds_dict.items()):
        ax = axes[i]
        yt = y_true.flatten()
        yp = y_pred.flatten()
        ax.scatter(yt, yp, alpha=0.5, s=10, color=colors[i % len(colors)])
        
        min_val = min(yt.min(), yp.min())
        max_val = max(yt.max(), yp.max())
        ax.plot([min_val, max_val], [min_val, max_val], 'r--', linewidth=2, label='理想线')
        
        metrics = compute_metrics(yt, yp)
        ax.set_title(f"{name}\nR²={metrics['R2']:.4f}", fontsize=11)
        ax.set_xlabel("真实值")
        ax.set_ylabel("预测值")
        ax.legend()
        
    plt.suptitle("真实值 vs 预测值散点图", fontsize=14, y=1.02)
    plt.tight_layout()
    plt.savefig(save_path, dpi=150, bbox_inches='tight')
    print(f"[INFO] 散点图保存至：{save_path}")
    plt.show()


# ────────────────────────────────────────────
# 8. 箱线图（多模型误差对比）
# ──────────────────────────────────────────────
def plot_boxplot_errors(y_true, preds_dict, save_path="boxplot_errors.png"):
    """绘制各模型绝对误差的箱线图"""
    errors_data = []
    labels = []
    for name, y_pred in preds_dict.items():
        abs_error = np.abs(y_true.flatten() - y_pred.flatten())
        errors_data.append(abs_error)
        labels.append(name)
    
    plt.figure(figsize=(10, 6))
    bp = plt.boxplot(errors_data, labels=labels, patch_artist=True)
    colors = ["#2E86AB", "#A23B72", "#F18F01", "#C73E1D"]
    for patch, color in zip(bp['boxes'], colors):
        patch.set_facecolor(color)
        patch.set_alpha(0.6)
    
    plt.title("各模型绝对误差箱线图", fontsize=14)
    plt.xlabel("模型")
    plt.ylabel("绝对误差")
    plt.xticks(rotation=15)
    plt.grid(axis='y', alpha=0.3)
    plt.tight_layout()
    plt.savefig(save_path, dpi=150, bbox_inches='tight')
    print(f"[INFO] 箱线图保存至：{save_path}")
    plt.show()


# ────────────────────────────────────────────
# 9. 训练损失曲线图
# ──────────────────────────────────────────────
def plot_training_loss_curves(histories_dict, save_path="loss_curves.png"):
    """
    绘制LSTM和Bi-GRU的训练损失曲线
    histories_dict: {模型名称: history对象}
    """
    fig, axes = plt.subplots(1, 2, figsize=(14, 5))
    
    colors_train = ['#2E86AB', '#A23B72']
    colors_val = ['#F18F01', '#C73E1D']
    
    for i, (name, history) in enumerate(histories_dict.items()):
        ax = axes[i]
        epochs = range(1, len(history.history['loss']) + 1)
        
        ax.plot(epochs, history.history['loss'], 
                label='训练损失', color=colors_train[i], linewidth=2, marker='o', markersize=4)
        ax.plot(epochs, history.history['val_loss'], 
                label='验证损失', color=colors_val[i], linewidth=2, marker='s', markersize=4)
        
        ax.set_title(f"{name} 训练过程", fontsize=13)
        ax.set_xlabel('Epoch')
        ax.set_ylabel('Loss (MSE)')
        ax.legend(loc='best')
        ax.grid(alpha=0.3)
        
        # 标注最小验证损失点
        min_val_loss = min(history.history['val_loss'])
        min_epoch = history.history['val_loss'].index(min_val_loss) + 1
        ax.axhline(y=min_val_loss, color=colors_val[i], linestyle='--', alpha=0.5)
        ax.annotate(f'最小验证损失: {min_val_loss:.6f}\n@ Epoch {min_epoch}',
                   xy=(min_epoch, min_val_loss),
                   xytext=(min_epoch + 5, min_val_loss + 0.005),
                   fontsize=9,
                   bbox=dict(boxstyle='round,pad=0.5', facecolor='yellow', alpha=0.3),
                   arrowprops=dict(arrowstyle='->', connectionstyle='arc3,rad=0'))
    
    plt.suptitle("LSTM与Bi-GRU训练损失曲线对比", fontsize=15, y=1.02)
    plt.tight_layout()
    plt.savefig(save_path, dpi=150, bbox_inches='tight')
    print(f"[INFO] 训练损失曲线保存至：{save_path}")
    plt.show()


# ──────────────────────────────────────────────
# Main
# ──────────────────────────────────────────────
if __name__ == "__main__":
    import os, json
    
    print("=" * 60)
    print("开始模型评估与可视化...")
    print("=" * 60)
    # 1. 加载测试数据
    try:
        X_test = np.load("X_test.npy")
        y_test = np.load("y_test.npy")
        X_train = np.load("X_train.npy")
        y_train = np.load("y_train.npy")
        X_val = np.load("X_val.npy")
        y_val = np.load("y_val.npy")
        print("[INFO] 数据加载成功")
    except FileNotFoundError as e:
        print(f"[ERROR] 数据文件未找到：{e}")
        print("请先运行 01_data_preprocessing_late.py 生成数据")
        exit(1)
    
    LOOK_BACK = 30
    
    # 2. 定义最优参数（从网格搜索结果中获取）
    best_lstm_params = {"units": 64, "lr": 0.001, "dropout": 0.2, "batch_size": 32}
    best_bigru_params = {"units": 64, "lr": 0.001, "epochs": 100, "batch_size": 32}
    best_arima_order = (2, 1, 2)
    
    # 尝试从JSON文件读取最优参数
    if os.path.exists("lstm_grid_results.json"):
        with open("lstm_grid_results.json", "r") as f:
            lstm_results = json.load(f)
            valid_results = [r for r in lstm_results if r["cv_rmse"] != float('inf')]
            if valid_results:
                best_result = min(valid_results, key=lambda x: x["cv_rmse"])
                best_lstm_params = {k: best_result[k] for k in ["units", "lr", "dropout", "batch_size"]}
                print(f"[INFO] 从JSON读取LSTM最优参数：{best_lstm_params}")
    
    if os.path.exists("bigru_grid_results.json"):
        with open("bigru_grid_results.json", "r") as f:
            bigru_results = json.load(f)
            valid_results = [r for r in bigru_results if r["cv_rmse"] != float('inf')]
            if valid_results:
                best_result = min(valid_results, key=lambda x: x["cv_rmse"])
                best_bigru_params = {k: best_result[k] for k in ["units", "lr", "epochs", "batch_size"]}
                print(f"[INFO] 从JSON读取Bi-GRU最优参数：{best_bigru_params}")
    
    if os.path.exists("arima_grid_results.json"):
        with open("arima_grid_results.json", "r") as f:
            arima_results = json.load(f)
            valid_results = [r for r in arima_results if r["rmse"] != float('inf') and r["rmse"] < 999999]
            if valid_results:
                best_result = min(valid_results, key=lambda x: x["aic"])
                best_arima_order = (best_result["p"], best_result["d"], best_result["q"])
                print(f"[INFO] 从JSON读取ARIMA最优参数（基于AIC）：{best_arima_order}")
    
    # 3. 训练模型并进行预测
    print("\n" + "=" * 60)
    print("正在训练和预测各模型...")
    print("=" * 60)
    
    y_true = y_test.flatten()
    predictions = {}
    histories = {}  # 保存训练历史
    
    # LSTM（未优化）
    print("\n[1/5] LSTM（未优化）...")
    lstm_default = build_lstm(look_back=LOOK_BACK, units=64, lr=0.001, dropout=0.2)
    Xt_train = X_train.reshape(-1, LOOK_BACK, 1)
    Xv_train = X_val.reshape(-1, LOOK_BACK, 1)
    history_lstm_default = lstm_default.fit(Xt_train, y_train, validation_data=(Xv_train, y_val),
                     epochs=30, batch_size=32, verbose=0)
    pred_lstm_default = predict_dl(lstm_default, X_test, LOOK_BACK)
    predictions["LSTM（未优化）"] = pred_lstm_default
    print(f"  RMSE: {compute_metrics(y_true, pred_lstm_default)['RMSE']:.6f}")
    
    # LSTM（网格搜索优化）
    print("\n[2/5] LSTM（网格搜索优化）...")
    lstm_optimized = build_lstm(look_back=LOOK_BACK, 
                                units=best_lstm_params["units"],
                                lr=best_lstm_params["lr"],
                                dropout=best_lstm_params["dropout"])
    history_lstm_optimized = lstm_optimized.fit(Xt_train, y_train, validation_data=(Xv_train, y_val),
                       epochs=30, batch_size=best_lstm_params["batch_size"], verbose=0)
    pred_lstm_optimized = predict_dl(lstm_optimized, X_test, LOOK_BACK)
    predictions["LSTM（网格搜索）"] = pred_lstm_optimized
    print(f"  RMSE: {compute_metrics(y_true, pred_lstm_optimized)['RMSE']:.6f}")
    histories["LSTM（网格搜索）"] = history_lstm_optimized
    
    # Bi-GRU（网格搜索优化）
    print("\n[3/5] Bi-GRU（网格搜索优化）...")
    bigru_optimized = build_bigru(look_back=LOOK_BACK,
                                  units=best_bigru_params["units"],
                                  lr=best_bigru_params["lr"],
                                  dropout=0.2)
    history_bigru = bigru_optimized.fit(Xt_train, y_train, validation_data=(Xv_train, y_val),
                        epochs=best_bigru_params["epochs"],
                        batch_size=best_bigru_params["batch_size"], verbose=0)
    pred_bigru = predict_dl(bigru_optimized, X_test, LOOK_BACK)
    predictions["Bi-GRU（网格搜索）"] = pred_bigru
    print(f"  RMSE: {compute_metrics(y_true, pred_bigru)['RMSE']:.6f}")
    histories["Bi-GRU（网格搜索）"] = history_bigru
    
    # ARIMA（网格搜索优化）
    print("\n[4/5] ARIMA（网格搜索优化）...")
    try:
        from models_02 import train_arima, predict_arima_static

        # 从滑动窗口样本还原连续时间序列
        # X_train 形状: (N, look_back)，每个样本末尾比上一个样本多一步
        def rebuild_series(X):
            series = list(X[0].flatten())
            for i in range(1, len(X)):
                series.append(float(X[i][-1]))
            return np.array(series)

        train_series = rebuild_series(X_train)
        val_series   = rebuild_series(X_val)

        # 将 train+val 拼接作为 ARIMA 拟合的完整历史序列，
        # 使模型在预测测试集时拥有更多上下文信息
        full_history = np.concatenate([train_series, val_series])

        print(f"  [INFO] ARIMA历史序列长度: {len(full_history)}")
        print(f"  [INFO] ARIMA预测步数: {len(y_test.flatten())}")

        # 训练ARIMA模型
        p, d, q = best_arima_order
        arima_model = train_arima(full_history, p=p, d=d, q=q, verbose=False)

        # 静态滚动预测：每步用真实观测值更新历史窗口，避免误差累积
        pred_arima = predict_arima_static(
            fitted_model=arima_model,
            test_series=y_test.flatten(),
            train_series=full_history,
            refit_step=10
        )

        predictions["ARIMA（网格搜索）"] = pred_arima
        print(f"  RMSE: {compute_metrics(y_true, pred_arima)['RMSE']:.6f}")
    except Exception as e:
        print(f"  [WARN] ARIMA预测失败: {e}")
        import traceback
        traceback.print_exc()


    
    # 贝叶斯优化的LSTM（如果已运行05_bayesian_optimization.py）
    if os.path.exists("bayesian_results.npy"):
        print("\n[5/5] LSTM（贝叶斯优化）...")
        try:
            bayes_results = np.load("bayesian_results.npy", allow_pickle=True).item()
            best_bayes_params = bayes_results["best_params"]
            lstm_bayes = build_lstm(look_back=LOOK_BACK,
                                   units=int(best_bayes_params["units"]),
                                   lr=float(best_bayes_params["lr"]),
                                   dropout=float(best_bayes_params["dropout"]))
            lstm_bayes.fit(Xt_train, y_train, validation_data=(Xv_train, y_val),
                          epochs=30, batch_size=int(best_bayes_params["batch_size"]), verbose=0)
            pred_lstm_bayes = predict_dl(lstm_bayes, X_test, LOOK_BACK)
            predictions["LSTM（贝叶斯优化）"] = pred_lstm_bayes
            print(f"  RMSE: {compute_metrics(y_true, pred_lstm_bayes)['RMSE']:.6f}")
        except Exception as e:
            print(f"  [WARN] 贝叶斯优化结果加载失败：{e}")
    
    # 4. 性能对比表
    print("\n" + "=" * 60)
    results_dict = {name: (y_true, pred) for name, pred in predictions.items()}
    performance_df = compare_models(results_dict)
    
    # 保存性能对比表
    performance_df.to_csv("model_performance.csv", encoding='utf-8-sig')
    print("\n[INFO] 性能对比表已保存至：model_performance.csv")
    
    # 5. 绘制所有图表
    print("\n" + "=" * 60)
    print("正在生成可视化图表...")
    print("=" * 60)
    
    # 图1：预测曲线对比
    print("\n[图1/6] 生成预测曲线对比图...")
    plot_predictions(y_true, predictions, n_samples=200,
                    save_path="comparison_plot.png")
    
    # 图2：残差分布
    print("\n[图2/6] 生成残差分布图...")
    plot_error_distribution(y_true, predictions, save_path="error_distribution.png")
    
    # 图3：散点图
    print("\n[图3/6] 生成散点图...")
    plot_scatter_comparison(y_true, predictions, save_path="scatter_comparison.png")
    
    # 图4：箱线图
    print("\n[图4/6] 生成箱线图...")
    plot_boxplot_errors(y_true, predictions, save_path="boxplot_errors.png")
    
    # 图5：参数热力图（如果有网格搜索结果）
    if os.path.exists("lstm_grid_results.json"):
        print("\n[图5/6] 生成LSTM参数热力图...")
        with open("lstm_grid_results.json", "r") as f:
            lstm_results = json.load(f)
        plot_param_heatmap(lstm_results, param_x="lr", param_y="units",
                          metric="cv_rmse", model_name="LSTM",
                          save_path="lstm_heatmap.png")
    
    # 图6：学习率曲线
    if os.path.exists("lstm_grid_results.json"):
        print("\n[图6/7] 生成学习率影响曲线...")
        with open("lstm_grid_results.json", "r") as f:
            lstm_results = json.load(f)
        plot_lr_curve(lstm_results, model_name="LSTM",
                     save_path="lr_curve.png")
    
    # 图7：训练损失曲线
    if histories:
        print("\n[图7/7] 生成训练损失曲线...")
        plot_training_loss_curves(histories, save_path="loss_curves.png")
    
    # 6. 鲁棒性分析
    print("\n" + "=" * 60)
    print("鲁棒性分析（以LSTM网格搜索优化模型为例）...")
    print("=" * 60)
    evaluate_robustness(y_true, pred_lstm_optimized)
    
    # 总结
    print("\n" + "=" * 60)
    print("评估完成！所有图表已保存。")
    print("=" * 60)
    print("\n生成的文件：")
    print("  - model_performance.csv  : 性能对比表")
    print("  - comparison_plot.png    : 预测曲线对比图")
    print("  - error_distribution.png : 残差分布图")
    print("  - scatter_comparison.png : 散点图")
    print("  - boxplot_errors.png     : 箱线图")
    if os.path.exists("lstm_grid_results.json"):
        print("  - lstm_heatmap.png       : LSTM参数热力图")
        print("  - lr_curve.png           : 学习率影响曲线")
    if histories:
        print("  - loss_curves.png        : LSTM与Bi-GRU训练损失曲线")
    print("\n所有任务完成！")
