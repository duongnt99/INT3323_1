from sklearn.metrics import mean_absolute_error, mean_squared_error
from xgboost import XGBRegressor
from .utils import MethodAssessment
from .imputation import impute, impute_extenstion

def xgboost_score(method_name, X_train, X_test, y_train, y_test) -> MethodAssessment:
    xgb = XGBRegressor(n_estimators=1000, learning_rate=0.5, n_jobs=-1, random_state=32)
    xgb.fit(X_train, y_train, eval_set=[(X_test, y_test)], verbose=False)
    
    y_test_predict = xgb.predict(X_test)
    
    return MethodAssessment(method=method_name,
                            test_size=None,
                            mae=mean_absolute_error(y_test, y_test_predict),
                            mse=mean_squared_error(y_test, y_test_predict),
                            score=xgb.score(X_test, y_test),
                            r2=xgb.score(X_test, y_test),
                            y_predict=y_test_predict,
                            y_test=y_test)
    
    
def xgboost_impute(X_train, X_test, y_train, y_test) -> MethodAssessment:
    X_train, X_test = impute(X_train, X_test)
    return xgboost_score("XGBoost Regressor (Imputation)", X_train, X_test, y_train, y_test)


def xgboost_impute_extension(X_train, X_test, y_train, y_test) -> MethodAssessment:
    X_train, X_test = impute_extenstion(X_train, X_test)
    return xgboost_score("XGBoost Regressor (Imputation Extension)", X_train, X_test, y_train, y_test)