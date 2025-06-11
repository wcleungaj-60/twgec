mkdir -p ./build
cd ./build
cmake ../src
cmake --build .
cd ..
./test/run-test.sh