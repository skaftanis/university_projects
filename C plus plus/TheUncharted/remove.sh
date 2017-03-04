#!/bin/bash

for i in $(seq 1 $1)
do
    echo "$i"
    if [ $i = $1 ]; then
	echo "Continue"
    fi
done | zenity --list --title="Remove Move" --text="text" --column="Select a Robot to Remove"   > RE.txt


# το $1 είναι το RobotsAdded που μπαίνει σαν όρισμα από τη κλήση της στο
# C++ αρχείο. 

# η απάντηση θα είναι αποθηκευμένη στο RE.txt
