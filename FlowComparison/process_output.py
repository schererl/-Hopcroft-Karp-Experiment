from math import sqrt
import pandas as pd
import csv

infile = "flowcomparison.out"
output_data = []

with open(infile, "r") as file:
    data = {}
    for line in file.readlines():
        if "Algorithm:" in line:
            if data:
                output_data.append(data)
            data = {"algorithm": line.split(": ")[1].strip(), "operations": 0, "augmentations": 0}
        elif "Density:" in line:
            data["density"] = float(line.split(": ")[1].strip())
        elif "Total Vertices:" in line:
            data["vertices"] = int(line.split(": ")[1].strip())
        elif "Total Edges:" in line:
            data["edges"] = float(line.split(": ")[1].strip())
        elif "Total matchings:" in line:
            data["matchings"] = int(line.split(": ")[1].strip().split()[0])
        elif "Total augmentations:" in line:
            data["augmentations"] = int(line.split(": ")[1].strip().split()[0])
        elif "Elapsed time:" in line:
            data["elapsed_time"] = float(line.split(": ")[1].strip().split()[0])
        elif "BFS operations:" in line:
            data["operations"] += int(line.split(": ")[1].strip().split()[0])
        elif "DFS operations:" in line:
            data["operations"] += int(line.split(": ")[1].strip().split()[0])

    if data:
        output_data.append(data)

fields = ["algorithm", "density", "vertices", "edges", "augmentations", "matchings", "operations", "elapsed_time"]
outfile = "flowcomparison.csv"
with open(outfile, "w") as csv_file:
    writer = csv.DictWriter(csv_file, fieldnames=fields)
    writer.writeheader()
    writer.writerows(output_data)

# Load the CSV file into a DataFrame
df = pd.read_csv(outfile)

# Calculate lower bounds
df['bound_augmentations'] = df.apply(
    lambda row: 0.0 if row['algorithm'] == 'Edmonds-Karp' else (sqrt(row['vertices'])), axis=1
)
df['bound_operations'] = df.apply(
    lambda row: 0.0 if row['algorithm'] == 'Edmonds-Karp' else (sqrt(row['vertices'])* (row['vertices']+ row['edges'])), axis=1
)

# Group by density and algorithm and calculate the averages
grouped_df = df.groupby(['algorithm', 'density']).agg({
    'vertices': 'mean',
    'edges': 'mean',
    'elapsed_time': 'mean',
    'matchings': 'mean',
    'operations': 'mean',
    'augmentations': 'mean',
    'lower_bound_augmentations': 'mean',
    'lower_bound_operations': 'mean'
}).reset_index()

# Round the average vertices to integers and limit float precision
grouped_df['vertices'] = grouped_df['vertices'].round().astype(int)
grouped_df['edges'] = grouped_df['edges'].round().astype(int)
grouped_df['elapsed_time'] = grouped_df['elapsed_time'].round(6)
grouped_df['matchings'] = grouped_df['matchings'].round(2)
grouped_df['operations'] = grouped_df['operations'].round(2)
grouped_df['augmentations'] = grouped_df['augmentations'].round(2)
grouped_df['lower_bound_augmentations'] = grouped_df['lower_bound_augmentations'].round(2)
grouped_df['lower_bound_operations'] = grouped_df['lower_bound_operations'].round(2)

# Save the grouped and averaged results to a new CSV file
grouped_df.to_csv("flowcomparison_averaged.csv", index=False)

print(grouped_df)
