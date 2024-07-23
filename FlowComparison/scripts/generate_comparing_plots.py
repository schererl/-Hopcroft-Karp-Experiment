import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV data
data = pd.read_csv('flow_comparison.csv')

# Filter data for each algorithm
hk_data = data[data['algorithm'] == 'Hopcroft-Karp']
ek_data = data[data['algorithm'] == 'Edmonds-Karp']

# Ensure the data is sorted by density to align the pairs correctly
hk_data = hk_data.sort_values(by='density').reset_index(drop=True)
ek_data = ek_data.sort_values(by='density').reset_index(drop=True)

# Create a figure with two subplots
plt.figure(figsize=(15, 7))

# Plot elapsed time comparison
plt.subplot(1, 2, 1)
plt.scatter(hk_data['elapsed_time'], ek_data['elapsed_time'], color='blue')
plt.xlabel('Hopcroft-Karp Elapsed Time')
plt.ylabel('Edmonds-Karp Elapsed Time')
plt.title('Elapsed Time Comparison (Log Scale)')
plt.xscale('log')
plt.yscale('log')
plt.grid(False)

# Plot line y=x for reference
max_time = max(hk_data['elapsed_time'].max(), ek_data['elapsed_time'].max())
plt.plot([1e-6, max_time], [1e-6, max_time], linestyle='--', color='red')

# Plot operations comparison
plt.subplot(1, 2, 2)
plt.scatter(hk_data['operations'], ek_data['operations'], color='green')
plt.xlabel('Hopcroft-Karp Operations')
plt.ylabel('Edmonds-Karp Operations')
plt.title('Operations Comparison (Log Scale)')
plt.xscale('log')
plt.yscale('log')
plt.grid(False)

# Plot line y=x for reference
max_operations = max(hk_data['operations'].max(), ek_data['operations'].max())
plt.plot([1, max_operations], [1, max_operations], linestyle='--', color='red')

# Adjust layout and save the figure
plt.tight_layout()
plt.savefig('./comparison_plots.png')
plt.show()


# Calculate bounds and ratios
n = data['vertices'].iloc[0]  # Number of vertices is constant across the dataset
m = data['edges']  # Number of edges varies
sqrt_n = np.sqrt(n)

hk_data['bound'] = sqrt_n * (n + hk_data['edges'])
ek_data['bound'] = n * ek_data['edges']
hk_data['ratio'] = hk_data['operations'] / hk_data['bound']
ek_data['ratio'] = ek_data['operations'] / ek_data['bound']
# Create a figure for bound-complexity ratios
plt.figure(figsize=(7, 7))

# Plot bound-complexity ratio comparison
plt.scatter(hk_data['ratio'], ek_data['ratio'], color='green')
plt.xlabel('Hopcroft-Karp Complexity-Ratio')
plt.ylabel('Edmonds-Karp Complexity-Ratio')
plt.title('Complexity-Ratio Comparison (Log Scale)')
plt.grid(False)

# Plot line y=x for reference
max_ratio = max(hk_data['ratio'].max(), ek_data['ratio'].max())
plt.plot([1e-6, max_ratio], [1e-6, max_ratio], linestyle='--', color='red')

# Adjust layout and save the figure
plt.tight_layout()
plt.savefig('./bound_complexity_comparison.png')
plt.show()