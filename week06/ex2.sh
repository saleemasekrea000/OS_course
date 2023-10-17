gcc scheduler.c -o scheduler
gcc worker.c -o worker

# Run the scheduler program in the background
./scheduler data.txt &


