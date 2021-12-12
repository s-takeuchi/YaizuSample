#!/bin/bash

echo
echo .tar.gz packing...
cd deployment

echo Checking "svl100.tar.gz" existence...
if [ -e svl100.tar.gz ];then
  rm svl100.tar.gz
fi
echo Checking "serval-1.0-0.el8.x86_64.rpm" existence...
if [ ! -e serval-1.0-0.el8.x86_64.rpm ]; then
  exit
fi
echo Checking "servalagt-1.0.0.msi" existence...
if [ ! -e servalagt-1.0.0.msi ]; then
  exit
fi
echo Checking "servalagt-1.0-0.el8.x86_64.rpm" existence...
if [ ! -e servalagt-1.0-0.el8.x86_64.rpm ]; then
  exit
fi
echo Checking "ReadmeENG.html" existence...
if [ ! -e ../../doc/readme/ReadmeENG.html ];then
  exit
fi
echo Checking "ReadmeJPN.html" existence...
if [ ! -e ../../doc/readme/ReadmeJPN.html ];then
  exit
fi

cp ../../doc/readme/ReadmeENG.html .
cp ../../doc/readme/ReadmeJPN.html .

tar -cvf svl100.tar serval-1.0-0.el8.x86_64.rpm \
  servalagt-1.0.0.msi \
  servalagt-1.0-0.el8.x86_64.rpm \
  ReadmeENG.html \
  ReadmeJPN.html

gzip svl100.tar

rm ReadmeENG.html
rm ReadmeJPN.html
 
