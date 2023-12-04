#!/bin/bash

# Execute the first command
sudo rmmod uas
# Check if the command was successful
if [ $? -ne 0 ]; then
    echo "Error in removing uas module"
fi

# Execute the second command
sudo rmmod usb_storage
# Check if the command was successful
if [ $? -ne 0 ]; then
    echo "Error in removing usb_storage module"
fi

# Execute the third command
sudo insmod kern_kill.ko
# Check if the command was successful
if [ $? -ne 0 ]; then
    echo "Error in inserting kern_kill module"
fi
