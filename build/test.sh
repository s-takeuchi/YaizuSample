#!/bin/bash -x

# Make build folder
BUILDDIR=$(pwd)/sample
if [ -e $BUILDDIR ]; then
    yes|rm -r $BUILDDIR
fi
mkdir -p $BUILDDIR


# Build
cd ../src/sample
make all
cp sample $BUILDDIR
cp sample.conf $BUILDDIR
cp sample.dat $BUILDDIR
echo servicehost=127.0.0.1 >> $BUILDDIR/sample.conf
echo serviceport=2070 >> $BUILDDIR/sample.conf
cd ../../test/sampletest
make all
cp sampletest $BUILDDIR


# Testing
cd $BUILDDIR
echo
echo Test starts
./sample&
sleep 3
./sampletest
echo Test ends
