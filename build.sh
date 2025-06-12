WORKSPACE="$(cd "$(dirname "$0")" && pwd)"

mkdir -p $WORKSPACE/build
cd $WORKSPACE/build
cmake $WORKSPACE/src
cmake --build .
cd $WORKSPACE
bash $WORKSPACE/test/run-test.sh