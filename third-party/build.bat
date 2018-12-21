#
# Variables
#
set cwd=%cd%
set botan=%cwd%\botan-2.8.0
set distdir=%cwd%\..\picsou\third-party\build
#
# Script
#
mkdir %distdir%
cd %botan%
python.exe configure.py --cc=gcc --os=mingw --prefix=%distdir%
mingw32-make.exe -j4
mingw32-make.exe install
cd %pwd%
