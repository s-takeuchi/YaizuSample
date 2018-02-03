@echo off

echo;
echo =========================================
echo Build StkWebApp
echo =========================================

set CURRENTPATH=%cd%
set DEVENV="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.exe"
set SEVENZIP="C:\Program Files\7-Zip\7z.exe"
set LCOUNTER="C:\Program Files (x86)\lcounter\lcounter.exe"

echo;
echo This batch file requires softwares shown below.
echo (1) Microsoft Visual Studio 2008 Professional Edition
echo (2) 7-Zip 9.20
echo (3) Line Counter

if not exist %DEVENV% exit
if not exist %SEVENZIP% exit
if not exist %LCOUNTER% exit



rem ########## Initializing ##########
echo;
echo Initializing...
if exist sample rmdir /S /Q sample
if exist deployment rmdir /S /Q deployment



rem ########## Building ##########
echo;
echo Building sample.sln...
%DEVENV% "..\src\sample\sample.sln" /rebuild Release 
echo Building stkdatagui.sln...
%DEVENV% "..\..\YaizuComLib\src\stkdatagui\stkdatagui.sln" /rebuild Release
echo Building stkwebappcmd.sln...
%DEVENV% "..\..\YaizuComLib\src\stkwebapp\stkwebappcmd.sln" /rebuild Release



rem ########## Checking file existence ##########
echo;
echo Checking "stkwebapp.exe" existence...
if not exist "..\src\sample\Release\stkwebapp.exe" goto FILENOTEXIST
echo Checking "stkdatagui.exe" existence...
if not exist "..\..\YaizuComLib\src\stkdatagui\Release\stkdatagui.exe" goto FILENOTEXIST
echo Checking "stkwebappcmd.exe" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\Release\stkwebappcmd.exe" goto FILENOTEXIST
echo Checking "nginx-1.10.3.zip" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\nginx-1.10.3.zip" goto FILENOTEXIST
echo Checking "jquery-3.2.0.min.js" existence...
if not exist "..\src\sample\jquery-3.2.0.min.js" goto FILENOTEXIST
echo Checking "stkwebapp.conf" existence...
if not exist "..\src\sample\stkwebapp.conf" goto FILENOTEXIST
echo Checking "stkwebapp.dat" existence...
if not exist "..\src\sample\stkwebapp.dat" goto FILENOTEXIST
echo Checking "index.html" existence...
if not exist "..\src\sample\index.html" goto FILENOTEXIST
echo Checking "style.css" existence...
if not exist "..\src\sample\style.css" goto FILENOTEXIST



rem ########## Deployment of files and folders ##########
echo;
echo Deployment of files and folders...

mkdir sample
copy "..\src\sample\Release\stkwebapp.exe" sample
copy "..\src\sample\stkwebapp.conf" sample
copy "..\src\sample\stkwebapp.dat" sample
copy "..\..\YaizuComLib\src\stkdatagui\Release\stkdatagui.exe" sample
copy "..\..\YaizuComLib\src\stkwebapp\Release\stkwebappcmd.exe" sample

mkdir sample\nginx
copy "..\..\YaizuComLib\src\stkwebapp\nginx-1.10.3.zip" sample\nginx
pushd sample\nginx
%SEVENZIP% x "nginx-1.10.3.zip"
popd
xcopy /y /q /i /s /e "sample\nginx\nginx-1.10.3" sample
if exist sample\nginx rmdir /S /Q sample\nginx

copy "..\src\sample\index.html" sample\html
copy "..\src\sample\style.css" sample\html
copy "..\src\sample\jquery-3.2.0.min.js" sample\html



rem ########## Making installer ##########
echo;
echo Making installer...
%DEVENV% "setup\setup.sln" /rebuild Release
mkdir deployment
copy setup\Release\stkwebapp.msi deployment



rem ########## build complete ##########
echo;
%LCOUNTER% ..\src /subdir
echo;
echo All building processes of StkWebApp have been successfully finished.
pause
exit /B



rem ########## Error ##########
:FILENOTEXIST
echo;
echo Build error occurred because some build target files do not exist.
pause
exit /B

