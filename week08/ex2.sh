gcc pager.c -o pager
./pager 4 2 &
pager_pid=$!
gcc mmu.c -o mmu
./mmu 4 R0 R1 W1 R0 R2 W2 R0 R3 W2 "$pager_pid"
rm pager
rm mmu
