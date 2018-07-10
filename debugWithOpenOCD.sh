#!/bin/bash

SC_DIR="${SC_DIR:-/mnt/bayShareExt/opt/Microsemi/scLinux-5.3-20180705-191129/}"

PROJECT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"


$PROJECT_DIR/killOpenOCD.sh

$SC_DIR/openocd/bin/openocd -f board/microsemi-riscv.cfg -c "echo openocd-started" &
sleep 4 # give enough time for hardware to init
echo "OpenOCD should be launched, start the gdb now:"

cd $PROJECT_DIR/Debug
$SC_DIR/riscv-unknown-elf-gcc/bin/riscv64-unknown-elf-gdb -x ../gdb-tests/gdb-test-looped-checksum miv-rv32imaf-dct.elf miv-rv32imaf-dct.elf
RESULT=$? # Store the exit code

$PROJECT_DIR/killOpenOCD.sh

# Cascade the gdb pass exit code out as a pass and all others as fails
echo "Got $RESULT"
if [ $RESULT == 0xc1054a86 ];
then
    echo "Exiting as PASS"
    exit 0
else
    echo "Exiting as FAILURE"
    exit -1
fi
