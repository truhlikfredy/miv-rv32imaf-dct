# get project directory no matter what the current directory is
PROJECT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo "Clean the $PROJECT_DIR Debug/Release folders"
rm -fr $PROJECT_DIR/Debug-hardfloat >/dev/null 2>&1
rm -fr $PROJECT_DIR/Debug-softfloat >/dev/null 2>&1
rm -fr $PROJECT_DIR/Release >/dev/null 2>&1
