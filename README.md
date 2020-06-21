# Picsou

*Keep track of your income and expenses.*
        
Releases can be found [here](https://github.com/koromodako/picsou/releases).

**Warning: as long as you are not using a release (tagged commit), it might not build or the app might be unstable.**

## Getting Started

1. Start the app and follow your instinct. 
2. A short user documentation might be helpful I suppose.
3. Some actions are not explicitly acknowleged: if you perform an action and you don't see an error message, it means what you did seems ok.
4. Know that this project lacks tests, even if I use it twice a month.

## Security Considerations

Encryption features have been implemented to protect user's data at rest. 

**If well implemented**, in-memory secrets should be protected to.

I tried to conform to *Option 2a (Figure 3)* of [NIST publication 800-132](https://csrc.nist.gov/publications/detail/sp/800-132/final)

## Build

You can build Picsou following two steps. These steps are platform-independent as long as you're using one 
 1. Build Botan
 2. Build Picsou

Build process does not cover dependencies installation.

### Linux

Successfully built using the following configuration:

 + Ubuntu 20.04 LTS
 + GCC 9.3.0
 + Qt 5.15.0
 + Botan 2.14.0

**Build process:**

```bash
git clone https://github.com/koromodako/picsou
cd picsou
# 1. Build Botan
cd third-party
./build.sh
# 2. Build Picsou
mkdir /tmp/build && cd /tmp/build
/path/to/Qt/5.15.0/gcc_64/bin/qmake /path/to/picsou/picsou/picsou.pro
make -j4
```

Picsou should be available in `/path/to/picsou/dist/release`.

**Note:**

It should build on any Linux distribution (providing appropriate dependencies are available).
 
### Windows

Successfully built using the following configuration:

 + Windows 10
 + MinGW 7.3.0 64-bit
 + Qt 5.15.0
 + Botan 2.14.0

**Process:**

*Warning: Ensure that Qt's MinGW location is part of your PATH before building Botan !*

 1. Start a PowerShell command line go to `third-party/` and run `build.ps1`.
 2. Open `picsou/picsou.pro` with QtCreator, configure the project and build.

**Note:**

I tried MSVC and the build process succeeded yet execution failed (`std::bad_alloc`) within Botan according to stacktrace... Strange.

## Contributions

Feel free to make pull requests for bugfixes, enhancements and whatever comes to your mind that seems relevant regarding this project.

And do not forget to add your pseudo in the following list !

**Contributors:**
 - [koromodako](https://github.com/koromodako)
