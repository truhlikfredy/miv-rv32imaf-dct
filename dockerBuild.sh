#!/bin/bash

# get project directory no matter what the current directory is
PROJECT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
HOST_UID=`id -u`
HOST_GID=`id -g`

$PROJECT_DIR/clean.sh

echo "Building the $PROJECT_DIR project"

# --rm remove the container after finished
# --volume will mount a local folder to the docker container

docker run --rm --volume $PROJECT_DIR:/project antonkrug/softconsole-5-3-slim:3056bfb bash -c "sch --drop=$HOST_UID:$HOST_GID -b=.+/Debug"

