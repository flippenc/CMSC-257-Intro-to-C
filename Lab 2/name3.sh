#! /bin/bash
#counter for how many times to ask for the name
declare -i counter=0
#if no parameters, then ask for a number
#else use the parameter of times to prompt for the name
if [ $# -eq 0 ]
	then
		echo Input a number:
		read temp
		num=$temp
else
	num=$1
fi
#Ask for name and print the name num times
while [ $counter -lt $num ]
	do
		echo Enter a name:
		read name
		echo Your name is: $name
		counter=$((counter+1))
done
