#!/bin/bash

SC_DIR="${SC_DIR:-/mnt/750/downloads/scLinux-5.3.0.30-20180702-105624/}"

PROJECT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"


cd $SC_DIR/openocd/bin/
rm openocd-started

kill `pidof openocd`

./openocd -f board/microsemi-riscv.cfg -c "echo openocd-started" &
sleep 3 # give enough time for hardware to init
echo 

cd $PROJECT_DIR
$SC_DIR/riscv-unknown-elf-gcc/bin/riscv64-unknown-elf-gdb Debug/miv-rv32imaf-dct.elf

echo "killing openocd:"
kill `pidof openocd`
echo "done."



