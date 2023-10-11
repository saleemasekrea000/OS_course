#!/bin/bash

# Compilation
gcc -o ex4 ex4.c -lpthread

# Define values of n and an array of m values
n=10000000
m_values=(1 2 4 10 100)

# Output file for results
output_file="ex4_res.txt"

# Clear existing results file
> $output_file

# Loop over different values of m
for m in "${m_values[@]}"
do
    # Measure execution time for each value of m
    echo "Executing with n=$n and m=$m..."
    execution_time=$( /usr/bin/time -f "%e" ./ex4 $n $m 2>&1 )
    
    # Append results to the output file
    echo "n=$n m=$m Execution Time: $execution_time seconds" >> $output_file
done

# Cleanup the compiled program
rm ex4

echo "Execution completed."
