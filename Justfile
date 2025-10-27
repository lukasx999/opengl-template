run: build
    ./build/opengl-template

build: configure
    glslangValidator shader.vert
    glslangValidator shader.frag
    cmake --build build

configure:
    cmake -B build -GNinja
