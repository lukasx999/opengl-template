configure:
    cmake -B build -GNinja

build: configure
    cmake --build build

run: build
    ./build/proj
