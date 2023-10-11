#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <number of subscribers>"
    exit 1
fi

n=$1
gcc publisher.c -o publisher
gcc subscriber.c -o subscriber
# Open subscribers in separate shell windows
for (( i=1; i<=n; i++ ))
do
    gnome-terminal -- bash -c "./subscriber $i; exec bash"
done

# Open publisher
sudo gnome-terminal -- bash -c "./publisher $n; exec bash"
