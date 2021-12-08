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
cp serval.conf $BUILDDIR
cp serval.dat $BUILDDIR
echo servicehost=localhost >> $BUILDDIR/serval.conf
echo serviceport=10009 >> $BUILDDIR/serval.conf
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
