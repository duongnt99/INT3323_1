class MethodAssessment:
    def __init__(self, method: str, test_size: float, mae: float, mse: float, score: float, r2: float, y_predict, y_test):
        self.method = method
        self.test_size = test_size
        self.mae = mae
        self.mse = mse
        self.score = score
        self.r2 = r2
        self.y_predict = y_predict
        self.y_test = y_test
        
        
    def to_dict(self) -> dict:
        return {
            "method": self.method,
            "test_size": self.test_size,
            "mae": float(self.mae),
            "mse": float(self.mse),
            "score": float(self.score),
            "r2": float(self.r2),
            "y_predict": self.y_predict.tolist(),
            "y_test": self.y_test.tolist()
        }