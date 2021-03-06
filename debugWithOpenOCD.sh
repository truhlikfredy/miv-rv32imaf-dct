#!/bin/bash

SC_DIR="${SC_DIR:-/opt/microsemi/softconsole/}"

PROJECT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"


$PROJECT_DIR/killOpenOCD.sh

$SC_DIR/openocd/bin/openocd -f board/microsemi-riscv.cfg -c "echo openocd-started" &
sleep 4 # give enough time for hardware to init


# Start GDB
echo "OpenOCD should be launched, start the gdb now. Go into the first Debug"
echo "folder I can find (make sure to run clean before this so there are no"
echo "other folders present). One of these below should success:"
ls -la
cd $PROJECT_DIR/Debug*
cd $PROJECT_DIR/Release*
ls -la

$SC_DIR/riscv-unknown-elf-gcc/bin/riscv64-unknown-elf-gdb -x ../gdb-tests/gdb-test-looped-checksum *.elf

# Store the exit code
RESULT=$? 

# Kill OpenOCD first 
$PROJECT_DIR/killOpenOCD.sh

# Cascade the gdb pass exit code out as a pass and all others as fails
echo "Got $RESULT"
if [ $RESULT == 149 ];
then
    echo "Exiting as PASS"
    exit 0
fi

if [ $RESULT == 150 ];
then
    echo "Exiting as FAILUER on test failed (the checksum didn't matched)"
    exit -1
else
    echo "Exiting as FAILURE on the process failed (openocd/build/gdb)"
    exit -1
fi
