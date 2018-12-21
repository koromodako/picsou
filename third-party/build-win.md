# Build instructions for Windows

1. Install Visual Studio 2017
2. Extract botan-2.8.0.tar.gz
3. Open PowerShell and perform the following commands:
```
cd C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools

.\LaunchDevCmd.bat

cd C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build

.\vcvarsall.bat x86_amd64

cd C:\Users\Public\picsou\third-party\botan-2.8.0

python.exe configure.py --cc=msvc --os=windows --prefix=C:\Users\Public\picsou\third-party\build

nmake

nmake install
```
4. It should be ok for Botan !
