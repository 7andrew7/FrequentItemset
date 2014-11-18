
## One-time setup

## Debug builds

mkdir debug
cd debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

## Release builds

mkdir release
cd release
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
make
