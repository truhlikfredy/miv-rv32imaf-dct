#!/bin/bash

echo "Will kill all existing OpenOCD and fpServer instances"

function kill_all_existing {
    PIDS=`pidof $1`
    if [ "$PIDS" ];
    then
        # kill it even from other users, as only instance of OpenOCD can run at the same time
        # make sure you do not run these scripts in paraler 

        sudo kill $PIDS 
    fi
}

echo "Making sure no OpenOCD/fpServer is running"

kill_all_existing openocd
sleep 2

kill_all_existing fpServer
sleep 2
