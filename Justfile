run: build
    ./build/opengl-template

build: configure
    cmake --build build

configure:
    cmake -B build -GNinja
