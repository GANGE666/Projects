#! /bin/bash

echo "Install keyboard driver"

insmod mylock/driver/mega8.ko
mknod /dev/Mega8-kbd c 252 0

insmod mylock/driver/s3c2410-dc-motor.ko
insmod mylock/driver/s3c2410-led.ko

