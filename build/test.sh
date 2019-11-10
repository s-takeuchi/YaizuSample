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
cp sample $BUILDDIR
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
./sample&
./sampletest
echo Test ends
