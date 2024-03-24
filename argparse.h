#ifndef ARGPARSE_H
#define ARGPARSE_H

/*Argparse
* line        the input string that contains arguments seperated by whitespaces
* arggcp      A int pointer that the count of the amount of arguments will be stored
* returns a array of char*'s each of which is a argument 
*/
char** argparse(char* line, int* argcp);

#endif

