#!/bin/bash

#fedora_setup


ARCH=`uname -m`
PASS=$(zenity --password --title="Give your sudo password")
if [ $? = 1 ]; then
	exit
fi

(
	echo "20" ; sleep 1

	echo "#Removing libGLEW 1.9"
	echo $PASS | sudo yum remove -y libGLEW

	echo "50"; sleep 1

	echo "#Installing  libGLEW 1.5"
	cd libs
	if [ $ARCH = x86_64 ]; then
		echo $PASS | sudo rpm -i libGLEW-1.5.8-4.fc15.x86_64.rpm
	else 
		echo $PASS | sudo rpm -i libGLEW-1.5.8-4.fc15.i686.rpm
	fi
	cd ..
	cd SFML-2.1/lib
	echo $PASS | su -c "echo $(pwd) >> /etc/ld.so.conf"
	echo $PASSS | sudo ldconfig

	echo "70" ; sleep 1
	echo "#Building application"
	cd ..
	cd ..
	g++ -c *.cpp -ISFML-2.1/include
	g++ *.o -o TheUncharted -LSFML-2.1/lib -lsfml-graphics -lsfml-window -lsfml-system



	echo "#Finished"
	echo "100" ; sleep 1



	) | zenity --progress \
	--title="The Uncharted" \
	--text="Seting Up TheUncharted"\
	--percentage=0