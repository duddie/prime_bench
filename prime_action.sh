#!/bin/bash
printf "*** START *** \n\n"
system_profiler SPHardwareDataType
uname -a
printf "\nProfiling...\n\n"
for i in 1 2 3 4 5 6 ; do ./prime.elf ${i} ; done
printf "\n*** END ***\n"