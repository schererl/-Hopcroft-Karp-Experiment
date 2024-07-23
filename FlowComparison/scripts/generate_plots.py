import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Load the CSV data
data = pd.read_csv('hopcroft_karp_only.csv')

# Calculate the bounds
n = data['vertices'].iloc[0]  # Number of vertices is constant across the dataset
m = data['edges']  # Number of edges varies
sqrt_n = np.sqrt(n)

data['iterations_bound'] = sqrt_n
data['operations_bound'] = n + m
data['time_complexity_bound'] = sqrt_n * (n + m)

# Calculate the ratios
data['a'] = data['augmentations'] / data['iterations_bound']
data['o'] = (data['operations'] / data['augmentations']) / data['operations_bound']
data['T'] = data['operations'] / data['time_complexity_bound']

# Plot the ratios over the graph's density
plt.figure(figsize=(15, 5))

# Plot for iterations bound ratio (a)
plt.subplot(1, 3, 1)
plt.scatter(data['density'], data['a'], color='b')
plt.xlabel('Density')
plt.ylabel('Iterations Bound Ratio (a)')
plt.title('Iterations Bound Ratio vs Density')
plt.grid(True)

# Plot for operations bound ratio (o)
plt.subplot(1, 3, 2)
plt.scatter(data['density'], data['o'], color='g')
plt.xlabel('Density')
plt.ylabel('Operations Bound Ratio (o)')
plt.title('Operations Bound Ratio vs Density')
plt.grid(True)

# Plot for time complexity bound ratio (T)
plt.subplot(1, 3, 3)
plt.scatter(data['density'], data['T'], color='r')
plt.xlabel('Density')
plt.ylabel('Time Complexity Bound Ratio (T)')
plt.title('Time Complexity Bound Ratio vs Density')
plt.grid(True)

# Show the plots
plt.tight_layout()
plt.savefig('./ratios_vs_density.png')
plt.show()

# Plot the augmenting path set and size against density
plt.figure(figsize=(10, 5))

# Plot for maximum augmenting path set
plt.subplot(1, 2, 1)
plt.scatter(data['density'], data['max_augpath_set']/data['matchings'], color='c')
plt.xlabel('Density')
plt.ylabel('Matching Difficulty')
plt.title('Matching Difficulty vs Density')
plt.grid(True)

# Plot for maximum augmenting path size
plt.subplot(1, 2, 2)
plt.scatter(data['density'], data['max_augpath_size'], color='m')
plt.xlabel('Density')
plt.ylabel('Maximum Augmenting Path Size')
plt.title('Maximum Augmenting Path Size vs Density')
plt.grid(True)

# Show the plots
plt.tight_layout()
plt.savefig('./augmenting_paths_vs_density.png')
plt.show()
