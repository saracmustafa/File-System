# File-System

:computer: &nbsp;**Spring 2017 COMP 304 Operating Systems Course Project, Koç University**

The final version of this code has been developed by **Mustafa SARAÇ** and **Musa ÇIBIK** as a project of Operating Systems (COMP 304) course. **Koç University's code of ethics can be applied to this code and liability can not be accepted for any negative situation. Therefore, be careful when you get content from here.**

### Description: 

In this project, we implemented a simple file system called FS304 and it has basic three commands:
- **compare:** Checks whether the given two files' contents are same. Based on the comparison, if there is a difference between these files, this command states the value of the difference. Otherwise, it states that these files are same.
- **rename:** Based on the given old name, finds the file or the directory and changes the name of this file or directory with the given new name. If there is no file or directory with given old name or if there already exists a file or a directory with given new name, it prints a warning.
- **copy:** Based on the given original name, finds the file and copies the content of that file to a new file. If there is not a file with given original name or if there already exists a file or a directory with given new name, it prints warning.

In addition to these, our main program **mounts the disk file** and perform the given user operations such as **compare, rename, copy, ls, cd, rd and md**, using this disk for the file storage.

Provided disk file called **fs304.disk** can simultaneously work with more than one executable files because we do not wait to write changes to the disk file until the termination of the program. For instance, when we copy a file in the first executable file, our disk file will be updated which means that we can access to the copied file from the second executable file.

As a reminder, we made some changes in **'fs304.h'** file. In the given code, when we run ls command, we get directory name in red. We also gave color to the files while printing the names of the files.

#### For more detailed questions, you can contact me at this email address: msarac13@ku.edu.tr &nbsp;&nbsp;:email:
