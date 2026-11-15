from sklearn.impute import SimpleImputer
import pandas as pd
    
def impute_extenstion(X_train, X_test):
    X_train_plus = X_train.copy()
    X_test_plus = X_test.copy()
    
    columns_with_missing = [column for column in X_train.columns if X_train[column].isnull().any()]
    
    for column in columns_with_missing:
        X_train_plus[column + "_was_missing"] = X_train_plus[column].isnull()
        X_test_plus[column + "_was_missing"] = X_test_plus[column].isnull()
        
    return impute(X_train_plus, X_test_plus)    
    
    
def impute(X_train, X_test):
    imputer = SimpleImputer()
    imputed_X_train = pd.DataFrame(imputer.fit_transform(X_train))
    imputed_X_test = pd.DataFrame(imputer.transform(X_test))
    imputed_X_train.columns = X_train.columns
    imputed_X_test.columns = X_test.columns
    return imputed_X_train, imputed_X_test