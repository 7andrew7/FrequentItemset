
## One-time setup
./fetch_dependencies.sh

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
