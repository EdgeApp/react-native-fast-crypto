# Buffer Handling Patterns

This document describes patterns for working with buffers and binary data in react-native-fast-crypto.

## Overview

react-native-fast-crypto uses Uint8Array for all binary data operations. The library internally converts between different encodings as needed for native module communication.

## Key Patterns

### 1. Using the Buffer Polyfill

The library uses the `buffer` npm package to provide Node.js Buffer compatibility:

```javascript
const Buffer = require('buffer/').Buffer
```

This allows using `Buffer.allocUnsafe()` for performance when creating output buffers:

```javascript
// From index.js - efficient buffer allocation
const outBuf = base16.parse(publicKeyHex, { out: Buffer.allocUnsafe })
```

### 2. Base64 Bridge Pattern

The native modules communicate using Base64 encoding to safely pass binary data:

```javascript
// JavaScript side encodes to Base64
const out = await RNFastCrypto.pbkdf2Sha512(
  base64.stringify(data),
  base64.stringify(salt),
  iterations,
  size
)
// Result is decoded from Base64
return base64.parse(out, { out: Buffer.allocUnsafe })
```

### 3. Hex String Pattern for secp256k1

The secp256k1 functions use hex strings for communication with native:

```javascript
async function publicKeyCreate(privateKey: Uint8Array, compressed: boolean) {
  // Convert to hex for native
  const privateKeyHex = base16.stringify(privateKey)

  // Native returns hex
  const publicKeyHex: string = await RNFastCrypto.secp256k1EcPubkeyCreate(
    privateKeyHex,
    compressed
  )

  // Convert back to Uint8Array
  const outBuf = base16.parse(publicKeyHex, { out: Buffer.allocUnsafe })
  return outBuf
}
```

## Best Practices

### 1. Consistent Encoding/Decoding

Always use the same encoding library (rfc4648) throughout:

```javascript
import { base16, base64 } from 'rfc4648'

// Good - consistent library usage
const hex = base16.stringify(data)
const bytes = base16.parse(hex)

// Avoid mixing different encoding libraries
```

### 2. Efficient Buffer Allocation

Use `Buffer.allocUnsafe` when parsing to avoid zero-initialization overhead:

```javascript
// Efficient - skips zero-initialization
const buffer = base64.parse(encoded, { out: Buffer.allocUnsafe })

// Less efficient - zeros memory first
const buffer = base64.parse(encoded)
```

### 3. Size Validation

The scrypt function validates output size:

```javascript
let uint8array = base64.parse(retval)
return uint8array.subarray(0, size) // Ensure exact size
```

### 4. Type Safety

Always ensure inputs are Uint8Array:

```javascript
// Good - explicit type
function processKey(key: Uint8Array) {
  // ...
}

// Validate at runtime if needed
if (!(data instanceof Uint8Array)) {
  throw new Error('Input must be Uint8Array')
}
```

## Platform-Specific Handling

### iOS

- Uses UTF8String for hex conversions
- Pre-allocates buffers for output strings
- Handles null terminators in C strings

### Android

- Uses Base64.DEFAULT flag for standard encoding
- Converts through UTF-8 for PBKDF2 character encoding
- JNI layer handles hex string conversions

## Common Patterns

### 1. String to Binary Conversion

```javascript
import { utf8 } from './utf8'

// Convert UTF-8 string to bytes
const passwordBytes = utf8.parse('my password')

// Use with crypto functions
const key = await scrypt(passwordBytes, salt, N, r, p, size)
```

### 2. Binary to Storage Format

```javascript
// For storage/transmission
const keyHex = base16.stringify(privateKey)
const keyBase64 = base64.stringify(privateKey)

// From storage
const privateKey = base16.parse(storedHex)
```

### 3. Working with Test Vectors

```javascript
// Parse test vectors
const testKey = base16.parse(
  '0d5a06c12ed605cdcd809b88f3299efda6bcb46f3c844d7003d7c9926adfa010'
)

// Compare results
expect(base16.stringify(result).toLowerCase()).equals(expectedHex)
```

## Memory Considerations

1. **Native buffers**: Freed automatically after use
2. **JavaScript buffers**: Garbage collected
3. **Large operations**: Consider chunking for very large inputs
4. **Sensitive data**: No built-in secure erasure - implement if needed

## Error Handling

Handle encoding errors gracefully:

```javascript
try {
  const decoded = base16.parse(userInput)
} catch (error) {
  throw new Error('Invalid hex input: ' + error.message)
}
```
