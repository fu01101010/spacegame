#! /bin/zsh
cd /users/ulysses/Desktop/source/projects/game
rm -rf build/
mkdir build
cd build
cmake ..
make install
