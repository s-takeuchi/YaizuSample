@echo off

echo;
echo =========================================
echo Build YaizuSample
echo =========================================

if defined GITHUBACTIONS (
  echo For GitHub Actions
  set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\msbuild.exe"
  set DEVENV="C:\Program Files\Microsoft Visual Studio\2022\Enterprise\Common7\IDE\devenv.com"
  set SEVENZIP="7z.exe"
  set LCOUNTER=""
  goto definitionend
)

set LOCALMACHINE="true"

set MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\msbuild.exe"
set DEVENV="C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.com"
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
if exist server rmdir /S /Q server
if exist deployment\serval-1.0.0.msi del deployment\serval-1.0.0.msi


rem ########## Building ##########
echo;
echo Building serval.sln...
%DEVENV% "..\src\sample\serval.sln" /rebuild "Release|x64"
echo Building stkdatagui.sln...
%DEVENV% "..\..\YaizuComLib\src\stkdatagui\stkdatagui.sln" /rebuild "Release|x64"
echo Building stkwebappcmd.sln...
%DEVENV% "..\..\YaizuComLib\src\stkwebapp\stkwebappcmd.sln" /rebuild "Release|x64"


rem ########## Checking file existence ##########
echo;
echo Checking "serval.exe" existence...
if not exist "..\src\sample\x64\Release\serval.exe" goto ERRORRAISED
echo Checking "stkdatagui.exe" existence...
if not exist "..\..\YaizuComLib\src\stkdatagui\x64\Release\stkdatagui.exe" goto ERRORRAISED
echo Checking "stkwebappcmd.exe" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\x64\Release\stkwebappcmd.exe" goto ERRORRAISED
echo Checking "nginx-1.12.2.zip" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\nginx-1.12.2.zip" goto ERRORRAISED
echo Checking "jquery-3.2.0.min.js" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\jquery-3.2.0.min.js" goto ERRORRAISED
echo Checking "IcoMoon-Free.ttf" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\IcoMoon-Free.ttf" goto ERRORRAISED
echo Checking "bootstrap-4.4.1-dist.zip" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\bootstrap-4.4.1-dist.zip" goto ERRORRAISED
echo Checking "stkcommon.js" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\stkcommon.js" goto ERRORRAISED
echo Checking "stkcommon_um.js" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp_um\stkcommon_um.js" goto ERRORRAISED
echo Checking "stkcommon.css" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\stkcommon.css" goto ERRORRAISED
echo Checking "stkwebappcmd.conf" existence...
if not exist "..\..\YaizuComLib\src\stkwebapp\stkwebappcmd.conf" goto ERRORRAISED
echo Checking "serval.dat" existence...
if not exist "..\src\sample\serval.dat" goto ERRORRAISED
echo Checking "serval.conf" existence...
if not exist "..\src\sample\serval.conf" goto ERRORRAISED
echo Checking "serval.html" existence...
if not exist "..\src\sample\serval.html" goto ERRORRAISED
echo Checking "serval.js" existence...
if not exist "..\src\sample\serval.js" goto ERRORRAISED
echo Checking "squirrel.svg" existence...
if not exist "..\src\sample\squirrel.svg" goto ERRORRAISED
echo Checking "favicon-16x16.png" existence...
if not exist "..\src\sample\favicon-16x16.png" goto ERRORRAISED


rem ########## Deployment of files and folders ##########
echo;
echo Deployment of files and folders...

mkdir server
copy "..\src\sample\x64\Release\serval.exe" server
copy "..\src\sample\serval.dat" server
copy "..\src\sample\serval.conf" server
copy "..\..\YaizuComLib\src\stkwebapp\stkwebappcmd.conf" server
copy "..\..\YaizuComLib\src\stkdatagui\x64\Release\stkdatagui.exe" server
copy "..\..\YaizuComLib\src\stkwebapp\x64\Release\stkwebappcmd.exe" server

mkdir server\nginx
copy "..\..\YaizuComLib\src\stkwebapp\nginx-1.12.2.zip" server\nginx
pushd server\nginx
%SEVENZIP% x "nginx-1.12.2.zip"
popd
xcopy /y /q /i /s /e "server\nginx\nginx-1.12.2" server
if exist server\nginx rmdir /S /Q server\nginx

mkdir server\bootstrap
copy "..\..\YaizuComLib\src\stkwebapp\bootstrap-4.4.1-dist.zip" server\bootstrap
pushd server\bootstrap
%SEVENZIP% x "bootstrap-4.4.1-dist.zip"
popd
xcopy /y /q /i /s /e "server\bootstrap\bootstrap-4.4.1-dist" server\html\bootstrap-4.4.1-dist
if exist server\bootstrap rmdir /S /Q server\bootstrap

mkdir server\html\img
copy "..\src\sample\serval.html" server\html
copy "..\src\sample\serval.js" server\html
copy "..\src\sample\squirrel.svg" server\html
copy "..\src\sample\favicon-16x16.png" server\html\img
copy "..\..\YaizuComLib\src\stkwebapp\stkcommon.js" server\html
copy "..\..\YaizuComLib\src\stkwebapp\stkcommon.css" server\html
copy "..\..\YaizuComLib\src\stkwebapp_um\stkcommon_um.js" server\html
copy "..\..\YaizuComLib\src\stkwebapp\jquery-3.2.0.min.js" server\html
copy "..\..\YaizuComLib\src\stkwebapp\IcoMoon-Free.ttf" server\html
copy "..\..\YaizuComLib\src\stkwebapp\IcoMoon-Free.css" server\html


rem ########## Making installer ##########
if defined LOCALMACHINE (
  echo;
  echo Making installer...
  %DEVENV% "setup_svr\setup_svr.sln" /rebuild Release
  if not exist deployment mkdir deployment
  copy setup_svr\Release\serval-1.0.0.msi deployment
)


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

