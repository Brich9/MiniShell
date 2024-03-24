myshell: myshell.o argparse.o builtin.o
	gcc -o myshell argparse.o builtin.o myshell.o

argparse.o: argparse.c argparse.h
	gcc -c -g  argparse.c -Wall -Werror
	
builtin.o: builtin.c builtin.h
	gcc -c -g  builtin.c -Wall -Werror

myshell.o: myshell.c builtin.h argparse.h
	gcc -c -g  myshell.c -Wall -Werror

clean:
	rm -f myshell
	rm -f *.o *.bak *~*
