
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

// program to check if a mode is valid
int is_valid_mode(char *mode)
{

    if (strlen(mode) != 4)
    {
        return 0;
    }
    int x = atoi(mode);
    if (x > 777)
    {
        return 0;
    }
    return 1;
}

// program to convert mode from string to mode_t type
mode_t string_to_filemode(char *str, mode_t *mode)
{
    char *end = NULL;
    *mode = (mode_t)strtol(str, &end, 8);
    if (!end)
        return 0;
    while (isspace(*end))
        end++;
    return *end == '\0' && (unsigned)*mode < 010000;
}

// program to get size of file
long sizeof_(char *file)
{
    struct stat st;

    if (stat(file, &st) == 0)
        return st.st_size;

    return -1;
}

// program to check if file exists or not
int doesExist(char *file)
{
    struct stat file_status;
    if (stat(file, &file_status) < 0)
    {
        return 0;
    }
    return 1;
}

/*
  THIS FUNCTION CREATES A FILE WITH "file" AND WILL PROVIDE THE
  FILE DESCRIPTOR TO USER IF THE FILE CREATION WAS SUCCESSFUL
  OTHERWISE IT WILL PROMPT AN ERROR
*/
void create_file(char *file, char *mode)
{
    // check if file exists
    int o;
    if (o = open(file, O_WRONLY) != -1)
    {
        printf("\nCan't create! File: %s already exists\n", file);
        close(o);
        return;
    }

    // check if file mode is valid
    if (is_valid_mode(mode) == 0)
    {   printf("\nMODE NOT VALID\n");
        return;
    }

    mode_t x;

    // convert file mode using the function string_to_filemode()
    string_to_filemode(mode, &x);

    // create file using creat() syscall
    int f = creat(file, x);

    // error prompt if file not created
    if (f < 0)
    {
        printf("\nERROR : Can't Create file: %s\n", file);
        return;
    }

    printf("\n\nSuccess!\n");
    printf("\nFile descriptor: %d\n", f);

    // closing the file using close() syscall

    f = close(f);

    // error prompt if file not closed
    if (f < 0)
    {
        printf("\nERROR : Can't Close file: %s\n", file);
        return;
    }
}

/*
  THIS FUNCTION WILL READ 'n' BYTES FROM A GIVEN FILE
  AND PROVIDE THE DATA TO USER ON THE "CLI",
  PROVIDED AN OFFSET 'seek' FROM WHICH IT'LL START READING
*/
void read_file(int n, char *file, int seek)
{

    // checking if number of bytes to read is valid
    if (n < 0)
    {
        printf("\nERROR: please enter +ve number of bytes\n");
    }

    // checking if valid offset is provided
    if (seek != 0)
    {
        if (abs(seek) > sizeof_(file))
        {
            printf("\nERROR : INVALID OFFSET %d\n", seek);
            return;
        }
    }

    if (doesExist(file) == 0)
    {
        printf("\nERROR :FILE [ %s ] DOESN'T EXIST!\n", file);
        return;
    }

    if (sizeof_(file) == 0)
    {
        printf("\n\nEMPTY FILE\n\n");
    }

    char *buff = (char *)calloc(n, sizeof(char));

    // open the file using open() syscall
    int f = open(file, O_RDONLY);

    // error prompt if file not closed
    if (f < 0)
    {
        printf("\nERROR : Can't Open file: %s\n", file);
        return;
    }

    // moving to desired offset
    if (seek >= 0)
    {
        if (lseek(f, seek, SEEK_SET) == -1)
        {
            printf("\nERROR : Can't go to offset %d\n", seek);
            close(f);
            return;
        }
    }
    else
    {
        if (lseek(f, seek, SEEK_END) == -1)
        {
            printf("\nERROR : Can't go to offset %d\n", seek);
            close(f);
            return;
        }
    }
    // read n bytes from file

    if (n == 0)
    {
        while (read(f, buff, 1) != 0)
        {
            printf("%s", buff);
        }
    }

    if (n != 0)
    {
        read(f, buff, n);
        printf("%s", buff);
    }

    // closing the file using close() syscall
    f = close(f);

    // error prompt if file not closed
    if (f < 0)
    {
        printf("\nERROR : Can't Close file: %s\n", file);
        return;
    }
}

/*
  THIS FUNCTION NOW CAN DO BOTH APPEND & WRITE

  IT WILL READ 'n' BYTES FROM A GIVEN FILE
  AND WRITE TO ANOTHER FILE,
  PROVIDED AN OFFSET 'seek1' FROM WHICH IT'LL START READING FROM 'file1'
  AND 'seek2' FROM WHICH IT WILL START WRITING TO 'file2'.

  UPDATE: THIS FUNCTION NOW CAN BE USED IN 2 MODES-> 0 to 'write' and 1 to 'append'.
*/
void write_file(int n, char *file1, int seek1, char *file2, int seek2, int mode)
{

    // if user enter -ve bytes
    if (n < 0)
    {
        printf("\nERROR: please enter +ve number of bytes\n");
    }

    // check if input file exists or not
    if (file1 != "0" && doesExist(file1) == 0)
    {
        printf("\nERROR :FILE [ %s ] DOESN'T EXIST!\n", file1);
    }
    if (doesExist(file2) == 0)
    {
        printf("\nERROR :FILE [ %s ] DOESN'T EXIST!\n\nCREATING A NEW FILE NAMED [%s] TO WRITE DATA", file2, file2);
    }

    char *buff = (char *)calloc(n, sizeof(char));
    int f1;

    // f1 is set to 0 , if file1 is 0 ,i.e user wants to write input from CLI
    if (file1 == "0")
    {
        f1 = 0;
    }
    // otherwise open file using open() syscall
    else
    {
        f1 = open(file1, O_RDONLY);
    }

    int f2;

    // if mode=0, i.e write mode, open file in WRITE ONLY mode
    if (mode == 0)
    {
        f2 = open(file2, O_WRONLY | O_CREAT);
    }
    // if mode=1, i.e append mode, open file in WRITE + APPEND mode
    else if (mode == 1)
    {
        f2 = open(file2, O_WRONLY | O_APPEND | O_CREAT);
    }

    // error prompt if file not opened
    if (f1 < 0)
    {
        printf("\nERROR : Can't Open file: %s\n", file1);
        return;
    }
    if (f2 < 0)
    {
        printf("\nERROR : Can't Open file : %s\n", file2);
        return;
    }

    // if f1 isn't '0' then lseek to the offset seek1 in file1
    if (f1 != 0)
    {
        if (seek1 >= 0)
        {
            if (lseek(f1, seek1, SEEK_SET) == -1)
            {
                printf("\nERROR : Can't go to offset %d\n", seek1);
                close(f1);
                return;
            }
        }
        else
        {
            if (lseek(f1, seek1, SEEK_END) == -1)
            {
                printf("\nERROR : Can't go to offset %d\n", seek1);
                close(f1);
                return;
            }
        }
    }
    // if mode=0 (write) is selected then move to seek2 using lseek in fiile2
    if (mode == 0)
    {
        if (seek2 >= 0)
        {

            if (lseek(f2, seek2, SEEK_SET) == -1)
            {
                printf("\nERROR : Can't go to offset %d\n", seek1);
                close(f2);
                return;
            }
        }
        else
        {
            if (lseek(f2, seek2, SEEK_END) == -1)
            {
                printf("\nERROR : Can't go to offset %d\n", seek1);
                close(f2);
                return;
            }
        }
    }
    if (f1 != 0)
    {
        printf("\n\n...Writing data...\n\n");
    }
    else
    {
        printf("\n\nWRITE DATA HERE (TYPE AND ENTER 'EXIT' TO STOP WRITING):\n\n");
    }

    // case when we've to write whole data of file1 to file2
    if (n == 0)
    {
        while (read(f1, buff, 1) != 0)
        {
            if (buff[0] == '\n')
            {
                char *ex = (char *)calloc(5, sizeof(char));
                read(f1, ex, 5);
                if (strcmp(ex, "EXIT\n") == 0)
                {
                    break;
                }
                else
                {
                    write(f2, buff, 1);
                    write(f2, ex, 5);
                }
            }
            else
            {
                write(f2, buff, 1);
            }
        }
    }

    // case when we've to read & write only 'n' bytes
    read(f1, buff, n);
    write(f2, buff, n);

    // close the files using close() sys call.
    f1 = close(f1);
    f2 = close(f2);

    // error prompt if file not closed
    if (f1 < 0)
    {
        printf("\nERROR : Can't Close file: %s\n", file1);
        return;
    }
    if (f2 < 0)
    {
        printf("\nERROR : Can't Close file : %s\n", file2);
        return;
    }
}

/*
  THIS FUNCTION WILL CREATE A COPY OF 'file1' TO A
  DESTINATION 'file2'. THIS ONE USES ABOVE FUNCTIONS
*/
void copy_file_2(char *file1, char *file2, char *mode)
{
    // check if files exists or not
    if (open(file2, O_WRONLY) >= 0)
    {
        printf("\nCan't create COPY! File: %s already exists\n", file2);
        return;
    }
    if (open(file1, O_WRONLY) == -1)
    {
        printf("\nCan't create COPY! File: %s doesn't exists\n", file1);
        return;
    }
    // check if modes are valid
    if (is_valid_mode(mode) == 0)
    {
        return;
    }

    create_file(file2, mode);
    write_file(0, file1, 0, file2, 0, 0);
}

/*
  THIS FUNCTION WILL DISPLAY ALL THE
  INFO RELATED TO A FIKE
*/
void file_info(char *file)
{
    struct stat statbuff;
    int f = stat(file, &statbuff);

    // error prompt if file not closed
    if (f < 0)
    {
        printf("\nERROR: Can't access info of file: %s\n", file);
        return;
    }

    printf("INFO OF FILE: %s\n", file);
    printf("\nFile Size: \t\t%d bytes", statbuff.st_size); // size of file
    printf("\nFile I-node: \t\t%ld", statbuff.st_ino);     // i-node number
    printf("\nLinks to File:\t\t%d", statbuff.st_nlink);   // number of links to a file
    printf("\nFile Permissions: \t");
    printf((S_ISDIR(statbuff.st_mode)) ? "dir" : "-");
    printf((statbuff.st_mode & S_IRUSR) ? "r" : "-");
    printf((statbuff.st_mode & S_IWUSR) ? "w" : "-");
    printf((statbuff.st_mode & S_IXUSR) ? "x" : "-");
    printf(" ");
    // printf( (statbuff.st_mode & S_IRGRP) ? "r" : "-");
    // printf( (statbuff.st_mode & S_IWGRP) ? "w" : "-");
    // printf( (statbuff.st_mode & S_IXGRP) ? "x" : "-");
    // printf(" ");
    // printf( (statbuff.st_mode & S_IROTH) ? "r" : "-");
    // printf( (statbuff.st_mode & S_IWOTH) ? "w" : "-");
    // printf( (statbuff.st_mode & S_IXOTH) ? "x" : "-");
}

// Main function
int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        printf("\n\nWELCOME TO THE CUSTOM FILE HANDLING WIZARD!\nUSE THESE COMMANDS TO USE VARIOUS FUNCTIOINALITIES.");
        printf("\n1) creator \n2) reader \n3) writer \n4) append \n5) copy \n6) info \n7) help\n\n\n");
        printf("\n\nENTER ATLEAST ONE OF THESE ARGUMENTS TO PERFORM AN OPERATION\n");
        printf("\ntype 'help' to know more about how to use these commands\n\n");
        return 0;
    }

    // creating file
    else if (strcmp(argv[1], "creator") == 0)
    {
        if (argc > 4 || argc < 3)
            printf("INVALID NO. OF ARGUMENTS");
        else if (argc == 4)
        {
            if (is_valid_mode(argv[3]) == 0)
            {
                printf("\nINVALID MODE!!! %s\n", argv[3]);
                exit(0);
            }
            create_file(argv[2], argv[3]);
        }
        else if (argc == 3)
        {
            create_file(argv[2], "0777");
        }
    }

    // reading file
    else if (strcmp(argv[1], "reader") == 0)
    {
        if (argc > 5 || argc < 3)
        {
            printf("INVALID NO. OF ARGUMENTS");
        }

        else if (argc == 5)
        {
            // read 'n' bits from 'm' offset from starting
            read_file(atoi(argv[2]), argv[3], atoi(argv[4]));
        }

        else if (argc == 4)
        {

            // read 'n' bits from starting of file
            read_file(0, argv[2], atoi(argv[3]));
        }

        else if (argc == 3)
        {
            // read whole file
            read_file(0, argv[2], 0);
        }
    }

    // writing a file
    else if (strcmp(argv[1], "writer") == 0)
    {
        if (argc > 7 || argc < 3)
        {
            printf("INVALID NO. OF ARGUMENTS");
        }

        else if (argc == 7)
        {

            // write 'x' bytes from file1 to file 2 having offset m & n respectively
            // i.e it'll start read from offset m in file1 & write to offset n in file2
            write_file(atoi(argv[2]), argv[3], atoi(argv[4]), argv[5], atoi(argv[6]), 0);
        }
        else if (argc == 6)
        {

            // write 'x' bytes to offset n of file2
            write_file(atoi(argv[2]), argv[3], 0, argv[4], atoi(argv[5]), 0);
        }
        else if (argc == 5)
        {

            // write 'x' bytes from file1 to file 2 from start
            write_file(atoi(argv[2]), argv[3], 0, argv[4], 0, 0);
        }
        else if (argc == 4)
        {
            // write from cli to an offset
            write_file(0, "0", 0, argv[2], atoi(argv[3]), 0);
        }
        else if (argc == 3)
        {

            // write from command line argument
            write_file(0, "0", 0, argv[2], 0, 0);
        }
    }

    else if (strcmp(argv[1], "append") == 0)
    {

        if (argc > 6 || argc < 3)
        {
            printf("INVALID NO. OF ARGUMENTS");
        }
        else if (argc == 6)
        {
            // append 'x' bytes from file1's offset 'n' to file 2 from start
            write_file(atoi(argv[2]), argv[3], atoi(argv[4]), argv[5], 0, 1);
        }
        else if (argc == 5)
        {
            // append 'x' bytes from file1 to file 2 from start
            write_file(atoi(argv[2]), argv[3], 0, argv[4], 0, 1);
        }
        else if (argc == 4)
        {

            // append whole file1 t0 file2
            write_file(0, argv[2], 0, argv[3], 0, 1);
        }
        else if (argc == 3)
        {

            // append from command line argument
            write_file(0, "0", 0, argv[2], 0, 1);
        }
    }

    else if (strcmp(argv[1], "copy") == 0)
    {
        if (argc > 5 || argc < 4)
        {
            printf("INVALID NO. OF ARGUMENTS");
        }
        else if (argc == 5)
        {
            // copy source file to a destination with custom mode
            if (is_valid_mode(argv[4]) == 0)
            {
                printf("\nINVALID MODE!!! %s\n", argv[4]);
                exit(0);
            }
            else
            {
                copy_file_2(argv[2], argv[3], argv[4]);
            }
        }
        else if (argc == 4)
        {

            // copy source file to a destination
            copy_file_2(argv[2], argv[3], "0777");
        }
    }

    // display the info of a file
    else if (strcmp(argv[1], "info") == 0)
    {
        if (argc != 3)
            printf("INVALID NO. OF ARGUMENTS");
        else
            file_info(argv[2]);
    }

    // display the help manual
    else if (strcmp(argv[1], "help") == 0)
    {
        read_file(0, "examples/help.txt", 0);
    }

    else
    {
        printf("\n\nINVALID ARGUMENTS!! \nUSE THESE ONLY: \n1) creator \n2) reader \n3) writer \n4) append \n5) copy \n6) info \n7) help \n");
    }
    return 0;
}