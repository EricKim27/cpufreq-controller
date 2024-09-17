# About

This module is made to control the max and the minimal cpu frequency the hard way by making a kernel module for it, instead of using the existing interface in the sysfs.

This is made in order to learn how sysfs works.

# How this module is going to work

This module would make two entries in /sys/kernel/freqcon directory.

- /sys/kernel/freqcon/max
- /sys/kernel/freqcon/min

These two entries are going to be used to control
the CPU's max/minimal frequency at all cores.
 
It's important to note that the making of this module is solely for educational purposes.
