# Mini Shell



## A replicated version of a linux based shell, Myshell, written in C.

This project, written by Dr. Phil Nelson and administered by Dr. Yasmine Elglaly at Western Washington University, emulates a linux based shell that adheres to the following restrictions and criteria:

* Create six built-in functions
  * exit
  * cd
  * pwd
  * stat
  * touch
  * tail (excluding tail with no file)
* Perform a basic fork-exec-wait pattern to execute all other linux commands
   * Functionality has not been extended to quoting or backslash escaping.
* White space is assumed to be the only way arguments are to be separated, however any number of spaces before, in between or after have been accounted for.

