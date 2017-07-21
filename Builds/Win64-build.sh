#!/bin/bash

sudo apt-get install build-essential libtool autotools-dev automake pkg-config bsdmainutils curl
sudo apt-get install g++-mingw-w64-x86-64 mingw-w64-x86-64-dev

cd ..
cd depends
make HOST=x86_64-w64-mingw32 -j 8

cd ..
./autogen.sh
CONFIG_SITE=$PWD/depends/x86_64-w64-mingw32/share/config.site ./configure --prefix=/ --disable-tests
make -j 8 		

exit 0
