WORKSPACE="$(cd "$(dirname "$0")" && pwd)"

mkdir -p $WORKSPACE/build
cd $WORKSPACE/build
cmake $WORKSPACE/src
if cmake --build .; then
    cd $WORKSPACE
    bash $WORKSPACE/test/run-test.sh
    $WORKSPACE/build/twgec --help 2> $WORKSPACE/example/help.txt
fi