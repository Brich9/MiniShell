#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h> 
#include <fcntl.h> 
#include <ctype.h>
#include "builtin.h"
#include <string.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

//Prototype
static void exitProgram(char** args, int argcp); 
static void cd(char** args, int argpcp);
static void pwd(char** args, int argcp);
static void my_stat(char** args, int argcp);
static void my_tail (char **args, int argcp);
static void my_touch(char **args, int argcp);
static void calculate_timevals(struct timespec access_time, int print_flag);

/* builtIn
** built in checks each built in command the given command, if the given command
 * matches one of the built in commands, that command is called and builtin returns 1.
 *If none of the built in commands match the wanted command, builtin returns 0;
 */

//**NOTE** memory and error handling for entire file has not been done yet.
int builtIn(char** args, int argcp)
{
  if(strcmp(args[0], "stat") == 0) {
    my_stat(args, argcp);
    return 1;
  } else if (strcmp(args[0], "tail") == 0) {
    my_tail(args, argcp);
    return 1;
  } else if (strcmp(args[0], "touch") == 0){
    my_touch(args, argcp);
    return 1;
  } else if(strcmp(args[0], "cd") == 0){
    cd(args, argcp);
    return 1;
  } else if(strcmp(args[0], "pwd") == 0){
    pwd(args, argcp);
    return 1;
  } else if (strcmp(args[0], "exit") == 0){
    exitProgram(args, argcp);
    return 1;
  } else {
    return 0;
  }
}
//exits the program with default of 0 or specified exit code
static void exitProgram(char** args, int argcp)
{
  int status = 0;
  if(argcp > 1) {
    char *temp_status = args[1];
    status = (int)*temp_status;
  }
  for(int i = 0; i < argcp; i++){
    free(args[i]);
  }
  free(args);
  exit(status);
}
//prints the current working directory
static void pwd(char** args, int argpc)
{
  char current_dir[260];
  if((getcwd(current_dir, sizeof(current_dir))) == NULL) {
    perror(NULL);
  } else {
    printf("%s\n", current_dir); 
  }
}
//changes the current working directory to ., .. , home, or sepecified path
static void cd(char **args, int argcp)
{
  //checking for specific file path, ., or ..
  if(argcp > 1 && argcp < 3){
    if(chdir(args[1]) == -1 ){
      perror("bash: cd");
    }
    //checking for just cd
  } else if (argcp > 0 && argcp < 2) {
    if(chdir("/home") == -1 ){
      perror("bash: cd");
    }
    //fail if more than 2 args are called
  } else {
    fprintf(stderr, "bash: cd: too many arguments\n");
  }
}
//touch creates new file, if the file already exists updates timestamps
static void my_touch(char **args, int argcp){
  if(argcp < 2 ){
    fprintf(stderr, "touch: missing file operand\n");
    return;
  } 
  //loop for number of files inputed
  for(int i = 1; i < argcp; i++) {
    char *file = args[i];
    if(strlen(args[i]) > 260) {
      fprintf(stderr,"'%s': File name too long\n", args[i]);
    } else {
      struct timespec update_time[2];
      struct stat *buf = calloc(1,sizeof(struct stat));
      stat(file, buf);
      int buf_size =  buf->st_size;
      //check to see if file exists
      if(buf_size < 1){
        FILE *create_file = fopen(file, "w+");
        if(create_file == NULL){
          perror(NULL);
        }
        fclose(create_file);
        update_time[0].tv_sec = 0;
        update_time[0].tv_nsec = UTIME_NOW;
        update_time[1].tv_sec = 0;
        update_time[1].tv_nsec = UTIME_NOW;
        if(utimensat(AT_FDCWD, file, update_time, 0) == -1){
          perror(NULL);
        }
        //if it already exists update timestamps
      } else {
        update_time[0].tv_sec = 0;
        update_time[0].tv_nsec = UTIME_NOW;
        update_time[1].tv_sec = 0;
        update_time[1].tv_nsec = UTIME_NOW;
        if(utimensat(AT_FDCWD, file, update_time, 0) == -1){
          perror(NULL);
        }
      }
      free(buf);
    }
  }  
}
//tail prints the last 10 lines of specified file(s)
static void my_tail (char **args, int argcp){
  if(argcp < 2) {
    fprintf(stderr, "tail: missing file operand(s)\n");
    return;
  } 
  //loop for #of files
  for(int i = 1; i < argcp; i++) {
    if(argcp > 2){
      printf("==> %s <==\n", args[i]);
    }
    char *file = args[i];
    FILE *collect_file = fopen(file, "r");
    int num_lines = 0;
    if(collect_file == NULL) {
      fprintf(stderr, "tail: cannot open '%s' for reading: ", args[i]);
      perror(NULL);
    } else {
      char *line_track = NULL;
      size_t size = 0;
      //initial loop gathers #of lines
      while(getline(&line_track, &size, collect_file) != -1){
        num_lines++;
      }
      free(line_track);
      fclose(collect_file);
    }
    char *new_track = NULL;
    int ten_lines = 0;
    FILE *print_file = fopen(file, "r");
    if(print_file != NULL){
      size_t new_size = 0;
      //secondary loop prints out lines based on previouly collected info
      while(getline(&new_track, &new_size, print_file) != -1 ){
        if((num_lines - ten_lines) <= 10){
          printf("%s", new_track);
        }
        ten_lines++;
      }
      printf("\n");
      free(new_track);
      fclose(print_file);
    }
  }
}
//stat displays file info
static void my_stat(char** args, int argcp){
  if(argcp < 2 ){
    fprintf(stderr, "stat: missing file operand\n");
    return;
  }
  char *fd = args[1];
  FILE *error_check = fopen(fd, "r");
  if(error_check == NULL){
    fprintf(stderr, "stat: cannot statx '%s': \n", args[1]);
    perror(NULL);
  } else {
    struct stat *buf = malloc(sizeof(struct stat));
    stat(fd, buf);
    printf("%7s %s \n","File:", fd);
    char *file_type = calloc(sizeof( "regular empty file"), sizeof(char));
    char permissions[11];
    permissions[10] = '\0';
    //declaring file permissions all as - if they have permissons they will change later
    for (int i = 0; i < 10; i++){
      permissions[i] = '-';
    }
    //determining file type
    switch (buf->st_mode & S_IFMT) {
        break;
      case S_IFBLK:
        strcpy(file_type, "block device");  
        permissions[0] = 'b';          
        break;
      case S_IFCHR:  
        strcpy(file_type, "character device");   
        permissions[0] = 'c';     
        break;
      case S_IFDIR:  
        strcpy(file_type, "directory");    
        permissions[0] = 'd';            
        break;
      case S_IFIFO: 
        strcpy(file_type, "FIFO/pipe");  
        permissions[0] = 'p';              
        break;
      case S_IFLNK:  
        strcpy(file_type, "symlink");    
        permissions[0] = 's';           
        break;
      case S_IFREG: 
        if(buf->st_size < 1){
          strcpy(file_type, "regular empty file");
        } else {
          strcpy(file_type, "regular file");
        }
        break;
      case S_IFSOCK: 
        strcpy (file_type,"socket");   
        permissions[0] = 's';            
        break;
      default:       
        strcpy(file_type, "unknown?");                 
        break;
    }
    //Determining the read/write/execute permissions.
    int file_mask = 07777;
    int user_permissions = buf->st_mode & file_mask;
    if(buf->st_mode & S_IRUSR) {
      permissions[1] = 'r'; 
    } 
    if(buf->st_mode & S_IWUSR) {
      permissions[2] = 'w';
    }
    if(buf->st_mode & S_IXUSR) {
      permissions[3] = 'x';
    }
    if(buf->st_mode & S_IRGRP){
      permissions[4] = 'r';
    }
    if(buf->st_mode & S_IWGRP){
      permissions[5] = 'w';
    }  
    if(buf->st_mode & S_IXGRP){
      permissions[6] = 'x';
    }
    if(buf->st_mode & S_IROTH){
      permissions[7] = 'r';
    }
    if(buf->st_mode & S_IWOTH){
      permissions[8] = 'w';
    }
    if(buf->st_mode & S_IXOTH){
      permissions[9] = 'x';
    }
    struct passwd *user_name;
    user_name = getpwuid(buf->st_uid); 
    struct group *grp_name;
    grp_name = getgrgid(buf->st_gid);

    printf("%s\n", grp_name->gr_name);
    printf("%7s %ld%19s %ld%18s %ld %s\n","Size:",buf->st_size,"Blocks:", 
      buf->st_blocks, "IO Block:", buf->st_blksize, file_type);
    printf("%7s %ld,%ld%10s %ld%12s %ld\n", "Device:", buf->st_rdev, buf->st_dev,
      "Inode:",buf->st_ino, "Links:", buf->st_nlink);
    //right shifting octal bytes by 6
    printf("%7s (%04o/%s)  %s(%d/%s)   %s(%d/%s)\n", "Access:", 
      user_permissions, permissions, "Uid: ", 
      buf->st_uid, user_name->pw_name,"Gid: " ,buf->st_gid, grp_name->gr_name);
    //calculating timestamps
    calculate_timevals(buf->st_atim, 0);
    calculate_timevals(buf->st_mtim, 1);
    calculate_timevals(buf->st_ctim, 2);
    //st_birthtime unavailable
    printf("%7s\n", "Birth: -");
    free(file_type);   
    free(buf);
  }
}
//this function calculates the timestamps for Access, Modification and
// Change then prints them out structured for stat
static void calculate_timevals(struct timespec access_time, int print_flag){
  time_t convert_access = access_time.tv_sec;
  time_t current_time = mktime(localtime(&convert_access));
  time_t gm_time = mktime(gmtime(&convert_access));
  int difference = difftime(current_time, gm_time );
  int h_difference = (difference / 3600) * 100;
  char t_buff[80];
  char string[10];
  string[9] = '\0';
  if(print_flag == 0) {
    strcpy(string, "Access: ");
  } else if(print_flag == 1) {
    strcpy(string, "Modify: ");
  } else {
    strcpy(string, "Change: ");
  }
  strftime(t_buff, sizeof(t_buff), "%Y-%m-%d %H:%M:%S", localtime(&convert_access));
  printf("%7s%s.%06ld %05d\n", string, t_buff, (access_time.tv_nsec), h_difference);
}