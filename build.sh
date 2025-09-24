WORKSPACE="$(cd "$(dirname "$0")" && pwd)"

mkdir -p $WORKSPACE/build
cd $WORKSPACE/build
cmake $WORKSPACE/src
if cmake --build .; then
    cd $WORKSPACE
    bash $WORKSPACE/test/run-test.sh
fi