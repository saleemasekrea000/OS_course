gcc ex1.c -o ex1
# Allocate space for a 50 MB image file named lofs.img using fallocate.
fallocate -l 50M lofs.img 

# Connect the image file to a loop device using losetup.
losetup -f lofs.img 

# Format the image file with an ext4 file system using mkfs.
mkfs -t ext4 lofs.img 

# Remove the existing lofsdisk directory if it's present.
sudo rm -r ./lofsdisk 

# Create a new directory named lofsdisk.
mkdir -p ./lofsdisk 

# Mount the loop device to the lofsdisk directory.
sudo mount -o loop lofs.img ./lofsdisk 

# Generate two text files (file11 and file2) with specific content in the lofsdisk directory.
echo "Saleem" > ./lofsdisk/file11
echo "Asekrea" > ./lofsdisk/file2

# The function get_libs() prints shared libraries required by the specified executable ($1).
# The awk command processes ldd output to extract shared library paths.
# The sed command replaces trailing commas with newline characters for formatting.
get_libs() {
    ldd $1 | awk 'BEGIN{ORS=" "}$1~/^\//{print $1}$3~/^\//{print $3}' | sed 's/,$/\n/'
}

# Specify the loop file system directory.
lofsdisk="./lofsdisk"

# Commands for which shared libraries are obtained.
commands=("bash" "cat" "echo" "ls")

# Create bin, lib, and lib64 directories in the loop file system.
mkdir -p ./lofsdisk/bin
mkdir -p ./lofsdisk/lib
mkdir -p ./lofsdisk/lib64

# Loop through commands, copying their shared libraries to the loop file system.
for cmd in "${commands[@]}"; do
    libs=$(get_libs "$(which $cmd)")
    echo "$(which $cmd)"
    cp "$(which $cmd)" ./lofsdisk/bin

    # Copy shared libraries to the loop file system.
    for lib in $libs; do
        if [ -f "$lib" ]; then
            cp "$lib" ./lofsdisk/lib
            cp "$lib" ./lofsdisk/lib64
            echo "Copied $lib to $lofsdisk"
        else
            echo "Error: Shared library $lib not found."
        fi
    done
done

# Copy an executable file named ex1 to the loop file system and perform additional file operations.
cp ./ex1 ./lofsdisk/bin/ex1
touch ./lofsdisk/bin/ex1.txt
chmod +w ./lofsdisk/bin/ex1.txt

# Use chroot to change the root directory to lofsdisk and execute a command (bin/ex1 > bin/ex1.txt) within that environment.
sudo chroot ./lofsdisk /bin/bash -c "bin/ex1 > bin/ex1.txt"
rm ex1.txt
touch ex1.txt

# Perform cleanup operations and copy the results to the host system.
cp ./lofsdisk/bin/ex1.txt ./ex1.txt
echo " " >> ex1.txt
echo " " >> ex1.txt
echo " " >> ex1.txt

# Add the results of ex1 when the root is not changed.
./ex1 >> ex1.txt
echo "The variation in outputs is due to changing the root directory for ex1 during the first execution using chroot." >> ex1.txt

