import pandas as pd

print("Reading dataset 1...")
data1 = pd.read_csv("content/user1_data-1.csv")
print(f"Dataset 1 read with {data1.columns} ({data1.shape})")

print("Reading dataset 2...")
data2 = pd.read_csv("content/user2_data-1.csv")
print(f"Dataset 2 read with {data2.columns} ({data2.shape})")

# print("Generating data1 heatmap...")
# sns.heatmap(data2.corr(numeric_only=True), annot=True, cmap='viridis')
# plt.title("Heatmap of co-relation between variables")
# plt.show()

# Pre-processing dataset 1
def preprocess_data(data):
    data['ttime'] = pd.to_datetime(data.ttime, format='%Y-%m-%d %H:%M:%S')
    data.set_index('ttime', inplace=True)
    data['day'] = data.index.day
    data['month'] = data.index.month
    data['year'] = data.index.year
    data['hour'] = data.index.hour
    data['minute'] = (data.index.minute // 5) + 1
    data.reset_index(inplace=True)
    data = data.drop(['ttime'], axis=1)

    return data
        
def combine_datasets(data1, data2):
    combined_data = data1.merge(data2, on=['day', 'month', 'year', 'hour', 'minute'], how='outer')
    
    averaging_fields = ['pm1', 'pm2', 'pm3', 'am', 'sm', 'lum']
    drop_fields = []
    for field in averaging_fields:
        x = field + '_x'
        y = field + '_y'
        combined_data[field] = combined_data[[x, y]].mean(axis=1)
        drop_fields.append(x)
        drop_fields.append(y)
    
    combined_data = combined_data.drop(columns=drop_fields)

    return combined_data

combined = combine_datasets(preprocess_data(data1), preprocess_data(data2))
# print(combined)
# print(combined.isnull().sum())

# for i in combined.columns.drop('sm'):
#     y_point = combined['sm']
#     plt.scatter(combined[i], y_point)
#     plt.title(i)
#     plt.show()

print(combined.describe())