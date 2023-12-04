#!/bin/bash

sudo rmmod uas
if [ $? -ne 0 ]; then
    echo "Error in removing uas module"
fi

sudo rmmod usb_storage
if [ $? -ne 0 ]; then
    echo "Error in removing usb_storage module"
fi

sudo insmod kern_kill.ko
if [ $? -ne 0 ]; then
    echo "Error in inserting kern_kill module"
fi
