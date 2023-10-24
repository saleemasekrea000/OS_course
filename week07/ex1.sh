#!/bin/bash
# Compile agent and controller if not already compiled
gcc allocator.c -o allocator
./allocator
rm allocator
