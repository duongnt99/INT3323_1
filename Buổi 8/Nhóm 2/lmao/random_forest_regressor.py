from sklearn.metrics import mean_absolute_error, mean_squared_error, r2_score
from sklearn.ensemble import RandomForestRegressor
from .utils import MethodAssessment
from .imputation import impute, impute_extenstion

import pandas as pd

def rfr_score(method_name, X_train, X_test, y_train, y_test) -> MethodAssessment:
    rfr = RandomForestRegressor(n_estimators=1000, criterion='squared_error', random_state=32, n_jobs=-1)
    
    print("Training...")
    rfr.fit(X_train, y_train)
    
    print("Predicting...")
    y_test_predict = rfr.predict(X_test)
    
    return MethodAssessment(method=method_name, 
                            mae=mean_absolute_error(y_test, y_test_predict), 
                            mse=mean_squared_error(y_test, y_test_predict), 
                            score=rfr.score(X_test, y_test), 
                            r2=r2_score(y_test, y_test_predict, force_finite=True))


def rfr_no_impute(X_train, X_test, y_train, y_test) -> MethodAssessment:
    return rfr_score("Random Forest Regressor (No Imputation)", X_train, X_test, y_train, y_test)


def rfr_impute(X_train, X_test, y_train, y_test) -> MethodAssessment:
    X_train, X_test = impute(X_train, X_test)
    return rfr_score("Random Forest Regressor (Imputation)", X_train, X_test, y_train, y_test)
    
    
def rfr_impute_extenstion(X_train, X_test, y_train, y_test) -> MethodAssessment:
    X_train, X_test = impute_extenstion(X_train, X_test)
    return rfr_score("Random Forest Regressor (Imputation Extension)", X_train, X_test, y_train, y_test)