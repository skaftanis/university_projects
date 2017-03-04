#!/bin/bash

RE=`zenity --list --column="What element would toy like to change?" Leukoxrisos Iridio Palladio Continue --title="Entry Mode" --width=600 --height=200` 
if [ $? = 1 ]; then
	exit
fi
if [ $RE = "Leukoxrisos|Leukoxrisos" ]; then
	ENTRY=`zenity --entry --text="Give new value for Leukoxrisos" --title="Edit Mode"`
elif [ $RE = "Iridio|Iridio" ]; then
	ENTRY=`zenity --entry --text="Give new value for Iridio" --title="Edit Mode"`
elif [ $RE = "Palladio|Palladio" ]; then
	ENTRY=`zenity --entry --text="Give new value for Palladio" --title="Edit Mode"`
elif [ $RE = "Continue|Continue" ]; then
	echo $RE > RE.txt
	exit
fi
echo $RE > RE.txt
echo $ENTRY > ENTRY.txt
