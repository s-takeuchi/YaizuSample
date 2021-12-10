@echo off
set SEVENZIP="C:\Program Files\7-Zip\7z.exe"
echo;
echo ZIP packing...
cd deployment

echo Checking "svl100.zip" existence...
if exist "svl100.zip" del svl100.zip
echo Checking "7z.exe" existence...
if not exist %SEVENZIP% goto SCRIPTEND
echo Checking "serval-1.0.0.msi" existence...
if not exist "serval-1.0.0.msi" goto SCRIPTEND
echo Checking "servalagt-1.0.0.msi" existence...
if not exist "servalagt-1.0.0.msi" goto SCRIPTEND
echo Checking "servalagt-1.0-0.el8.x86_64.rpm" existence...
if not exist "servalagt-1.0-0.el8.x86_64.rpm" goto SCRIPTEND
echo Checking "ReadmeENG.html" existence...
if not exist "..\..\doc\readme\ReadmeENG.html" goto SCRIPTEND
echo Checking "ReadmeJPN.html" existence...
if not exist "..\..\doc\readme\ReadmeJPN.html" goto SCRIPTEND

%SEVENZIP% a svl100.zip serval-1.0.0.msi
%SEVENZIP% a svl100.zip servalagt-1.0.0.msi
%SEVENZIP% a svl100.zip servalagt-1.0-0.el8.x86_64.rpm
%SEVENZIP% a svl100.zip ..\..\doc\readme\ReadmeENG.html
%SEVENZIP% a svl100.zip ..\..\doc\readme\ReadmeJPN.html

:SCRIPTEND
cd..
