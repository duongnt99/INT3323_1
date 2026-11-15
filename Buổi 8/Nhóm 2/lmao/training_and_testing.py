from sklearn.model_selection import train_test_split

from lmao.utils import MethodAssessment
from .random_forest_regressor_pipeline import rfr_pipeline_impute, rfr_pipeline_impute_extension
from .random_forest_regressor import rfr_no_impute, rfr_impute, rfr_impute_extenstion
from .xg_boost_regressor import xgboost_impute, xgboost_impute_extension
import json
import pandas as pd

TEST_SIZES = [0.05, 0.1, 0.2, 0.3, 0.5, 0.7, 0.8]
METHODS = [
    rfr_no_impute,
    rfr_impute,
    rfr_impute_extenstion,
    rfr_pipeline_impute,
    rfr_pipeline_impute_extension,
    xgboost_impute,
    xgboost_impute_extension
]

def main():
    df = pd.read_csv("./content/data_comb-2.csv")
    X = df.drop(columns=['sm'])
    y = df['sm']
    
    results_raw: list[MethodAssessment] = []

    for test_size in TEST_SIZES:
        print(f"Benchmarking with {1 - test_size:%} train and {test_size:%} test...")
        X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=test_size, random_state=32)
        
        for method in METHODS:
            print(f"Running {method.__name__}...")
            assessment = method(X_train, X_test, y_train, y_test)
            assessment.test_size = test_size
            results_raw.append(assessment)
        
    results = [{
        "method": assessment.method,
        "test_size": assessment.test_size,
        "mae": assessment.mae,
        "mse": assessment.mse,
        "score": assessment.score,
        "r2": assessment.r2
    } for assessment in results_raw]
    pd.DataFrame(results).to_csv("./output/benchmark_results.csv", index=False)
    
    best = min(results_raw, key=lambda entry: entry.mae, default=None)
    if best is not None:
        print(f"Best method: {best.method} with MAE: {best.mae:.4f} at test size: {best.test_size:%}")
        with open("./output/best_method.json", "w", encoding="utf-8") as file:
            json.dump(best.to_dict(), file, ensure_ascii=False, indent=4)
    
    
if __name__ == "__main__":
    main()