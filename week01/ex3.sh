printDateandsleep(){
date 
sleep 3
}
mkdir root_folder
printDateandsleep
mkdir home_folder
printDateandsleep
touch root_folder/root.txt
printDateandsleep
touch home_folder/home.txt
printDateandsleep
ls -l
chmod +xrw root.txt
ls -lt -r / > root.txt
ls -lt -r ~ > home.txt
cat root_folder/root.txt
cat home_folder/home.txt
ls  root_folder
ls home_folder
