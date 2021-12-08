@echo off

echo;
echo =========================================
echo Build YaizuSampleAgent
echo =========================================

if defined GITHUBACTIONS (
  echo For GitHub Actions
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
if exist agent rmdir /S /Q agent
if exist deployment\servalagt-1.0.0.msi del deployment\servalagt-1.0.0.msi


rem ########## Building ##########
echo;
echo Building servalagt.sln...
%MSBUILD% "..\src\agent\servalagt.sln" /t:clean;build /p:Configuration=Release /p:platform="x64"
echo Building agentsvc.sln...
%MSBUILD% "..\src\agentsvc\agentsvc.sln" /t:clean;build /p:Configuration=Release /p:platform="x64"


rem ########## Checking file existence ##########
echo;
echo Checking "servalagt.exe" existence...
if not exist "..\src\agent\x64\Release\servalagt.exe" goto ERRORRAISED
echo Checking "agentsvc.exe" existence...
if not exist "..\src\agentsvc\x64\Release\agentsvc.exe" goto ERRORRAISED
echo Checking "servalagt.conf" existence...
if not exist "..\src\agent\servalagt.conf" goto ERRORRAISED
echo Checking "svcadd.bat" existence...
if not exist "..\src\agentsvc\svcadd.bat" goto ERRORRAISED
echo Checking "svcdel.bat" existence...
if not exist "..\src\agentsvc\svcdel.bat" goto ERRORRAISED


rem ########## Deployment of files and folders ##########
echo;
echo Deployment of files and folders...

mkdir agent
copy "..\src\agent\x64\Release\servalagt.exe" agent
copy "..\src\agentsvc\x64\Release\agentsvc.exe" agent
copy "..\src\agent\servalagt.conf" agent
copy "..\src\agentsvc\svcadd.bat" agent
copy "..\src\agentsvc\svcdel.bat" agent


rem ########## Making installer ##########
if defined LOCALMACHINE (
  echo;
  echo Making installer...
  %DEVENV% "setup_agt\setup_agt.sln" /rebuild Release
  if not exist deployment mkdir deployment
  copy setup_agt\Release\servalagt-1.0.0.msi deployment
)


rem ########## build complete ##########
if defined LOCALMACHINE (
  echo;
  %LCOUNTER% ..\src /subdir
)

echo;
echo All building processes of YaizuSampleAgent have been successfully finished.
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

