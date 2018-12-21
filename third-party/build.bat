#
# Variables
#
set build=%cd%\build
set botan=%cd%\botan-2.8.0
#
# Script
#
del /s /f /q %build%
mkdir %build%
cd %botan%
python.exe configure.py --cc=gcc --os=mingw --prefix=%build%
mingw32-make.exe
mingw32-make.exe install
cd ..
