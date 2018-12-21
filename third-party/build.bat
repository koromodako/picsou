#
# Variables
#
set pwd=%cd%
set build=%pwd%\build
set botan=%pwd%\botan-2.8.0
set picbuild=%pwd%\..\picsou\third-party\build
#
# Script
#
del /s /f /q %build%
mkdir %build%
cd %botan%
python.exe configure.py --cc=gcc --os=mingw --prefix=%build%
mingw32-make.exe
mingw32-make.exe install
cd %pwd%
mkdir %picbuild%
xcopy /s/e %build% %picbuild%
