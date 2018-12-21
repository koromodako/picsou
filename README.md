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

### Linux 

Successfully built using the following configuration :

 + Ubuntu 18.10 LTS
 + GCC 8
 + Qt 5.12.0
 + botan 2.8.0
 
### Windows

It should be OK as long as Qt and botan are both cross-platform libraries. 

**Currently working on it.**

## Contributions

Feel free to make pull requests for bugfixes, enhancements and whatever comes to your mind that seems relevant regarding this project.

And do not forget to add your pseudo in the following list !

**Contributors:**
 - [koromodako](https://github.com/koromodako)
