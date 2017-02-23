@echo off

echo;
echo =========================================
echo Build YaizuSample
echo =========================================

set CURRENTPATH=%cd%
set DEVENV="C:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\IDE\devenv.exe"
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
if not exist "..\..\YaizuComLib\src\stkwebapp\etc\nginx-1.10.3.zip" goto FILENOTEXIST



rem ########## Deployment of files and folders ##########
echo;
echo Deployment of files and folders...

mkdir sample
copy "..\src\sample\Release\stkwebapp.exe" sample
copy "..\..\YaizuComLib\src\stkdatagui\Release\stkdatagui.exe" sample
copy "..\..\YaizuComLib\src\stkwebapp\Release\stkwebappcmd.exe" sample

mkdir sample\nginx
copy "..\..\YaizuComLib\src\stkwebapp\etc\nginx-1.10.3.zip" sample\nginx
pushd sample\nginx
%SEVENZIP% x "nginx-1.10.3.zip"
popd
xcopy /y /q /i /s /e "sample\nginx\nginx-1.10.3" sample
if exist sample\nginx rmdir /S /Q sample\nginx

pause
exit



rem ########## Making installer ##########
echo;
echo Making installer...
%DEVENV% "setup\cmdfreak.sln" /rebuild Release
mkdir deployment
copy setup\Release\cmdfreak.msi deployment



rem ########## ZIP packing ##########
echo;
echo ZIP packing stage...
copy ..\doc\readme\ReadmeJPN.txt deployment
copy ..\doc\readme\ReadmeENG.txt deployment
cd deployment
%SEVENZIP% a cfk100.zip cmdfreak.msi
%SEVENZIP% a cfk100.zip ReadmeENG.txt
%SEVENZIP% a cfk100.zip ReadmeJPN.txt
del ReadmeENG.txt
del ReadmeJPN.txt
del cmdfreak.msi
cd..



rem ########## build complete ##########
echo;
%LCOUNTER% ..\src /subdir
echo;
echo All building processes of CmdFreak have been successfully finished.
pause
exit /B



rem ########## Error ##########
:FILENOTEXIST
echo;
echo Build error occurred because some build target files do not exist.
pause
exit /B

