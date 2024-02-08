#!/bin/bash
#*************************************************************
#Declaring a function to ask for and print a name an input number of times
#if no parameter input, the function asks for a number
#*************************************************************
printName () {
#counter variable to count how many times the name has been printed
declare -i counter=0
#if no parameters, ask for a number, else set $1 to be the
#number of times to prompt for and print names
if [ $# -eq 0 ]
	then
		echo Input a number:
		read temp
		num=$temp
else
	num=$1
fi
#while the counter is less than the desired number, prompt and print num times
while [ $counter -lt $num ]
	do
		echo Enter a name:
		read name
		echo Your name is $name
		counter=$((counter+1))
done
}
printName 
