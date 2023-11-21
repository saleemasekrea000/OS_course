#!/bin/bash
gcc ex2.c -o ex1
./ex2 disk0
gcc ex2.c -o ex2
./ex2 disk0 < input.txt
rm create_fs
rm ex2
