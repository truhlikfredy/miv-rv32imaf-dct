#!/bin/bash

# get project directory no matter what the current directory is
PROJECT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
HOST_UID=`id -u`
HOST_GID=`id -g`

echo "Clean the $PROJECT_DIR Debug/Release folders"
rm -r $PROJECT_DIR/Debug >/dev/null 2>&1
rm -r $PROJECT_DIR/Release >/dev/null 2>&1

echo "Building the $PROJECT_DIR project"

# --rm remove the container after finished
# --volume will mount a local folder to the docker container

docker run --rm --volume $PROJECT_DIR:/project antonkrug/softconsole-5-3-slim:5400e4d bash -c "sch --drop=$HOST_UID:$HOST_GID"

