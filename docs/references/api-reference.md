# react-native-fast-crypto API Reference

This document provides a comprehensive API reference for the react-native-fast-crypto library.

## Overview

react-native-fast-crypto provides native implementations of cryptographic functions for React Native applications. All functions are asynchronous and return Promises.

## Imports

```javascript
import { pbkdf2, scrypt, secp256k1 } from 'react-native-fast-crypto'
```

## API Functions

### pbkdf2.deriveAsync

Derives a key using PBKDF2 (Password-Based Key Derivation Function 2) with SHA-512.

```javascript
pbkdf2.deriveAsync(
  data: Uint8Array,
  salt: Uint8Array,
  iterations: number,
  keyLength: number,
  algorithm: string
): Promise<Uint8Array>
```

**Parameters:**

- `data`: The password/passphrase as a Uint8Array
- `salt`: The salt value as a Uint8Array
- `iterations`: Number of iterations (recommended: 2048 or higher)
- `keyLength`: Desired length of the derived key in bytes
- `algorithm`: Must be 'sha512' (only supported algorithm)

**Returns:** Promise resolving to derived key as Uint8Array

**Example:**

```javascript
import { pbkdf2 } from 'react-native-fast-crypto'
import { utf8 } from './utf8' // Your UTF-8 encoding utility

const password = utf8.parse('my secure password')
const salt = utf8.parse('salt value')
const key = await pbkdf2.deriveAsync(password, salt, 2048, 64, 'sha512')
```

### scrypt

Derives a key using the scrypt key derivation function.

```javascript
scrypt(
  password: Uint8Array,
  salt: Uint8Array,
  N: number,
  r: number,
  p: number,
  keyLength: number
): Promise<Uint8Array>
```

**Parameters:**

- `password`: The password as a Uint8Array
- `salt`: The salt value as a Uint8Array
- `N`: CPU/memory cost parameter (must be power of 2, e.g., 16384)
- `r`: Block size parameter (typically 8)
- `p`: Parallelization parameter (typically 1)
- `keyLength`: Desired length of the derived key in bytes

**Returns:** Promise resolving to derived key as Uint8Array

**Example:**

```javascript
import { scrypt } from 'react-native-fast-crypto'
import { utf8 } from './utf8'

const password = utf8.parse('password123')
const salt = Uint8Array.from([45, 124, 45, 29, 172, 238, 35])
const key = await scrypt(password, salt, 16384, 8, 1, 32)
```

### secp256k1.publicKeyCreate

Creates a public key from a private key.

```javascript
secp256k1.publicKeyCreate(
  privateKey: Uint8Array,
  compressed: boolean
): Promise<Uint8Array>
```

**Parameters:**

- `privateKey`: The private key as a Uint8Array (32 bytes)
- `compressed`: Whether to return compressed (33 bytes) or uncompressed (65 bytes) public key

**Returns:** Promise resolving to public key as Uint8Array

**Example:**

```javascript
import { secp256k1 } from 'react-native-fast-crypto'
import { base16 } from 'rfc4648'

const privateKey = base16.parse(
  '0d5a06c12ed605cdcd809b88f3299efda6bcb46f3c844d7003d7c9926adfa010'
)
const publicKey = await secp256k1.publicKeyCreate(privateKey, true)
// Result: compressed 33-byte public key
```

### secp256k1.privateKeyTweakAdd

Adds a tweak value to a private key (used in HD wallet derivation).

```javascript
secp256k1.privateKeyTweakAdd(
  privateKey: Uint8Array,
  tweak: Uint8Array
): Promise<Uint8Array>
```

**Parameters:**

- `privateKey`: The private key as a Uint8Array (32 bytes)
- `tweak`: The tweak value as a Uint8Array (32 bytes)

**Returns:** Promise resolving to tweaked private key as Uint8Array

**Example:**

```javascript
import { secp256k1 } from 'react-native-fast-crypto'
import { base16 } from 'rfc4648'

const privateKey = base16.parse(
  '0d5a06c12ed605cdcd809b88f3299efda6bcb46f3c844d7003d7c9926adfa010'
)
const tweak = base16.parse(
  'a0f24d30c336181342c875be8e1df4c29e25278282f7add9142c71c76c316c8a'
)
const tweakedKey = await secp256k1.privateKeyTweakAdd(privateKey, tweak)
```

### secp256k1.publicKeyTweakAdd

Adds a tweak value to a public key (used in HD wallet derivation).

```javascript
secp256k1.publicKeyTweakAdd(
  publicKey: Uint8Array,
  tweak: Uint8Array,
  compressed: boolean
): Promise<Uint8Array>
```

**Parameters:**

- `publicKey`: The public key as a Uint8Array (33 or 65 bytes)
- `tweak`: The tweak value as a Uint8Array (32 bytes)
- `compressed`: Whether to return compressed or uncompressed public key

**Returns:** Promise resolving to tweaked public key as Uint8Array

**Example:**

```javascript
import { secp256k1 } from 'react-native-fast-crypto'
import { base16 } from 'rfc4648'

const publicKey = base16.parse(
  '0215a94b717775b487330c47db0324df661f66759af7435e54567f99371cda79e8'
)
const tweak = base16.parse(
  'ce547fb348b6d058c8c6190b781f98811cd77db75943fe681732ff8cafb4bb8d'
)
const tweakedPubKey = await secp256k1.publicKeyTweakAdd(publicKey, tweak, true)
```

## Data Encoding

This library uses Uint8Array for all binary data. For encoding/decoding:

- **Base16 (Hex)**: Use `rfc4648` library's `base16` module
- **Base64**: Use `rfc4648` library's `base64` module
- **UTF-8**: Implement your own or use a utility library

Example using rfc4648:

```javascript
import { base16, base64 } from 'rfc4648'

// Hex to Uint8Array
const bytes = base16.parse('deadbeef')

// Uint8Array to Hex
const hex = base16.stringify(bytes)

// Base64 encoding
const b64 = base64.stringify(bytes)
```

## Error Handling

All functions return Promises and should be used with async/await or .then()/.catch():

```javascript
try {
  const key = await pbkdf2.deriveAsync(password, salt, 2048, 64, 'sha512')
  // Use key...
} catch (error) {
  console.error('Key derivation failed:', error)
}
```

Common errors:

- `ErrorUnsupportedPbkdf2Algorithm`: When using algorithm other than 'sha512'
- Native module errors for invalid inputs or crypto operation failures

## Performance Notes

1. These functions use native implementations for optimal performance
2. scrypt operations log timing information to console
3. Large iteration counts or memory parameters will increase computation time
4. All operations are asynchronous to avoid blocking the UI thread
