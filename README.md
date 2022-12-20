# build:

mkdir build

cd build

cmake ..

make main

make check_cut

cd ..

python3 test.py
