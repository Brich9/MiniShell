#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "argparse.h"
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define FALSE (0)
#define TRUE  (1)

/*
* argCount is a helper function that takes in a String and returns the number of "words" in the string assuming that whitespace is the only possible delimeter.
*/
static int argCount(char*line)
{
  int count = 0;
  if(line[0] == '\0'){
    return 0;
  }
  if(!isspace(line[strlen(line) - 1])){
    count = 1;
  } 
  for(int i = 0; i < strlen(line); i++){
    if(isspace(line[i]) && i > 0 && !isspace(line[i-1]) ){
      count++;
    }
  }
  return count;
}
/*
*
* Argparse takes in a String and returns an array of strings from the input.
* The arguments in the String are broken up by whitespaces in the string.
* The count of how many arguments there are is saved in the argcp pointer
*/
char** argparse(char* line, int* argcp)
{
  *argcp = argCount(line);
  if(*argcp == 0){
    return NULL;
  }
  char **arg_array = calloc(*argcp + 1, sizeof(char*));
  int i = 0;
  int string_track = 0;
  int char_track = 0;
  int arg_flag = 0;
  for(int t = 0; t < *argcp; t++) {
    arg_array[t] = calloc(strlen(line) + 1, sizeof(char));
  }
  //looping until the end of the line
  while(line[i] != '\0') {
    //checking if a char thats not a space has been found
    if(!(isspace(line[i]))){
      arg_flag++;
    }
    if(isspace(line[i]) && arg_flag != 0){ 
      arg_array[string_track][char_track] = '\0';
      string_track++;
      char_track = 0;
      arg_flag = 0;
    }else if(arg_flag != 0){
      arg_array[string_track][char_track++] = line[i];
    }
    i++;
  }
  return arg_array;
}
