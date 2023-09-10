mkdir root_folder
date
sleep 3
cd root_folder
touch root.txt
ls -l
chmod +xrw root.txt
ls -lt -r / > root.txt
cd ..
mkdir home_folder
cd home_folder/
touch home.txt
ls -lt -r ~ > home.txt
cd ..
