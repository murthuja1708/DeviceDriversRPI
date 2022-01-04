#!/bin/bash
make clean
make
set -e

if [ $1 ]
then

	is_device_exists=$(ls /dev/$1)
	if [ $is_device_exists ]
	then
		echo "device already exists"
	else
		read -p "please enter major number\n"  major_number
		read -p "please enter minor number\n"  minor_number
	        read -p "do you really want to create $1 node?" check
		if [check = 'y']
		then
			echo "creating device with $major_number  and $minor_number as major and minor numbers"
			#mknod "/dev/"$1 c $major_number and $minor_number 
		else
			echo "exiting\n"
			
		fi
	fi
else
	echo "enter device name"
	exit 1
fi

file_name=$2
if [ $file_name ]; then
	len=${#file_name}
	start=1
	temp=3
	end=$(($len-$temp))
	test=$(echo $file_name | cut -c $start-$end)
	all_modules=$(lsmod)
	echo $test

	if [[ "$all_modules" == *"$test"* ]]
	then
	    echo "removing $file_name Module"
	    sudo rmmod $file_name 
	    echo "inserting $file_name Module"
	    sudo insmod $file_name 
	else
	    echo "inserting $file_name Module"
	    sudo insmod $file_name 
	fi
	dmesg -c
else
	echo "enter the module name"
	exit 1
fi
