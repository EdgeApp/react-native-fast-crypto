declare module 'react-native' {
  // Provide our own local definitions for the few React Native
  // things we use.

  interface NativeModules {
    RNFastCrypto: {
      pbkdf2Sha512: (
        dataBase64: string,
        saltBase64: string,
        iterations: number,
        size: number
      ) => Promise<string>

      scrypt: (
        passwdBase64: string,
        saltBase64: string,
        N: number,
        r: number,
        p: number,
        size: number
      ) => Promise<string>

      secp256k1EcPrivkeyTweakAdd: (
        privateKeyHex: string,
        tweakHex: string
      ) => Promise<string>
      secp256k1EcPubkeyCreate: (
        privateKeyHex: string,
        compressed: boolean
      ) => Promise<string>
      secp256k1EcPubkeyTweakAdd: (
        publicKeyHex: string,
        tweakHex: string,
        compressed: boolean
      ) => Promise<string>
    }
  }
  const NativeModules: NativeModules
}
