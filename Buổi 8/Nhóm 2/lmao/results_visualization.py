import json

import pandas as pd
from matplotlib import pyplot as plt


def main():
    with open("output/best_method.json", "r", encoding="utf-8") as file:
        best_method = json.load(file)

    y_test = best_method["y_test"]
    y_predict = best_method["y_predict"]

    # Figure 1: Actual vs Predicted
    fig, ax = plt.subplots(figsize=(7, 5), layout="constrained")

    ax.scatter(y_test, y_predict, alpha=0.3, s=25, label="Predictions")

    min_value = min(min(y_test), min(y_predict))
    max_value = max(max(y_test), max(y_predict))
    ax.plot(
        [min_value, max_value],
        [min_value, max_value],
        color="red",
        linestyle="--",
        label="Expected",
    )

    fig.suptitle("Actual vs Predicted Soil Moisture", fontweight="bold")
    ax.set(
        title=(
            f"{best_method['method']}\n"
            f"Test size: {best_method['test_size']:.0%}  |  "
            f"MAE: {best_method['mae']:.4f}"
        ),
        xlabel="Actual values",
        ylabel="Predicted values",
    )
    ax.legend()
    ax.grid(alpha=0.3)
    fig.savefig("output/actual_vs_predicted.png", dpi=300, bbox_inches="tight")
    plt.show(block=True)

    # Figure 2: MAE vs Test Size
    df = pd.read_csv("output/benchmark_results.csv")
    fig, ax = plt.subplots(figsize=(11, 5), layout="constrained")

    for method, method_data in df.groupby("method", sort=False):
        method_data = method_data.sort_values("test_size")
        ax.plot(
            method_data["test_size"] * 100,
            method_data["mae"],
            marker="o",
            label=method,
        )

    test_percentages = sorted(df["test_size"].unique() * 100)
    ax.set_xticks(test_percentages)
    ax.set(
        title="MAE by Test Size",
        xlabel="Test size (%)",
        ylabel="Mean Absolute Error (MAE)",
    )
    ax.legend(loc="upper left", bbox_to_anchor=(1.02, 1))
    ax.grid(axis="y", alpha=0.3)

    fig.savefig("output/mae_by_test_size.png", dpi=300, bbox_inches="tight")
    plt.show(block=True)


if __name__ == "__main__":
    main()