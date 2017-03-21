@echo off

echo Setting up a Qt environment...

set QMAKESPEC=win32-msvc2015
echo -- QMAKESPEC set to "win32-msvc2015"

if exist env.cmd (
	echo "environment set, using env.cmd."
	call env.cmd 
) else (
	echo "environment not set, using default."
	set QT_DIR="C:\Qt\Qt5.8.0\5.8\msvc2015"
	set VS_DIR="C:\Program Files (x86)\Microsoft Visual Studio 14.0"
	set EVB_DIR="C:\Program Files (x86)\Enigma Virtual Box"
)
echo -- QT_DIR set to %QT_DIR%
echo -- VS_DIR set to %VS_DIR%
echo -- EVB_DIR set to %EVB_DIR%

set PATH=%QT_DIR%\bin;%EVB_DIR%;%PATH%
echo -- Added %QT_DIR%\bin;%EVB_DIR% to PATH

echo Setting up a VS environment...
call %VS_DIR%\VC\vcvarsall.bat x86


set SRC=%cd%\..
echo --SRC set to %SRC%

mkdir temp
mkdir build
cd temp

qmake %SRC%\PhotoSticker.pro
nmake release /nologo

cd ..
copy temp\release\PhotoSticker.exe build
rd /s/q temp
cd build

windeployqt --no-translations --no-compiler-runtime --no-angle --no-opengl-sw PhotoSticker.exe
cd ..
enigmavbconsole setup.evb
rd /s/q build