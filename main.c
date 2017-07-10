/**
 *
 * Simple File System Implementation
 *
 * The final version of this code has been developed by Mustafa SARAÇ and Musa ÇIBIK as a project
 * of Operating System (COMP 304) course. Koç University's code of ethics can be applied to this
 * code and liability can not be accepted for any negative situation. Therefore, be careful when
 * you get content from here.
 *
 * Description: In this project, we basicaly implemented three commands (compare, rename, copy) 
 * that operates on our file system called FS304. Therefore, with this project we had the 
 * opportunity to get a better understanding of how file systems work.
 *
 * You can also contact me at this email address: msarac13@ku.edu.tr
 * 
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include "fs304.h"

#define MAX_LINE 80 /* 80 chars per line */

int parseCommand(char inputBuffer[], char *args[]); 

int main(void)
{
  char inputBuffer[MAX_LINE]; 	        /* buffer to hold the command entered */
  char *args[MAX_LINE/2 + 1];	        /* command line (of 80) has max of 40 arguments */
  int shouldrun = 1;
  int i, upper;
	
  while (shouldrun) {           /* Continue to get command line input till 'shouldrun' is 0 */
		
    shouldrun = parseCommand(inputBuffer,args);       /* get next command line input */
		
    if (strncmp(inputBuffer, "exit", 4) == 0)	/* if user gives 'exit' command, program will terminate */
      shouldrun = 0;  

    if (shouldrun) {

	    if(args[0] != NULL) { 		/* If user gives an empty string, skip it and get another input */

	        int valid_command = 0;

			/* Before the execution of our file system commands, we need to use this function. 
	 		* It basicly reads the fs304.disk into data structures in memory.  */
	        mountFS304();

	        /* process of 'cd' command */
	        if(strcmp(args[0], "cd") == 0 && args[1] != NULL && args[2] == NULL){
	          valid_command = 1;
	          cd(args[1]);
	        }

			/* process of 'ls' command */
	        if(strcmp(args[0], "ls") == 0 && args[1] == NULL){
	          valid_command = 1;
	          ls();
	        }

	        /* process of 'rd' command */
	        if(strcmp(args[0], "rd") == 0 && args[1] == NULL){
	          valid_command = 1;
	          rd();
	        }

	        /* process of 'stats' command */
	        if(strcmp(args[0], "stats") == 0 && args[1] == NULL){
	          valid_command = 1;
	          stats();
	        }

	        /* process of 'md' command */
	        if(strcmp(args[0], "md") == 0 && args[1] != NULL && args[2] == NULL){
	          valid_command = 1;
	          md(args[1]);
	        }

	        /* process of 'compare' command */
	        if(strcmp(args[0], "compare") == 0 && args[1] != NULL && args[2] != NULL && args[3] == NULL){
	          valid_command = 1;
	          compare(args[1], args[2]);
	        }

	        /* process of 'rename' command */
	        if(strcmp(args[0], "rename") == 0 && args[1] != NULL && args[2] != NULL && args[3] == NULL){
	          valid_command = 1;
	          rename_file(args[1], args[2]);
	        }

			    /* process of 'copy' command */
	        if(strcmp(args[0], "copy") == 0 && args[1] != NULL && args[2] != NULL && args[3] == NULL){
	          valid_command = 1;
	          copy(args[1] ,args[2]);
	        }

	       
	       if(!valid_command) 		/* valid command warning */
	          printf("Please give a valid command! \n");
	       

	    } /* end of if(args[0] != NULL) */
    
    } /* end of if(shouldrun) */
  
  } /* end of while(shouldrun) */


  return 0;
}


/* FUNCTION: compare
 * 
 * OBJECTIVE: 
 * 		Checks whether the given two files' contents are same.
 * 		Based on the comparison, if there is a difference between
 * 		these files, this command states the value of the difference.
 * 		Otherwise, it states that these files are same.
 * 		
 * PARAMETERS: 
 * 		filename1: the name of the first file to be compared
 *   	filename2: the name of the first file second file to be compared 	
 */
void compare(char* filename1, char* filename2){

  char file1[1024] , file2[1024];
  int blocks_file1[3], blocks_file2[3];
  int e_inode_file1=0, e_inode_file2=0;

  /*Call find_or_rename fucntion
	The parameters of the find_or_rename function are as follows: 
	the name of the file (filename1); search the name only in files not directories(1); 
	do not rename the file(0); since we are not renaming our file, we do not have to provide a new name(NULL) 
	It returns the index of the given file; returns 0 if the file can not be found. */ 
  e_inode_file1=find_or_rename(filename1, 1, 0 ,NULL);
  e_inode_file2=find_or_rename(filename2, 1, 0 , NULL);

  // Check whether the file is found or not.
  if(e_inode_file1 == 0 && e_inode_file2 == 0 )
    printf("Could not find the files %s and %s\n", filename1, filename2 );
  else if(e_inode_file1 == 0)
    printf("Could not find a file called %s\n", filename1 );
  else if(e_inode_file2 == 0)
    printf("Could not find a file called %s\n", filename2 );
  else 
  {
	  // Get the blocks that our file's data is stored.
	  blocks_file1[0] = stoi(_inode_table[e_inode_file1].XX,2);
	  blocks_file1[1] = stoi(_inode_table[e_inode_file1].YY,2);
	  blocks_file1[2] = stoi(_inode_table[e_inode_file1].ZZ,2);

	  blocks_file2[0] = stoi(_inode_table[e_inode_file2].XX,2);
	  blocks_file2[1] = stoi(_inode_table[e_inode_file2].YY,2);
	  blocks_file2[2] = stoi(_inode_table[e_inode_file2].ZZ,2);

	  int difference=0;

	  for (int i=0; i<3; i++) {

		// Read the three blocks of the each file.
		// For each file in the block, compare the values.
		readFS304(blocks_file1[i],(char *)file1);
		readFS304(blocks_file2[i],(char *)file2);

		for(int i=0; i<1024; i++)
		  if(file1[i] != file2[i])
		    difference++;

	  }

	// Print the result.
	  if(difference == 0)
	    printf("There is no difference between the files: %s and %s \n", filename1, filename2 );
	  if(difference != 0)
	    printf("There %s %d difference%s between the files: %s and %s\n", (difference==1?"is":"are") , difference, (difference==1?"":"s"), filename1, filename2 );
	  
  }

}


/* FUNCTION: rename_file
 * 
 * OBJECTIVE: 
 * 		Based on the given old name, finds the file or the directory and 
 * 		changes the name of this file or directory with the given 
 * 		new name. If there is no file or directory with given old
 * 		name or if there already exists a file or a directory with given 
 * 		new name, it prints a warning.
 * 		
 * PARAMETERS: 
 * 		oldname: the name of the file or the directory to be renamed
 *   	newname: new name of that file or directory  	
 */
void rename_file(char* oldname, char* newname){
  
   /*Call find_or_rename fucntion
	The parameters of the find_or_rename function are as follows: 
	the name of the file (newname); search the name in files and directories(0); 
	do not rename the file(0); since we are not renaming our file, we do not have to provide a new name(NULL) 
	It returns the index of the given file; returns 0 if the file can not be found. */
  int inode_new = find_or_rename(newname, 0, 0, NULL);
  
    // Check whether a file or directory named with the given newname is found or not.
  if(inode_new != 0){
     printf("Could not rename the file %s, since there exists a file or directory called %s\n", oldname , newname);
     return;
  }

    /*Call find_or_rename fucntion
	The parameters of the find_or_rename function are as follows: 
	the name of the file (oldname); search the name in files and directories(0); 
	rename the file(1); since we are renaming our file, we have to provide a new name(newname) 
	It returns the index of the given file; returns 0 if the file can not be found. */
  int inode_old = find_or_rename(oldname, 0, 1, newname);

   // Check whether the file or the directory we want to rename is found or not.
  if(inode_old == 0)
     printf("Could not find a file called %s\n", oldname );
  
}

/* FUNCTION: copy
 * 
 * OBJECTIVE: 
 * 		Based on the given original name, finds the file and 
 * 		copies the content of that file to a new file.
 *   	If there is not a file with given original
 * 		name or if there already exists a file or a directory with given 
 * 		new name, it prints warning.
 * 		
 * PARAMETERS: 
 * 		original: the name of the file to be copied
 *   	copied: teh name of the copy. This file will have the same content with the original one if the operation is successful 	
 */
void copy(char* original, char* copied){
  
  /*Call find_or_rename fucntion
	The parameters of the find_or_rename function are as follows: 
	the name of the file (original); search the name only in files not directories(1); 
	do not rename the file(0); since we are not renaming our file, we do not have to provide a new name(NULL) 
	It returns the index of the given file; returns 0 if the file can not be found. */ 
  int original_inode = find_or_rename(original, 1, 0 ,NULL);
  
  /*Call find_or_rename fucntion
	The parameters of the find_or_rename function are as follows: 
	the name of the file (original); search the name in files and directories(0); 
	do not rename the file(0); since we are not renaming our file, we do not have to provide a new name(NULL) 
	It returns the index of the given file; returns 0 if the file can not be found. */ 
  int copied_inode = find_or_rename(copied, 0, 0 , NULL);

   // Check whether the original file is found or not.
  if(original_inode == 0)
  	printf("Could not find a file called %s\n", original );

 // Check whether a file or directory named with the given copied name is found or not.
  else if(copied_inode != 0)
    printf("Could not copy the file %s with name %s, since there already exists a file or a directory called %s in the directory\n", original , copied, copied);
  
  // COPY the original file.
  else 
  {

    char itype;
    int blocks[3];
    _directory_entry _directory_entries[4];

    int i,j;
    int empty_dblock=-1,empty_dentry=-1;
    int empty_ientry;

    // do we have free inodes
    if (free_inode_entries == 0) {
      printf("Error: Inode table is full.\n");
      return;
    }

    // read inode entry for current directory
    // in FS304, an inode can point to three blocks at the most
    itype = _inode_table[CD_INODE_ENTRY].TT[0];
    blocks[0] = stoi(_inode_table[CD_INODE_ENTRY].XX,2);
    blocks[1] = stoi(_inode_table[CD_INODE_ENTRY].YY,2);
    blocks[2] = stoi(_inode_table[CD_INODE_ENTRY].ZZ,2);

    // its a directory; so the following should never happen
    if (itype=='F') {
      printf("Fatal Error! Aborting.\n");
      exit(1);
    }

    // now lets try to see find an empty place
    for (i=0; i<3; i++) {
      if (blocks[i]==0) {   // 0 means pointing at nothing
        if (empty_dblock==-1) empty_dblock=i; // we can later add a block if needed
        continue;
      }

      readFS304(blocks[i],(char *)_directory_entries); // lets read a directory entry; notice the cast

      // so, we got four possible directory entries now
      for (j=0; j<4; j++) {
        if (_directory_entries[j].F=='0') { // means unused entry
          if (empty_dentry==-1) { empty_dentry=j; empty_dblock=i; } // AAHA! lets keep a note of it, just in case we have to create the new directory
          continue;
        }
      }

    }

    // if we did not find an empty directory entry and all three blocks are in use; then copy operation can not be made
    if (empty_dentry==-1 && empty_dblock==-1) {   
      printf("Error: Maximum directory entries reached.\n");
      return;
    
    } else { // otherwise

      if (empty_dentry == -1) { // Great! didn't find an empty entry but not all three blocks have been used
        empty_dentry=0;

        if ((blocks[empty_dblock] = getBlock())==-1) {  // first get a new block using the block bitmap
          printf("Error: Disk is full.\n");
          return;
        }

        writeFS304(blocks[empty_dblock],NULL);  // write all zeros to the block (there may be junk from the past!)

        switch(empty_dblock) {  // update the inode entry of current dir to reflect that we are using a new block
          case 0: itos(_inode_table[CD_INODE_ENTRY].XX,blocks[empty_dblock],2); break;
          case 1: itos(_inode_table[CD_INODE_ENTRY].YY,blocks[empty_dblock],2); break;
          case 2: itos(_inode_table[CD_INODE_ENTRY].ZZ,blocks[empty_dblock],2); break;
        }

      }
      // NOTE: all error checkings have already been done at this point!!
      // time to put everything together

      empty_ientry = getInode();  // get an empty place in the inode table which will store info about blocks for this new file

      readFS304(blocks[empty_dblock],(char *)_directory_entries); // read block of current directory where info on this new file will be written
      _directory_entries[empty_dentry].F = '1';     // remember we found which directory entry is unused; well, set it to used now
      strncpy(_directory_entries[empty_dentry].fname,copied,252);  // put the name in there
      itos(_directory_entries[empty_dentry].MMM,empty_ientry,3);  // and the index of the inode that will hold info inside this file
      writeFS304(blocks[empty_dblock],(char *)_directory_entries);  // now write this block back to the disk
      strncpy(_inode_table[empty_ientry].TT,"FI",2);    // create the inode entry...first, its a file, so FI
     
      int original_block_file[3]; 
      original_block_file[0] = stoi(_inode_table[original_inode].XX,2);
      original_block_file[1] = stoi(_inode_table[original_inode].YY,2);
      original_block_file[2] = stoi(_inode_table[original_inode].ZZ,2);

	
	// Copy the contents of the original file to the copied file. Write the copy to the disk.
      char* X[2], Y[2], Z[2];
      itos(X,original_block_file[0], 2);
      itos(Y,original_block_file[1], 2);
      itos(Z,original_block_file[2], 2);
      strncpy(_inode_table[empty_ientry].XX,X,2);  
      strncpy(_inode_table[empty_ientry].YY,Y,2);
      strncpy(_inode_table[empty_ientry].ZZ,Z,2);

      writeFS304(BLOCK_INODE_TABLE, (char *)_inode_table);  // phew!! write the inode table back to the disk
    
    }

  }
  
}

/* FUNCTION: find_or_rename

 * OBJECTIVE: 
 * 		Returns the inode of a file or a directory. Returns zero '0', if the file does not exist.
 *		Renames the file if the renameAvaliable parameter is set to 1 and a newname is given.
 * 		
 * PARAMETERS: 
 *    filename: the name of the file which will be found or renamed
 *    onlyForFile: search the given file only in files (give 0 if you want to include the directories to the search)
 *    renameAvailable: give 1 if you want to rename the given file after it's found
 *    newname: the newname of the file (it is effective, only if renameAvailable is 1)
 */
int find_or_rename(char* filename, int onlyForFile, int renameAvaliable, char* newname){

  char itype;
  int blocks[3];
  _directory_entry _directory_entries[4];

  int file_or_dir_exist=0;

  int i,j;
  int e_inode;

  // read inode entry for current directory
  // in FS304, an inode can point to three blocks at the most
  itype = _inode_table[CD_INODE_ENTRY].TT[0];
  blocks[0] = stoi(_inode_table[CD_INODE_ENTRY].XX,2);
  blocks[1] = stoi(_inode_table[CD_INODE_ENTRY].YY,2);
  blocks[2] = stoi(_inode_table[CD_INODE_ENTRY].ZZ,2);

  // its a directory; so the following should never happen
  if (itype=='F') {
    printf("Fatal Error! Aborting.\n");
    exit(1);
  }

  // lets traverse the directory entries in all three blocks
  for (i=0; i<3; i++) {
    
    if (blocks[i]==0) continue; // 0 means pointing at nothing

    readFS304(blocks[i],(char *)_directory_entries);  // lets read a directory entry; notice the cast

    // so, we got four possible directory entries now
    for (j=0; j<4; j++) {

      	if (_directory_entries[j].F=='0') continue; // means unused entry

      	e_inode = stoi(_directory_entries[j].MMM,3);  // this is the inode that has more info about this entry

      	if (_inode_table[e_inode].TT[0]=='F')  { // entry is for a file
        
	        if(strncmp(_directory_entries[j].fname , filename, 252) == 0){
	          file_or_dir_exist=1;

	          if(renameAvaliable){
	            strncpy(_directory_entries[j].fname, newname, 252);
	            writeFS304(blocks[i],(char *)_directory_entries);
	          }

	          break;     
	        }
      
      	}

     	if(!onlyForFile){

	        if (_inode_table[e_inode].TT[0]=='D')  { // entry is for a directory
	        
		        if(strncmp(_directory_entries[j].fname , filename, 252) == 0){
		          file_or_dir_exist=1;

		          if(renameAvaliable){
		            strncpy(_directory_entries[j].fname, newname, 252);
		            writeFS304(blocks[i],(char *)_directory_entries);
		          }

		          break;     
		        }      
	      
	      	}

    	}

   		
   		if(file_or_dir_exist == 1) break;
    
    } /* end of second 'for' */

  } /* end of first 'for' */

  
  if(file_or_dir_exist == 1) return e_inode;


  return 0;
}

/**  
 * The parseCommand function below will not return any value, but it will just: read
 * in the next command line; separate it into distinct arguments (using blanks as
 * delimiters), and set the args array entries to point to the beginning of what
 * will become null-terminated, C-style strings. 
 */
int parseCommand(char inputBuffer[], char *args[])
{
    int length,		/* # of characters in the command line */
      i,		/* loop index for accessing inputBuffer array */
      start,		/* index where beginning of next command parameter is */
      ct,	        /* index of where to place the next parameter into args[] */
      command_number;	/* index of requested command number */
    
    ct = 0;
	
    /* read what the user enters on the command line */
    do {
	    char *print_dir = malloc(sizeof(char) * 260); 
	    char *tilda = malloc(sizeof(char) * 270); 
	    strncpy(tilda, "~", 1);

	    strncpy(print_dir, current_working_directory, 252);

	    strcat(tilda, print_dir);

	    strcat(tilda , "> ");
	    printf("\e[93m%s\x1b[0m", tilda);
	  

	  fflush(stdout);
	  length = read(STDIN_FILENO,inputBuffer,MAX_LINE); 
    }
    while (inputBuffer[0] == '\n'); /* swallow newline characters */
	
    /**
     *  0 is the system predefined file descriptor for stdin (standard input),
     *  which is the user's screen in this case. inputBuffer by itself is the
     *  same as &inputBuffer[0], i.e. the starting address of where to store
     *  the command that is read, and length holds the number of characters
     *  read in. inputBuffer is not a null terminated C-string. 
     */    
    start = -1;
    if (length == 0)
      exit(0);            /* ^d was entered, end of user command stream */
    
    /** 
     * the <control><d> signal interrupted the read system call 
     * if the process is in the read() system call, read returns -1
     * However, if this occurs, errno is set to EINTR. We can check this  value
     * and disregard the -1 value 
     */

    if ( (length < 0) && (errno != EINTR) ) {
      perror("error reading the command");
      exit(-1);           /* terminate with error code of -1 */
    }
    
    /**
     * Parse the contents of inputBuffer
     */
    
    for (i=0;i<length;i++) { 
      /* examine every character in the inputBuffer */
      
      switch (inputBuffer[i]){
      case ' ':
      case '\t' :               /* argument separators */
	if(start != -1){
	  args[ct] = &inputBuffer[start];    /* set up pointer */
	  ct++;
	}
	inputBuffer[i] = '\0'; /* add a null char; make a C string */
	start = -1;
	break;
	
      case '\n':                 /* should be the final char examined */
	if (start != -1){
	  args[ct] = &inputBuffer[start];     
	  ct++;
	}
	inputBuffer[i] = '\0';
	args[ct] = NULL; /* no more arguments to this command */
	break;
	
      default :             /* some other character */
	if (start == -1)
	  start = i;
	if (inputBuffer[i] == '&') {
	  inputBuffer[i-1] = '\0';
	}
      } /* end of switch */
    }    /* end of for */
    
    /**
     * If we get &, don't enter it in the args array
     */
    
    args[ct] = NULL; /* just in case the input line was > 80 */
    
    return 1;
    
} /* end of parseCommand routine */