configure:
    cmake -B build -GNinja

build: configure
    glslangValidator shader.vert
    glslangValidator shader.frag
    cmake --build build

run: build
    ./build/opengl-template
