#!/bin/sh

# compile translations
lrelease resources/lcd-image-converter-ru.ts resources/lcd-image-converter-ru.qm

# generate version info
./version-gen.sh

qmake
make

