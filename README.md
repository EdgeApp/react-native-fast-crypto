# react-native-fast-crypto

This library implements fast, fully native crypto routines for React Native under iOS and Android. Fully built binaries are committed for both platforms but can also be built from scratch.

## Features

- **PBKDF2** - Password-Based Key Derivation Function 2 with SHA-512
- **scrypt** - Memory-hard key derivation function
- **secp256k1** - Elliptic curve operations for Bitcoin/Ethereum
  - Public key creation
  - Private key tweaking (for HD wallets)
  - Public key tweaking

## Getting started

This library directly supports React Native v0.60+ with autolinking. Simply add react-native-fast-crypto to your application, and React Native will link the necessary native modules into your app.

`npm install react-native-fast-crypto --save`

For iOS, you'll also need to install pods:

```bash
cd ios && pod install
```

## Quick Start

```javascript
import { scrypt, pbkdf2, secp256k1 } from 'react-native-fast-crypto'

// Hash a password with scrypt
const password = Uint8Array.from([1, 250, 3, 4, 34, 64, 39, 43, 12])
const salt = Uint8Array.from([45, 124, 45, 29, 172, 238, 35])
const hash = await scrypt(password, salt, 16384, 8, 1, 32)

// Derive a key with PBKDF2
const key = await pbkdf2.deriveAsync(password, salt, 2048, 64, 'sha512')

// Create a public key from a private key
const publicKey = await secp256k1.publicKeyCreate(privateKey, true)
```

## API Documentation

See the [detailed API reference](docs/references/api-reference.md) for complete documentation of all functions.

### Core Functions

- [`pbkdf2.deriveAsync`](docs/references/api-reference.md#pbkdf2deriveasync) - PBKDF2-SHA512 key derivation
- [`scrypt`](docs/references/api-reference.md#scrypt) - scrypt key derivation
- [`secp256k1.publicKeyCreate`](docs/references/api-reference.md#secp256k1publickeycreate) - Create public key from private key
- [`secp256k1.privateKeyTweakAdd`](docs/references/api-reference.md#secp256k1privatekeytweakadd) - Add tweak to private key
- [`secp256k1.publicKeyTweakAdd`](docs/references/api-reference.md#secp256k1publickeytweakadd) - Add tweak to public key

## Guides

- [Getting Started Guide](docs/guides/getting-started.md) - Installation and basic usage
- [Buffer Handling Patterns](docs/patterns/buffer-handling.md) - Working with binary data

## Build libsecp256k1 from scratch (optional)

The build process requires several pieces of software to be installed on the
host system:

- autoconf
- automake
- git
- libtool

To install these on the Mac, please use [Homebrew](http://brew.sh/):

    brew install autoconf automake git libtool

If you are building for iOS, you also need a working installation
of the XCode command-line tools. In addition, Macs with Apple Silicon
will also need to do `softwareupdate --install-rosetta`.

Finally, run `./build-secp256k1.sh`
