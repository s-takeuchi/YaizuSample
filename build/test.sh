#!/bin/bash -x

# Make build folder
BUILDDIR=$(pwd)/server
if [ -e $BUILDDIR ]; then
    yes|rm -r $BUILDDIR
fi
mkdir -p $BUILDDIR


# Build
cd ../src/sample
make all
cp serval $BUILDDIR
cp sample.conf $BUILDDIR
cp sample.dat $BUILDDIR
echo servicehost=localhost >> $BUILDDIR/sample.conf
echo serviceport=10009 >> $BUILDDIR/sample.conf
cd ../../test/sampletest
make all
cp sampletest $BUILDDIR


# Testing
cd $BUILDDIR
echo
echo Test starts
./serval&
./sampletest
echo Test ends
