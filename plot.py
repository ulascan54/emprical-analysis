import pandas as pd
import matplotlib.pyplot as plt

# Read the data from the consolidated_results.txt file
data = []
with open('consolidated_results.txt', 'r') as file:
    for line in file:
        parts = line.split(': Size : ')
        algorithm = parts[0].strip()
        size_and_time = parts[1].split(', Average time (excluding first run): ')
        size = int(size_and_time[0].strip())
        time = float(size_and_time[1].split()[0].strip())
        data.append([algorithm, size, time])

# Create a DataFrame
df = pd.DataFrame(data, columns=['Algorithm', 'Size', 'Time'])

# Print the DataFrame to check the data
print(df)
df.to_csv("result_table.txt")

# Plotting the data
plt.figure(figsize=(12, 8))

for algorithm in df['Algorithm'].unique():
    subset = df[df['Algorithm'] == algorithm]
    plt.plot(subset['Size'], subset['Time'], marker='o', label=algorithm)

plt.xscale('log')
plt.yscale('log')
plt.xlabel('Input Size (log scale)')
plt.ylabel('Average Time (log scale)')
plt.title('Sorting Algorithm Performance')
plt.legend()
plt.grid(True, which="both", ls="--")
plt.show()
