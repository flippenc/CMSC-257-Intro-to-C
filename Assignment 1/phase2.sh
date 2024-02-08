########################
#Assignment 1 - Phase 2#
#Christopher Flippen   #
#CMSC 257 - Dahlberg   #
#2/4/19                #
#######################################################################################################################################
# This program allows the user to work with file archives from a set of options                                                       #
# The user can archive files, extract files from an archive, and view the contents of an archive                                      #
# List of uses of the program:                                                                                                        #
# 1)	Allow the user to archive a set of files. First, the user will be prompted for a set of options.                              #
# 	Then, the user will be prompted for the output directory. Next, the user will be repeatedly prompted for the list of files.   #
#       The user will enter each file or directory one at a time, with the script continuing to prompt until the user specifies “:a”. #
#	Finally, the user will be prompted for the name of the output tarball.                                                        #
# 2)	Allow the user to extract a tarball. First, the user will be prompted for the archive to be extracted.                        #
#	Finally, the user will be prompted for the output directory.                                                                  #
# 3)	Allow the user to view the contents of a tarball. The user will only be prompted for the name of the archive to be viewed.    #
#######################################################################################################################################
#The quit function is used to exit the program is the user types ":q", this function is called whenever there is user input#
############################################################################################################################
quit () {
if [ "$1" == ':q' ]
	then
        	exit 1
fi
}
#########
#ARCHIVE#
#########
#############################################################################################################
#User gets choice to use gzip or bzip2 for archiving and if they want a verbose output                      #
#If the user makes an invalid selection, the program will loop until they make a valid one or use :q to quit#
#############################################################################################################
archive () {
	echo Select your options:
	echo v - show the progress of archiving \(verbose output\)
	echo j - use bzip2
	echo z - use gzip
	read selection
	#***********************
	#quit if user inputs :q*
	#***********************
	quit $selection
	#*****************************************************************************************************
	#Check if the selection contains exactly 1 compression type                                          *
	#I used Teemu Liesti and Adam Bellaire's syntax to check if the input string contains certain letters*
	#https://stackoverflow.com/questions/229551/how-to-check-if-a-string-contains-a-substring-in-bash    *
	#*****************************************************************************************************
	until [[ "$selection" == *"j"* &&  "$selection" != *"z"* ]] || [[ "$selection" != *"j"* && "$selection" == *"z"* ]]
		do
			echo You have to specifiy 1 compression type
			echo j is bzip2 and z is gzip
			echo Please re-enter your selection
			read selection
			#*******************************
			#:q is used to quit the program*
			#*******************************
			quit $selection
	done
	#****************************************************************************************************************
	#Checking which selection the user made and choosing the correct file extension and tar flag options accordingly*
	#****************************************************************************************************************
	if [[ "$selection" == *"j"* ]] && [[ "$selection" == *"v" ]]
		then
			compression='-cjf'
			extension='.bz2'
	elif [[ "$selection" == *"z"* ]] && [[ "$selection" == *"v"* ]]
		then
			compression='-czf'
			extension='.tgz'
	elif [[ "$selection" == *"j"* ]]
		then
			compression='-cjf'
			extension='.bz2'
	elif [[ "$selection" == *"z"* ]]
		then
			compression='-czf'
			extension='.tgz'
	fi
	#*****************************************************************************************************************************
	#All of the files for the tar are going to come from one directory, the program will use cd to move into the chosen directory*
	#*****************************************************************************************************************************
	echo Input the directory containing the source files
	echo Type \* for the current directory
	read location
	#****************
	#:q used to quit*
	#****************
	quit $location
	#****************************************
	#Asterisk (*) used for current directory*
	#****************************************
	if [ "$location" == '*' ]
		then
			echo You chose the current directory, $PWD
			location="${PWD}"
	fi
	#**************************************************************************************************
	#Using -d to check for directory existance                                                        *
	#I used BlueCacti's response to check the syntax for checking if files or directories exist       *
	#https://stackoverflow.com/questions/638975/how-do-i-tell-if-a-regular-file-does-not-exist-in-bash*
	#**************************************************************************************************
	until [ -d "$location" ]
		do
			echo This directory does not exist or is not a directory, try again
			read location
			#***********************
			#quit if user input :q*
			#***********************
			quit $location
			#*************************************************************
			#PWD stores the current directory, this is used if * is input*
			#*************************************************************
			if [ "$location" == '*' ]
				then
					echo You chose the current directory, $PWD
					location="${PWD}"
			fi
	done
	#***********************************************************************************************************************
	#cd used to move into the directory where the tar file will be created and the compressed files/directories are located*
	#$directory will store all of the files that the user wants to add to the archive                                      *
	#***********************************************************************************************************************
	cd $location
	directory=""
	#*******************************************************************************************************************
	#Keep adding files or directories until the program is quit or :a is entered and checking that the user doesn't try*
	#to create a directory with no files in it                                                                         *
	#*******************************************************************************************************************
	until [ "$directory" != "" ] && [ "$add" == ':a' ]
		do
			echo :a to proceed to archiving once at least 1 file or directory is added
			echo Type a file or directory name to add it to the archive
			#*****************************************************************************
			#$add is a string that stores all the files that will be added to the archive*
			#*****************************************************************************
			read add
			#*****************
			#quit if :q input*
			#*****************
			quit $add
			#*******************************************************************************************
			#Checking that the user is not creating an archive with no files in it and warning the user*
			#*******************************************************************************************
			if [ "$directory" == "" ] && [ "$add" == ':a' ]
				then
					echo You must specifiy at least 1 file to include in the directory before archiving
			fi
			#**************************************************************************
			#if a file exists, it is concatenated onto $add                           *
			#if -e says the file does not exist, then it is not concatenated onto $add*
			#**************************************************************************
			if [ -e "$add" ]
				then
					#************************************************************************************
					#The space in the string is used to make sure the file names are seperated correctly*
					#and without a leading space                                                        *
					#************************************************************************************
					echo "$add" added to the archive
					directory="$directory$add "
			elif [ ! -e "$add" ] && [ "$add" != ':a' ]
				then
					#*************************************************************************
					#Nothing added to the directory if the file is not found or doesn't exist*
					#User told when if this happens                                          *
					#*************************************************************************
					echo "$add" not added, file or directory not found
			fi
	done
	if [ "$directory" == "" ]
		then
			add=""
			echo You must specify at least 1 file to include in the directory
			echo Choose a file to add to the directory, then use :a to proceed to archiving
			until [ "$directory" != "" ] && [ "$add" == ':a' ]
				do
					echo :a to proceed to archiving
                        		echo type a file or directory name to add it to the archive
                       			#*****************************************************************************
                       			#$add is a string that stores all the files that will be added to the archive*
                       			#*****************************************************************************
                        		read add
                        		#*****************
                        		#quit if :q input*
                        		#*****************
                        		quit $add
                        		#**************************************************************************
                        		#if a file exists, it is concatenated onto $add                           *
                        		#if -e says the file does not exist, then it is not concatenated onto $add*
                        		#**************************************************************************
                        		if [ -e "$add" ]
                               			then
                                        		#************************************************************************************
                                        		#The space in the string is used to make sure the file names are seperated correctly*
                                        		#and without a leading space                                                        *
                                        		#************************************************************************************
                                        		directory="$directory$add "
					fi
			done
	fi
	echo Input the name of the archive without a file extension
	#**************************************************************************************
	#archiveName concatenated with the file extension $extension becomes the tar file name*
	#**************************************************************************************
	read archiveName
	#*****************
	#quit if :q input*
	#*****************
	quit $archiveName
	echo Creating tar file
	#*******************************************************************
	#Creating the tar archive with all of the user's chosen information*
	#*******************************************************************
	echo tar "$compression" ./"$archiveName""$extension" "$directory"
	tar $compression ./$archiveName$extension $directory
}
#########
#EXTRACT#
#########
#######################################################################################################################
#The user selects an archive file and the program will keep looping until the user quits or types a valid archive file#
#######################################################################################################################
extract () {
	echo Input the name of the archive to be extracted
	read extractFrom
	#****************
	#:q used to quit*
	#****************
	quit $extractFrom
	#**********************************************************************************************************
	#I used user183744 and daniel's method to check if the file is a tar                                      *
	#https://stackoverflow.com/questions/6853022/can-tar-xvf-be-used-in-a-if-statement-to-return-true-or-false*
	#Checking if the file is a tar file by trying to read it, if the return condition is not 0,               *
	#then it means tar could not read it                                                                      *
	#**********************************************************************************************************
	tar -tf $extractFrom	
	until [  $?  -eq 0  ]
		do
			echo "$extractFrom" does not exist or is not an archive, try again
			read extractFrom
			#****************
			#:q used to quit*
			#****************
			quit $extractFrom
			tar -tf $extractFrom
		done
	#*************************************************************
	#Obtaining a directory to extract to and saving as $extractTo*
	#*************************************************************
	echo File $extractFrom found and is an archive
	echo Input the directory to extract to \(use \* if current directory\)
	read extractTo
	#****************
	#:q used to quit*
	#****************
	quit $extractTo
	#***********************************************************************************
	# Asterisk (*) used for current directory, $PWD used to find what this directory is*
	#***********************************************************************************
	if [ "$extractTo" == '*' ]
		then
			echo You chose the current directory, $PWD
			extractTo="${PWD}"
			#***********************************************************
			#The program now has enough information to extract the file*
			#extracts the file and then returns                        *
			#***********************************************************
			echo tar -xvf "$extractFrom" --directory "$extractTo"
			tar -xvf "$extractFrom" --directory "$extractTo"
			return 0
	else
		#**************************************************************
		#Program loops until a valid directory to extract to is chosen*
		#**************************************************************
		until [ -d "$extractTo" ]
			do
				echo This directory does not exist or is not a directory, try again
				read extractTo
				#****************
				#:q used to quit*
				#****************
				quit $extractTo
				#*************************************************************************************************
				# Asterisk (*) used to choose current directory, the program uses $PWD to check current directory*
				#*************************************************************************************************
				if [ "$extractTo" == '*' ]
					then
						echo You chose the current directory, $PWD
						extractTo="${PWD}"
						#***********************************************************
						#The program now has enough information to extract the file*
						#extracts the file and then returns                        *
						#***********************************************************
						echo tar -xvf "$extractFrom" --directory "$extractTo"
						tar -xvf "$extractFrom" --directory "$extractTo"
						echo Done
						exit 0
				else
					echo Checking file "$extractTo"
			fi
		done
		#***********************************************************
		#The program now has enough information to extract the file*
		#extracts the file and then returns                        *
		#***********************************************************
		echo tar -xvf "$extractFrom" --directory "$extractTo"
		tar -xvf "$extractFrom" --directory "$extractTo"
		echo Done
		exit  0
	fi
}
######
#VIEW#
######
###########################################################
#Takes an archive name as an input and prints its contents#
###########################################################
view () {	
	#***************************************
	#Choose the name of the archive to view*
	#***************************************
	echo Input the name of the archive to be viewed
	read viewArchive
	#***********
	#quit if :q*
	#***********
	quit $viewArchive
	#*************************************************************************************************************************
	#The loop continues until a valid tar file is input or the program is quit                                               *
	#Checking if $viewArchive is a tar file by using -tf and $? -eq 0 is used to check the return status for reading the file*
	#*************************************************************************************************************************
	tar -tf $viewArchive
	until [ $? -eq 0 ]
		do
			echo "$viewArchive" does not exist or is not an archive, try again
			read viewArchive
			#***********
			#quit if :q*
			#***********
			quit $viewArchive
			#************************************
			#checking the file for the next loop*
			#************************************
			tar -tf "$viewArchive"
		done
	echo File $viewArchive found and is an archive
	echo Listing contents:
	#*****************************************************
	#Printing the contents of the tar file, then quitting*
	#*****************************************************
	tar -tvf "$viewArchive"
	exit 0
}
###############
#MAIN FUNCTION#
################################################################################################################
#Function that takes in an input option: archive, extract, or view and chooses the correct function accordingly#
################################################################################################################
tarballTool () {
#*******************************************************************
#Checking if any parameters input, else ask for one and ignore case*
#*******************************************************************
if [ $# -eq 1 ]
        then
                #***************************************************************************************************
                #I used the the syntax provided by alphaniner and iCodez to ignore case                            *
                #https://stackoverflow.com/questions/1728683/case-insensitive-comparison-of-strings-in-shell-script*
                #If there is a parameter, ignore case                                                              *
                #***************************************************************************************************
                echo At any time, :q can be used to quit
                choice="${1,,}"
        else
                #****************************
                #Set choice to a dummy value*
                #****************************
                choice=default
fi
#**************************************************************************************
#Ignore case, check which option, if none chosen, loops until a value choice is chosen*
#**************************************************************************************
while [ "${choice,,}" != 'archive' ] && \
      [ "${choice,,}" != 'extract' ] && \
      [ "${choice,,}" != 'view' ]
        do
                echo Choose an option:
                echo archive, extract, and view are your options
                echo At any time, :q can be used to quit
                read choice
                #*********************************
                #Quit the program if user uses :q*
                #*********************************
                quit $choice
                #*****************************************
                #If the choice is valid, store in $choice*
                #*****************************************
                choice="${choice,,}"
done
echo You chose $choice
if [ "${choice}" == 'archive' ]
        then
                archive
elif [ "${choice}" == 'extract' ]
        then
                extract
elif [ "${choice}" == 'view' ]
        then
		view
else
	exit -1
fi
}
#**************************
#Calling the main function*
#**************************
tarballTool $1
