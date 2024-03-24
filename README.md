# Mini Shell



## A replicated version of a linux based shell, Myshell, written in C.
Emulates a linux based shell that adheres to the following criteria:

* Created six built-in functions:
  * exit
  * cd
  * pwd
  * stat
  * touch
  * tail (excluding tail with no file/s as a parameter)
* Perform a basic fork-exec-wait pattern to execute all other linux commands
   * Functionality has not been extended to quoting or backslash escaping.
* White space is assumed to be the only way arguments are to be separated, however any number of spaces before, in between or after have been accounted for.

## Instructions for use:
* Insure OS is a linux based platform or running an available linux based distribution
1. After cloning repository "make myshell" will compile all necessary files.
2. ./myshell will run %myshell% until "exit" is prompted.

## Known Bugs
* stat only works for one file

