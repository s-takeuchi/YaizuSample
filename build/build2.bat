@echo off

echo;
echo =========================================
echo Build YaizuSampleAgent
echo =========================================

if defined APPVEYOR (
  set MSBUILD="msbuild.exe"
  set DEVENV="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.exe"
  set SEVENZIP="7z.exe"
  set LCOUNTER=""
)

if not defined APPVEYOR (
  set MSBUILD="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\msbuild.exe"
  set DEVENV="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.exe"
  set SEVENZIP="C:\Program Files\7-Zip\7z.exe"
  set LCOUNTER="C:\Program Files (x86)\lcounter\lcounter.exe"
)

if not defined APPVEYOR (
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
)


rem ########## Initializing ##########
echo;
echo Initializing...
if exist agent rmdir /S /Q agent
if exist deployment rmdir /S /Q deployment


rem ########## Building ##########
echo;
echo Building agent.sln...
%DEVENV% "..\src\agent\agent.sln" /rebuild Release 


rem ########## Checking file existence ##########
echo;
echo Checking "agent.exe" existence...
if not exist "..\src\agent\Release\agent.exe" goto ERRORRAISED
echo Checking "agent.conf" existence...
if not exist "..\src\agent\agent.conf" goto ERRORRAISED


rem ########## Deployment of files and folders ##########
echo;
echo Deployment of files and folders...

mkdir agent
copy "..\src\agent\Release\agent.exe" agent
copy "..\src\agent\agent.conf" agent


rem ########## Making installer ##########
echo;
echo Making installer...
rem %DEVENV% "setup\setup.sln" /rebuild Release
mkdir deployment
rem copy setup\Release\stkwebapp.msi deployment


rem ########## build complete ##########
if not defined APPVEYOR (
  echo;
  %LCOUNTER% ..\src /subdir
)
echo;
echo All building processes of YaizuSampleAgent have been successfully finished.
if not defined APPVEYOR (
  pause
)
exit /B %ERRORLEVEL%


rem ########## Error ##########
:ERRORRAISED
echo;
echo Build error.
if not defined APPVEYOR (
  pause
)
exit /B 1

