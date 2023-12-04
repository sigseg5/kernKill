#!/bin/bash

sudo rmmod kern_kill
if [ $? -ne 0 ]; then
    echo "Error in removing kern_kill module"
fi

sudo modprobe uas
if [ $? -ne 0 ]; then
    echo "Error in loading uas module"
fi

sudo modprobe usb_storage
if [ $? -ne 0 ]; then
    echo "Error in loading usb_storage module"
fi
