@echo off

echo;
echo =========================================
echo Build YaizuSample
echo =========================================

if defined APPVEYOR (
  set MSBUILD="msbuild.exe"
  set DEVENV="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.com"
  set SEVENZIP="7z.exe"
  set LCOUNTER=""
  goto definitionend
)

if defined GITHUBACTIONS (
  echo For AppVeyor
  set MSBUILD="C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\MSBuild\15.0\Bin\msbuild.exe"
  set DEVENV="C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\Common7\IDE\devenv.com"
  set SEVENZIP="7z.exe"
  set LCOUNTER=""
  goto definitionend
)

set LOCALMACHINE="true"

set MSBUILD="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\msbuild.exe"
set DEVENV="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.com"
set SEVENZIP="C:\Program Files\7-Zip\7z.exe"
set LCOUNTER="C:\Program Files (x86)\lcounter\lcounter.exe"

echo;
echo This batch file requires softwares shown below.
echo 1. Microsoft Visual Studio 2017
echo 2. 7-Zip 9.20
echo 3. Line Counter

if not exist %MSBUILD% (
  exit
) else if not exist %DEVENV% (
  exit
) else if not exist %SEVENZIP% (
  exit
) else if not exist %LCOUNTER% (
  exit
)

:definitionend


rem ########## Initializing ##########
echo;
echo Initializing...
if exist sample rmdir /S /Q sample
if exist deployment\YaizuSample.msi del deployment\YaizuSample.msi


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
echo Checking "sample.exe" existence...
if not exist "..\src\sample\Release\sample.exe" goto ERRORRAISED
echo Checking "stkdatagui.exe" existence...
if not exist "..\..\YaizuComLib\src\stkdatagui\Release\stkdatagui.exe" goto ERRORRAISED
echo Checking "stkwebappcmd.exe" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\Release\stkwebappcmd.exe" goto ERRORRAISED
echo Checking "nginx-1.12.2.zip" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\nginx-1.12.2.zip" goto ERRORRAISED
echo Checking "jquery-3.2.0.min.js" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\jquery-3.2.0.min.js" goto ERRORRAISED
echo Checking "bootstrap-3.3.7-dist.zip" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\bootstrap-3.3.7-dist.zip" goto ERRORRAISED
echo Checking "stkcommon.js" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\stkcommon.js" goto ERRORRAISED
echo Checking "stkwebappcmd.conf" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\stkwebappcmd.conf" goto ERRORRAISED
echo Checking "sample.dat" existence...
if not exist "..\src\sample\sample.dat" goto ERRORRAISED
echo Checking "sample.conf" existence...
if not exist "..\src\sample\sample.conf" goto ERRORRAISED
echo Checking "sample.html" existence...
if not exist "..\src\sample\sample.html" goto ERRORRAISED


rem ########## Deployment of files and folders ##########
echo;
echo Deployment of files and folders...

mkdir sample
copy "..\src\sample\Release\sample.exe" sample
copy "..\src\sample\sample.dat" sample
copy "..\src\sample\sample.conf" sample
copy "..\..\YaizuComLib\src\stkwebapp\stkwebappcmd.conf" sample
copy "..\..\YaizuComLib\src\stkdatagui\Release\stkdatagui.exe" sample
copy "..\..\YaizuComLib\src\stkwebapp\Release\stkwebappcmd.exe" sample

mkdir sample\nginx
copy "..\..\YaizuComLib\src\stkwebapp\nginx-1.12.2.zip" sample\nginx
pushd sample\nginx
%SEVENZIP% x "nginx-1.12.2.zip"
popd
xcopy /y /q /i /s /e "sample\nginx\nginx-1.12.2" sample
if exist sample\nginx rmdir /S /Q sample\nginx

mkdir sample\bootstrap
copy "..\..\YaizuComLib\src\stkwebapp\bootstrap-3.3.7-dist.zip" sample\bootstrap
pushd sample\bootstrap
%SEVENZIP% x "bootstrap-3.3.7-dist.zip"
popd
xcopy /y /q /i /s /e "sample\bootstrap\bootstrap-3.3.7-dist" sample\html\bootstrap-3.3.7-dist
if exist sample\bootstrap rmdir /S /Q sample\bootstrap

copy "..\src\sample\sample.html" sample\html
copy "..\..\YaizuComLib\src\stkwebapp\stkcommon.js" sample\html
copy "..\..\YaizuComLib\src\stkwebapp\jquery-3.2.0.min.js" sample\html


rem ########## Making installer ##########
echo;
echo Making installer...
%DEVENV% "setup\setup.sln" /rebuild Release
if not exist deployment mkdir deployment
copy setup\Release\YaizuSample.msi deployment


rem ########## build complete ##########
if defined LOCALMACHINE (
  echo;
  %LCOUNTER% ..\src /subdir
)
echo;
echo All building processes of YaizuSample have been successfully finished.
if defined LOCALMACHINE (
  pause
)
exit /B %ERRORLEVEL%


rem ########## Error ##########
:ERRORRAISED
echo;
echo Build error.
if defined LOCALMACHINE (
  pause
)
exit /B 1

