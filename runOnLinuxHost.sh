#!/bin/sh

mkdir -p ./Debug 
rm Debug/host.elf >/dev/null 2>&1 
gcc -std=c11 -lm src/main.c src/data.c -o Debug/host.elf
chmod a+x Debug/host.elf
./Debug/host.elf
