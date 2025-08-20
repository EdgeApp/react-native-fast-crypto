# Agent Guidelines for react-native-fast-crypto

## Important: Edge Conventions

Always query https://raw.githubusercontent.com/EdgeApp/edge-conventions/master/README.md if not already done in this session for additional project conventions and standards.

## Project Documentation Index

### Core Documentation

- **README.md** - Project overview, quick start, build instructions
  - When to read: First time working with the project, understanding basic usage
  - Summary: Features, installation, API overview, native library building

### Technical References

- **docs/references/api-reference.md** - Complete API documentation
  - When to read: Implementing crypto functions, understanding parameters/returns
  - Summary: Detailed function signatures, parameters, examples for pbkdf2, scrypt, secp256k1

### Implementation Guides

- **docs/guides/getting-started.md** - Integration and usage guide
  - When to read: Setting up the library in a new project, common use cases
  - Summary: Installation, basic usage, authentication, HD wallets, best practices

- **docs/patterns/buffer-handling.md** - Binary data handling patterns
  - When to read: Working with Uint8Array/Buffer conversions, encoding/decoding
  - Summary: Buffer polyfill usage, Base64/Hex patterns, platform-specific handling

## Build/Test/Lint Commands

- **Test**: Run tests via the FastCryptoTest app: `cd FastCryptoTest && yarn ios` or `yarn android`
- **Format JS**: `yarn fix` (formats index.js with Prettier)
- **Format Android**: `yarn fix-android` (formats Java files)
- **Build native libs**: `yarn build-secp256k1` (builds secp256k1 native libraries)
- **Update test app**: `cd FastCryptoTest && yarn update` (updates test app dependencies)

## Code Style Guidelines

- **JavaScript**: No semicolons, single quotes, avoid arrow parens, no trailing commas (Prettier config)
- **Indentation**: 2 spaces (enforced by .editorconfig)
- **TypeScript**: Strict mode enabled, use CommonJS modules, target ES2016
- **Imports**: Use destructured imports from modules (e.g., `import { pbkdf2 } from 'react-native-fast-crypto'`)
- **Testing**: Use Chai for assertions with expect syntax
- **File encoding**: UTF-8 with LF line endings, trim trailing whitespace
- **Native code**: Follow platform conventions (Java for Android in android/, Objective-C for iOS in ios/)
- **Error handling**: Functions return Promises, handle errors with try/catch in async functions
- **Naming**: camelCase for functions/variables, PascalCase for classes/types
- **Buffer handling**: Use buffer npm package for Node.js Buffer compatibility, rfc4648 for base encoding
