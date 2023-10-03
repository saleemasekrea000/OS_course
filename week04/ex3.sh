#!/bin/bash
gcc ex3.c -o ex3
# Run the program with n=3
./ex3 3 &
sleep 10
# Run pstree command several times
echo "pstree output for n=3:"
pstree -p $!

# Run the program with n=5
./ex3 5 &
# Wait for the program to start
sleep 20

echo "pstree output for n=5:"
pstree -p $!
rm ex3

