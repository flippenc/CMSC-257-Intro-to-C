#! /bin/bash
#If there are no parameters entered, ask for a name to print
#else print the parameter name
if [ $# -eq 0 ]
	then
		echo Enter Your Name:
		read name
		echo Your name is: $name
	else
		echo Your name is: $1
fi
