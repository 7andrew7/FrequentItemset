
## One-time setup
wget https://googletest.googlecode.com/files/gtest-1.7.0.zip
unzip gtest-1.7.0.zip

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
