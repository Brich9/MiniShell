/* CS 347 -- Mini Shell!
 * Original author Phil Nelson 2000
 */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include "argparse.h"
#include "builtin.h"


/* PROTOTYPES */

void processline (char *line);
ssize_t getinput(char** line, size_t* size);
/* main
 * This function is the main entry point to the program.  This is essentially
 * the primary read-eval-print loop of the command interpreter.
 */

int main () {
  //loop until exit is called
  while((0) == (0)) {
    size_t test = 3*sizeof(char);
    char **line = calloc(3, test);
    if(getinput(line, &test) != -1){
      processline(*line);
    }
    free(line[0]);
    free(line);
  } 
  return EXIT_SUCCESS;
}


/* getinput
* line     A pointer to a char* that points at a buffer of size *size or NULL.
* size     The size of the buffer *line or 0 if *line is NULL.
* returns  The length of the string stored in *line.
*
* This function prompts the user for input, e.g., %myshell%.  If the input fits in the buffer
* pointed to by *line, the input is placed in *line.  However, if there is not
* enough room in *line, *line is freed and a new buffer of adequate space is
* allocated.  The number of bytes allocated is stored in *size. 
* Hint: There is a standard i/o function that can make getinput easier than it sounds.
*/
ssize_t getinput(char** line, size_t* size) {
  size_t len = 0;
  printf("%%myshell%% ");
  len = getline(line, &len, stdin);
  if(len == -1) {
    return -1;
  } else { 
    line[0][len-1] = '\0';
    return len;
  }
}


/* processline
 * The parameter line is interpreted as a command name.  This function creates a
 * new process that executes that command.
 * Note the three cases of the switch: fork failed, fork succeeded and this is
 * the child, fork succeeded and this is the parent (see fork(2)).
 * processline only forks when the line is not empty, and the line is not trying to run a built in command
 */
void processline (char *line){

  pid_t cpid;
  int   status;
  int argCount;
  char** arguments = argparse(line, &argCount);
  if(arguments != NULL){
    int ret= 0;
    ret = builtIn(arguments, argCount);
    if(ret == 0) {
      cpid = fork();
      //unsucessful fork
      if(cpid < 0) {
        perror("Fork creation failed");
        exit(EXIT_FAILURE);
      //parent process
      } else if (cpid > 0 ) {
        wait(&status);
        //child process
      } else {
        int exec_error = 0;;
        exec_error = execvp(arguments[0], arguments);
        if(exec_error == -1){
          fprintf(stderr, "%s: command not found\n", arguments[0]);
        }
        exit(0);
      }
    }
  }
  for(int i = 0; i < argCount; i++){
    free(arguments[i]);
  }
  free(arguments);
}

