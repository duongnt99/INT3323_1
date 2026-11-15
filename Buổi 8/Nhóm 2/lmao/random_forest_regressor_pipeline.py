from sklearn.ensemble import RandomForestRegressor
from sklearn.impute import SimpleImputer
from sklearn.pipeline import Pipeline
from sklearn.compose import ColumnTransformer
from sklearn.metrics import mean_absolute_error, mean_squared_error, r2_score
from .utils import MethodAssessment
from .imputation import impute, impute_extenstion
    
def rfr_pipeline_score(method_name, X_train, X_test, y_train, y_test) -> MethodAssessment:
    pipeline = Pipeline(steps=[
        ('preprocessor', SimpleImputer(strategy='mean')),
        ('model', RandomForestRegressor(n_estimators=1000, random_state=32))
    ])
    
    print("Training...")
    pipeline.fit(X_train, y_train)
    
    print("Predicting...")
    y_test_predict = pipeline.predict(X_test)
    
    return MethodAssessment(method=method_name, 
                            mae=mean_absolute_error(y_test, y_test_predict), 
                            mse=mean_squared_error(y_test, y_test_predict), 
                            score=pipeline.score(X_test, y_test), 
                            r2=r2_score(y_test, y_test_predict, force_finite=True))


def rfr_pipeline_impute(X_train, X_test, y_train, y_test) -> MethodAssessment:
    X_train, X_test = impute_extenstion(X_train, X_test)
    return rfr_pipeline_score("Random Forest Regressor (Imputation)", X_train, X_test, y_train, y_test)


def rfr_pipeline_impute_extension(X_train, X_test, y_train, y_test) -> MethodAssessment:
    X_train, X_test = impute_extenstion(X_train, X_test)
    return rfr_pipeline_score("Random Forest Regressor (Imputation Extension)", X_train, X_test, y_train, y_test)