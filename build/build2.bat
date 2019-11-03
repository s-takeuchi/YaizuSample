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
  goto definitionend
)

if defined GITHUBACTIONS (
  echo For AppVeyor
  set MSBUILD="C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\MSBuild\15.0\Bin\msbuild.exe"
  set DEVENV="C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\Common7\IDE\devenv.exe"
  set SEVENZIP="7z.exe"
  set LCOUNTER=""
  goto definitionend
)

set LOCALMACHINE="true"

set MSBUILD="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\msbuild.exe"
set DEVENV="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.exe"
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


rem ########## Test for GitHub Actions ##########
pwd
dir c:\
dir "C:\Program Files"
dir "C:\Program Files (x86)\Microsoft Visual Studio\2017"
dir "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise"
dir "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\Common7"
dir "C:\Program Files (x86)\Microsoft Visual Studio\2017\Enterprise\Common7\IDE"
dir setup2
where /r "C:\Program Files (x86)" devenv.exe
set


rem ########## Initializing ##########
echo;
echo Initializing...
if exist agent rmdir /S /Q agent
if exist deployment\YaizuSampleAgent.msi del deployment\YaizuSampleAgent.msi


rem ########## Building ##########
echo;
echo Building agent.sln...
%MSBUILD% "..\src\agent\agent.sln" /t:clean;build /p:Configuration=Release
echo Building agentsvc.sln...
%MSBUILD% "..\src\agentsvc\agentsvc.sln" /t:clean;build /p:Configuration=Release


rem ########## Checking file existence ##########
echo;
echo Checking "agent.exe" existence...
if not exist "..\src\agent\Release\agent.exe" goto ERRORRAISED
echo Checking "agentsvc.exe" existence...
if not exist "..\src\agentsvc\Release\agentsvc.exe" goto ERRORRAISED
echo Checking "agent.conf" existence...
if not exist "..\src\agent\agent.conf" goto ERRORRAISED
echo Checking "svcadd.bat" existence...
if not exist "..\src\agentsvc\svcadd.bat" goto ERRORRAISED
echo Checking "svcdel.bat" existence...
if not exist "..\src\agentsvc\svcdel.bat" goto ERRORRAISED


rem ########## Deployment of files and folders ##########
echo;
echo Deployment of files and folders...

mkdir agent
copy "..\src\agent\Release\agent.exe" agent
copy "..\src\agentsvc\Release\agentsvc.exe" agent
copy "..\src\agent\agent.conf" agent
copy "..\src\agentsvc\svcadd.bat" agent
copy "..\src\agentsvc\svcdel.bat" agent


rem ########## Making installer ##########
echo;
echo Making installer...
%DEVENV% "setup2\setup2.sln" /rebuild Release
if not exist deployment mkdir deployment
copy setup2\Release\YaizuSampleAgent.msi deployment


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

