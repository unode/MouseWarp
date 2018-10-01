## WIP - MouseWarping guidance overlay

### Compiling

Currently we rely on libaosd to render the overlay.
At the moment libaosd isn't included in the build process so you need to:

    cd libaosd/
    ./autogen.sh
    ./configure
    make
    ln -s libaosd.so libaosd/libaosd/libaosd.so.2

before trying to compile mousewarp using:

    cmake .
    make
