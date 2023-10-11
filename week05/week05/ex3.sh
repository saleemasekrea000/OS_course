#!/bin/bash

# Compile the C program
gcc ex3.c -o ex3 -pthread

# Define the range and values of m to test
n=10000000
m_values=(1 2 4 10 100)

# Create or clear the result file
> ex3_res.txt

# Loop through each value of m
for m in "${m_values[@]}"; do
    # Measure execution time and append to result file
    echo "m=$m" >> ex3_res.txt
    { time ./ex3 $n $m; } 2>> ex3_res.txt
    echo "" >> ex3_res.txt
done

# Clean up the compiled program
rm ex3

