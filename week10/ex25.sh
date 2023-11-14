#!/bin/bash

# Step 1: Create an empty file named ex5.txt
touch ex5.txt

# Step 2: Revoke write permission for all users
chmod a-w ex5.txt

# Step 3: Allow owner and others (not in the group) to read, write, and execute
chmod ugo+rwx ex5.txt

# Step 4: Synchronze group permissions with user permisions
chmod g=u ex5.txt

# Optional: Display the final permissions
ls -l ex5.txt

