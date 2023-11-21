bash gen.sh 10 ex1.txt
ln ex1.txt ex11.txt
ln ex1.txt ex12.txt
diff ex1.txt ex11.txt
diff ex1.txt ex12.txt
ls -i ex1.txt ex11.txt ex12.txt > output.txt
du -h ex1.txt
mv ex13.txt /tmp/
find . -inum $(stat -c "%i" ex1.txt)
find / -inum $(stat -c "%i" ex1.txt)
stat -c "%h" ex1.txt #1
find ./ex1.txt -inum $(stat -c "%i" ex1.txt) -exec rm {} \;

#Content Equality:
#The contents of ex1.txt, ex11.txt, and ex12.txt are identical because hard links were created using the ln command. Hard links #establish multiple directory entries pointing to the same inode on the disk. Consequently, modifications made to one file are #reflected in the others since they share the same underlying data. Commands such as diff or cat reveal no distinctions among the #files.

#Inode Numbers:
#The fact that ex1.txt, ex11.txt, and ex12.txt share identical inode numbers indicates that they are hard links with a common #underlying dataset. If these files had different inode numbers, it would suggest that they are separate files with distinct data.

#Search Paths:
#When searching in the current path (.), the search is confined to the current working directory and its subdirectories. This #focused approach facilitates quicker searches within a specific area.

#On the other hand, searching in the root path (/) encompasses the entire filesystem, starting from the root directory. Although #this broader search may take longer, it proves useful when searching across the entire system for files or links.