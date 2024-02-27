# Mini Shell



## A replicated version of a linux based shell, Myshell, written in C.

This project, written by Dr. Phil Nelson and administered by Dr. Yasmine Elglaly at Western Washington University, emulates a linux based shell that adheres to the following restrictions and criteria:

* Create six built-in functions
  * exit
  * cd
  * pwd
  * stat
  * touch
  * tail (excluding tail with no file/s as a parameter)
* Perform a basic fork-exec-wait pattern to execute all other linux commands ![image](https://github.com/Brich9/MiniShell/assets/161379100/a5549895-0b3c-461a-a389-2994eb3f1776)

   * Functionality has not been extended to quoting or backslash escaping.
* White space is assumed to be the only way arguments are to be separated, however any number of spaces before, in between or after have been accounted for.

## Instructions for use:
* Insure OS is a linux based platform or running an available linux based distribution
1. After cloning repository "make myshell" will compile all necessary files.
2. ./myshell will run %myshell% until "exit" is prompted.

