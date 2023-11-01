#!/bin/bash

pid=$(cat /tmp/ex1.pid)
# Calculate the address range for the heap memory
maps_file="/proc/$pid/maps"
start=$(grep -m 1 'heap' "$maps_file" | awk '{print $1}' | cut -d'-' -f1)
end=$(grep -m 1 'heap' "$maps_file" | awk '{print $1}' | cut -d'-' -f2)
start_decimal=$((16#$start))
password_hex=$(sudo xxd -s $start_decimal -l $((0x$end - 0x$start)) /proc/$pid/mem | grep -A 8 "pass:" | head -9)
password_hex=$(echo "$password_hex" | grep "pass:" | cut -d' ' -f2-)
password=$(echo -e "$password_hex" | xxd -r -p)
echo "The password is : $password"
echo "Address of the password is : 0x$start"
kill -SIGKILL $pid
