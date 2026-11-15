class MethodAssessment:
    def __init__(self, method: str, mae: float, mse: float, score: float, r2: float):
        self.method = method
        self.mae = mae
        self.mse = mse
        self.score = score
        self.r2 = r2
        

    def print_assessment(self):
        print(f"---- Method: {self.method} ----")
        print(f"MAE: {self.mae}")
        print(f"MSE: {self.mse}")
        print(f"Score: {self.score}")
        print(f"R2: {self.r2}")
        print("-----------------------------")