#!/bin/bash

# RamDisk - Insert and test script
#
# Andreas Linz & Tobias Sekan
# andreas.linz@stud.htwk-leipzig.de
# Systemprogrammierung -- WS 2012/13
# HTWK

mountdir=/mnt/fs
devfile=/dev/RamDisk_a
modname=ramdisk
filesys=simplefs
disksize=64


echo "Creating mount directory..."
sudo mkdir $mountdir

echo "Insert module ramdisk.ko..."
sudo insmod ramdisk.ko user_disk_size=$disksize

echo "Creating filesystem \""$filesys"\" on ramdisk"
mkfs-simplefs $devfile

echo "Mounting ramdisk in "$mountdir
sudo mount -t $filesys $devfile $mountdir

echo "Changing permissions of "$mountdir
sudo chmod a+wrx $mountdir

echo "Goodbye and have a nice day!"
