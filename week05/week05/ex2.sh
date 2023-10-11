#!/bin/bash

# Compile the program
gcc -o ex2 ex2.c -pthread

# Run the program with n=5
./ex2 5

# Clean up the compiled program
rm ex2

echo "Execution completed."
