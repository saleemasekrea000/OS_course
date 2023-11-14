#!/bin/bash
gcc monitor.c -o monitor
gnome-terminal -- sh -c "./monitor ."
sleep 1
gcc ex1.c -o ex1
./ex1 .
