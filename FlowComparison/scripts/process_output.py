from math import sqrt
import pandas as pd
import csv


file_base_name = "flow_comparison"
infile = f"{file_base_name}.out"
output_data = []

with open(infile, "r") as file:
    data = {}
    for line in file.readlines():
        if "Algorithm:" in line:
            if data:
                output_data.append(data)
            data = {"algorithm": line.split(": ")[1].strip(), "operations": 0, "augmentations": 0, "max_augpath_size": 0, "max_augpath_set": 0}
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
        elif "Maximum size augmenting path:" in line:
            data["max_augpath_size"] += int(line.split(": ")[1].strip().split()[0])
        elif "Maximum number of augmenting paths:" in line:
            data["max_augpath_set"] += int(line.split(": ")[1].strip().split()[0])
    if data:
        output_data.append(data)

fields = ["algorithm", "density", "vertices", "edges", "augmentations", "matchings", "operations", "elapsed_time", "max_augpath_size", "max_augpath_set"]
#fields = ["algorithm", "density", "vertices", "edges", "augmentations", "matchings", "operations", "elapsed_time"]
outfile = f"{file_base_name}.csv"
with open(outfile, "w") as csv_file:
    writer = csv.DictWriter(csv_file, fieldnames=fields)
    writer.writeheader()
    writer.writerows(output_data)

# Load the CSV file into a DataFrame
df = pd.read_csv(outfile)


# Group by density and algorithm and calculate the averages
grouped_df = df.groupby(['density', 'algorithm']).agg({
    'vertices': 'mean',
    'edges': 'mean',
    'elapsed_time': 'mean',
    'matchings': 'mean',
    'operations': 'mean',
    'augmentations': 'mean'
}).reset_index()

# Round the average vertices to integers and limit float precision
grouped_df['vertices'] = grouped_df['vertices'].round().astype(int)
grouped_df['edges'] = grouped_df['edges'].round().astype(int)
grouped_df['elapsed_time'] = grouped_df['elapsed_time'].round(6)
grouped_df['matchings'] = grouped_df['matchings'].round(2)
grouped_df['operations'] = grouped_df['operations'].round(2)
grouped_df['augmentations'] = grouped_df['augmentations'].round(2)



# Save the grouped and averaged results to a new CSV file
grouped_df.to_csv(f"{file_base_name}_averaged.csv", index=False)

print(grouped_df)
