#!/bin/bash

SC_DIR="${SC_DIR:-/mnt/bayShareExt/opt/Microsemi/scLinux-5.3-20180705-191129/}"

PROJECT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"


cd $SC_DIR/openocd/bin/
#rm openocd-started

kill `pidof openocd`

./openocd -f board/microsemi-riscv.cfg -c "echo openocd-started" &
sleep 3 # give enough time for hardware to init
echo 

cd $PROJECT_DIR/Debug
$SC_DIR/riscv-unknown-elf-gcc/bin/riscv64-unknown-elf-gdb -x ../gdb-tests/gdb-test-looped-checksum miv-rv32imaf-dct.elf miv-rv32imaf-dct.elf
RESULT=$? # Store the exit code

echo "killing openocd:"
kill `pidof openocd`
sleep 3
echo "done."

# Cascade the exit code out
echo "Exiting with $RESULT ( 0=pass 1=fail )"
exit $RESULT


