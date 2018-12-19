# Picsou

Keep track of your income and expenses.

**Note: as long as you are not using a release (tagged commit), 
        it might not build or the app might be unstable.**
        
Releases can be found [here](https://github.com/koromodako/picsou/releases).

## Build

### Linux 

Successfully built using the following configuration :

 + Ubuntu 18.10 LTS
 + Clang 6.0.0
 + Qt 5.12.0
 + libgcrypt 1.8.1
 
### Windows

Not tested yet. It should be OK as long as Qt and libgcrypt are both 
cross-platform libraries.

[KeepassXC](https://github.com/keepassxreboot/keepassxc) also uses these 
dependencies and builds successfully on Windows.

## Getting started

Start the app and follow your instinct. A short user documentation might be
helpful I suppose.

## Security

I plan on adding encryption features (that's why libgcrypt is already linked) 
yet they will be implemented when I have time for this.
