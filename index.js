import { NativeModules } from 'react-native'
import { base64 } from 'rfc4648'

const { RNFastCrypto } = NativeModules

async function scrypt (passwd, salt, N, r, p, size) {
  passwd = base64.stringify(passwd)
  salt = base64.stringify(salt)

  console.log('RNFS:scrypt(' + N.toString() + ', ' + r.toString() + ', ' + p.toString())
  const t = Date.now()
  const retval:string = await RNFastCrypto.scrypt(passwd, salt, N, r, p, size)
  const elapsed = Date.now() - t
  console.log('RNFS:script finished in ' + elapsed + 'ms')

  let uint8array = base64.parse(retval)
  return uint8array.slice(0, size)
}

async function publicKeyCreate (privateKeyHex: string, compressed: boolean) {
  const publicKeyHex: string = RNFastCrypto.secp256k1EcPubkeyCreate(privateKeyHex, compressed)
  return publicKeyHex
}

async function privateKeyTweakAdd (privateKeyHex: string, tweakHex: string) {
  const privateKeyTweakedHex: string = RNFastCrypto.secp256k1EcPrivkeyTweakAdd(privateKeyHex, tweakHex)
  return privateKeyTweakedHex
}

async function publicKeyTweakAdd (publicKeyHex: string, tweakHex: string, compressed: boolean) {
  const publickKeyTweakedHex: string = RNFastCrypto.secp256k1EcPubkeyTweakAdd(publicKeyHex, tweakHex, compressed)
  return publickKeyTweakedHex
}

const secp256k1 = {
  publicKeyCreate,
  privateKeyTweakAdd,
  publicKeyTweakAdd
}

const crypto = {
  scrypt,
  secp256k1
}

// export default crypto
export default crypto
