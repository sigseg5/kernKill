#!/bin/bash

# Execute the first command
sudo rmmod kern_kill
# Check if the command was successful
if [ $? -ne 0 ]; then
    echo "Error in removing kern_kill module"
fi

# Execute the second command
sudo modprobe uas
# Check if the command was successful
if [ $? -ne 0 ]; then
    echo "Error in loading uas module"
fi

# Execute the third command
sudo modprobe usb_storage
# Check if the command was successful
if [ $? -ne 0 ]; then
    echo "Error in loading usb_storage module"
fi
