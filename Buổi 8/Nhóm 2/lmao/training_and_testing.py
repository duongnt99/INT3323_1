from sklearn.model_selection import train_test_split
from .random_forest_regressor_pipeline import rfr_pipeline_impute, rfr_pipeline_impute_extension
from .random_forest_regressor import rfr_no_impute, rfr_impute, rfr_impute_extenstion
from .xg_boost_regressor import xgboost_impute, xgboost_impute_extension

import pandas as pd

TEST_SIZES = [0.05, 0.1, 0.2, 0.3, 0.5]
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
    
    results = []

    for test_size in TEST_SIZES:
        print(f"Benchmarking with {1 - test_size:%} train and {test_size:%} test...")
        X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=test_size, random_state=32)
        
        for method in METHODS:
            print(f"Running {method.__name__}...")
            assessment = method(X_train, X_test, y_train, y_test)
            results.append({
                "method": assessment.method,
                "test_size": test_size,
                "n_train": len(y_train),
                "n_test": len(y_test),
                "mae": assessment.mae,
                "mse": assessment.mse,
                "score": assessment.score,
                "r2": assessment.r2
            })
        
    pd.DataFrame(results).to_csv("./output/benchmark_results.csv", index=False)
    
    
if __name__ == "__main__":
    main()