# Getting Started with react-native-fast-crypto

This guide will help you integrate react-native-fast-crypto into your React Native application.

## Installation

### React Native 0.60+

For React Native 0.60 and above, the library supports autolinking:

```bash
npm install react-native-fast-crypto --save
# or
yarn add react-native-fast-crypto
```

Then for iOS, install the native dependencies:

```bash
cd ios && pod install
```

### Platform-Specific Setup

#### iOS

No additional setup required after pod install.

#### Android

No additional setup required with autolinking.

## Basic Usage

### 1. Import the library

```javascript
import { pbkdf2, scrypt, secp256k1 } from 'react-native-fast-crypto'
```

### 2. Password Hashing with scrypt

```javascript
import { scrypt } from 'react-native-fast-crypto'
import { utf8 } from './utf8' // You'll need a UTF-8 encoding utility

async function hashPassword(password, salt) {
  // Convert strings to Uint8Array
  const passwordBytes = utf8.parse(password)
  const saltBytes = utf8.parse(salt)

  // Derive key using scrypt
  const derivedKey = await scrypt(
    passwordBytes,
    saltBytes,
    16384, // N: CPU/memory cost
    8, // r: block size
    1, // p: parallelization
    32 // key length in bytes
  )

  return derivedKey
}
```

### 3. Key Derivation with PBKDF2

```javascript
import { pbkdf2 } from 'react-native-fast-crypto'
import { base64 } from 'rfc4648'

async function deriveKey(passphrase, salt) {
  const key = await pbkdf2.deriveAsync(
    passphrase,
    salt,
    2048, // iterations
    64, // key length
    'sha512' // algorithm (only sha512 supported)
  )

  // Convert to base64 for storage
  return base64.stringify(key)
}
```

### 4. Working with secp256k1 Keys

```javascript
import { secp256k1 } from 'react-native-fast-crypto'
import { base16 } from 'rfc4648'

async function generatePublicKey() {
  // Generate or load your private key (32 bytes)
  const privateKey = base16.parse('your-private-key-hex')

  // Create compressed public key
  const publicKey = await secp256k1.publicKeyCreate(privateKey, true)

  console.log('Public key:', base16.stringify(publicKey))
}
```

## Common Use Cases

### 1. User Authentication

```javascript
async function authenticateUser(username, password) {
  // Create a salt from username (or store separately)
  const salt = utf8.parse('salt:' + username)
  const passwordBytes = utf8.parse(password)

  // Derive authentication key
  const authKey = await scrypt(passwordBytes, salt, 16384, 8, 1, 32)

  // Use authKey for authentication
  return base64.stringify(authKey)
}
```

### 2. HD Wallet Key Derivation

```javascript
async function deriveChildKey(parentPrivateKey, chainCode, index) {
  // Create tweak from chain code and index
  // (This is a simplified example - real HD derivation is more complex)
  const tweak = await createTweak(chainCode, index)

  // Derive child private key
  const childPrivateKey = await secp256k1.privateKeyTweakAdd(
    parentPrivateKey,
    tweak
  )

  // Get corresponding public key
  const childPublicKey = await secp256k1.publicKeyCreate(
    childPrivateKey,
    true // compressed
  )

  return { childPrivateKey, childPublicKey }
}
```

### 3. Secure Password Storage

```javascript
async function hashPasswordForStorage(password) {
  // Generate random salt
  const salt = crypto.getRandomValues(new Uint8Array(32))

  // Hash with high cost parameters
  const hash = await scrypt(
    utf8.parse(password),
    salt,
    65536, // Higher N for more security
    8,
    1,
    64 // Longer hash
  )

  // Store both salt and hash
  return {
    salt: base64.stringify(salt),
    hash: base64.stringify(hash)
  }
}
```

## Best Practices

1. **Always use salts**: Never hash passwords without a unique salt
2. **Choose appropriate parameters**: Higher iterations/memory costs increase security but also computation time
3. **Handle errors**: Always wrap async calls in try/catch blocks
4. **Secure key storage**: Never store private keys in plain text
5. **Use compressed keys**: When possible, use compressed public keys to save space

## Testing Your Integration

Create a simple test to verify the library is working:

```javascript
import { scrypt } from 'react-native-fast-crypto'

async function testCrypto() {
  try {
    const password = new Uint8Array([1, 2, 3, 4])
    const salt = new Uint8Array([5, 6, 7, 8])

    const result = await scrypt(password, salt, 1024, 8, 1, 32)
    console.log('Success! Key length:', result.length)
  } catch (error) {
    console.error('Crypto test failed:', error)
  }
}
```

## Troubleshooting

### iOS Build Issues

- Ensure you've run `pod install` after installation
- Clean build folder: Xcode → Product → Clean Build Folder

### Android Build Issues

- Clean and rebuild: `cd android && ./gradlew clean`
- Ensure minSdkVersion is compatible

### Runtime Errors

- Check that all inputs are Uint8Array type
- Verify algorithm parameter is 'sha512' for pbkdf2
- Ensure proper error handling for all async operations
