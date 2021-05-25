#!/bin/bash
#Kill pid giving a process name
argc=$@
if [ $# -eq 1 ]
then
	dt=$(ps | grep $argc | awk '{print $1}')
	for  i in $dt 
	do
		kill $i 
	done
else
	echo  U have to add the process name
fi
