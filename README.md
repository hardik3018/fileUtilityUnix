HI, HELPER IS HERE TO HELP YOU!!  :D


This is a modular function in C-language that carries out various file handling tasks.



Here are the following functionalities you can perform on a file with detailed explanation.

( Note: Write " ./(outputfilename) " before using any of the following functionality. )


--------------------------------------------------------------------------------------


      1. creator 

            SYNTAX: -> creator  char* filename_with_destination_here

            DESCRIPTION: This will create a file with default mode of the 
                         system and will provide it's file descriptor.


            SYNTAX: -> creator  char* filename_with_destination_here  mode_t mode

            DESCRIPTION: This will create a file with mode provided by user          
                         and will provide it's file descriptor.


           

--------------------------------------------------------------------------------------


      2. reader

            SYNTAX: -> reader char* filename

            DESCRIPTION: This will read a whole file and provide the data 
                         read.


            SYNTAX: -> reader char* filename int offset_t

            DESCRIPTION: This will all data from the offset given by user 
                         from file and provide the 'n_bytes' data read.


            SYNTAX: -> reader  int n_bytes  char* filename  int offset_t
            
            DESCRIPTION: This will read 'n_bytes' of data from the offset 
                         'offset_t' of a file and provide the 'n_bytes' 
                         data read.

            MORE: to read 'n' bytes from starting write this.
            SYNTAX : reader int n_bytes char* filename 0
                  


--------------------------------------------------------------------------------------


      3. writer 

            SYNTAX: -> writer file1

            DESCRIPTION: This will write content to a file from the input 
                         provided in command line interface.


            SYNTAX: -> writer file1 offset_t

            DESCRIPTION: write to offset_t of file1 from command line interface.


            SYNTAX: -> writer n_bytes file1 file2

            DESCRIPTION: This will write 'n_bytes' from starting of file1 to 
                         the starting of file2.


            SYNTAX: -> writer n_bytes file1 file2 offset2

            DESCRIPTION: This will write 'n_bytes' from starting of file1 
                         to file2 starting from the offset2


            SYNTAX: -> writer n_bytes file1 offset1 file2 offset2

            DESCRIPTION: This will write 'n_bytes' from 'offset1' of 
                         file1 to file2 starting from the 'offset2'
      
      (IF YOU GIVE AN OFFSET LARGER THAN FILE SIZE THEN THE DATA WILL BE AUTOMATICALLY 
       WRITTEN TO THE LAST OFFSET OF FILE)
      
      (NOTE: type 'EXIT' and Enter to stop writing from COMMAND LINE INTERFACE)


--------------------------------------------------------------------------------------


      4. append

            SYNTAX: -> append file1

            DESCRIPTION: This will append content to a file from the 
                         input provided in command line interface.


            SYNTAX: -> append file1 file2

            DESCRIPTION: This will append all the data from 'file1' 
                         to 'file2'.


            SYNTAX: -> append n_bytes file1 file2

            DESCRIPTION: This will append  'n_bytes' from starting of 
                         file1 to the file2.


            SYNTAX: -> append n_bytes file1 offset1 file2 

            DESCRIPTION: This will append 'n_bytes' from 'offset1' of 
                         file1 to file2.
      
      (IF YOU GIVE AN OFFSET LARGER THAN FILE SIZE THEN THE DATA WILL BE AUTOMATICALLY 
       WRITTEN TO THE LAST OFFSET OF FILE)
      
      (NOTE: type 'EXIT' and Enter to stop writing from COMMAND LINE INTERFACE)


--------------------------------------------------------------------------------------


      5. copy

            SYNTAX: -> copy sourcefile destinationfilepath

            DESCRIPTION: This will copy the source file to the 
                         destination path.
            
            SYNTAX: -> copy sourcefile destinationfilepath mode_t mode

            DESCRIPTION: This will copy the source file to the 
                         destination path with mode specified by user.


--------------------------------------------------------------------------------------


      6. info

            SYNTAX: -> info filename

            DESCRIPTION: This will provide the the info related to the file.
                         File Size, Links to file, number of I-nodes, 
                         permissions.


--------------------------------------------------------------------------------------


      7. help

            SYNTAX:->help

            DESCRIPTION: To display this helper page :)


--------------------------------------------------------------------------------------


Hope this will help you! Have a good day.
