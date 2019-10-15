import { NativeModules } from 'react-native'
import { base16, base64 } from 'rfc4648'

const { RNFastCrypto } = NativeModules
const Buffer = require('buffer/').Buffer

export async function scrypt (passwd, salt, N, r, p, size) {
  passwd = base64.stringify(passwd)
  salt = base64.stringify(salt)

  console.log('RNFS:scrypt(' + N.toString() + ', ' + r.toString() + ', ' + p.toString())
  const t = Date.now()
  const retval:string = await RNFastCrypto.scrypt(passwd, salt, N, r, p, size)
  const elapsed = Date.now() - t
  console.log('RNFS:scrypt finished in ' + elapsed + 'ms')

  let uint8array = base64.parse(retval)
  return uint8array.subarray(0, size)
}

export async function scryptBulk (passwds, salt, N, r, p, size) {
  passwds = passwds.map((passwd) => base64.stringify(passwd))
  salt = base64.stringify(salt)
  console.log('RNFS:scrypt Got ' + passwds.length + ' passwds to Hash')
  console.log('RNFS:scrypt(' + N.toString() + ', ' + r.toString() + ', ' + p.toString())
  const t = Date.now()
  const retval = await RNFastCrypto.scryptBulk(passwds, salt, N, r, p, size)
  const elapsed = Date.now() - t
  console.log('RNFS:scrypt bulk finished in ' + elapsed + 'ms')
  const retvalArray = new Array(retval.length);
  for (let i=0; i<retval.length; i++) {
    const uint8array = base64.parse(retval[i])
    retvalArray[i] = uint8array.subarray(0, size)
  }
  return retvalArray
}

async function publicKeyCreate (privateKey: Uint8Array, compressed: boolean) {
  const privateKeyHex = base16.stringify(privateKey)
  const publicKeyHex: string = await RNFastCrypto.secp256k1EcPubkeyCreate(privateKeyHex, compressed)
  const outBuf = base16.parse(publicKeyHex, { out: Buffer.allocUnsafe })
  return outBuf
}

async function privateKeyTweakAdd (privateKey: Uint8Array, tweak: Uint8Array) {
  const privateKeyHex = base16.stringify(privateKey)
  const tweakHex = base16.stringify(tweak)
  const privateKeyTweakedHex: string = await RNFastCrypto.secp256k1EcPrivkeyTweakAdd(privateKeyHex, tweakHex)
  const outBuf = base16.parse(privateKeyTweakedHex, { out: Buffer.allocUnsafe })
  return outBuf
}

async function publicKeyTweakAdd (publicKey: Uint8Array, tweak: Uint8Array, compressed: boolean) {
  const publicKeyHex = base16.stringify(publicKey)
  const tweakHex = base16.stringify(tweak)
  const publickKeyTweakedHex: string = await RNFastCrypto.secp256k1EcPubkeyTweakAdd(publicKeyHex, tweakHex, compressed)
  const outBuf = base16.parse(publickKeyTweakedHex, { out: Buffer.allocUnsafe })
  return outBuf
}

async function pbkdf2DeriveAsync(key: Uint8Array, salt: Uint8Array, iter: number, len: number, alg: string) {
  if (alg !== 'sha512') {
    throw new Error('ErrorUnsupportedPbkdf2Algorithm: ' + alg)
  }
  const keyHex = base16.stringify(key)
  const saltHex = base16.stringify(salt)
  const resultHex = await RNFastCrypto.pbkdf2Sha512(keyHex, saltHex, iter, len)
  const outBuf = base16.parse(resultHex, { out: Buffer.allocUnsafe })
  return outBuf
}

async function methodByString(method: string, jsonParams: string) {
  const result = await RNFastCrypto.moneroCore(method, jsonParams)
  return result
}

global.moneroCore = { methodByString }

export const secp256k1 = {
  publicKeyCreate,
  privateKeyTweakAdd,
  publicKeyTweakAdd
}

export const pbkdf2 = {
  deriveAsync: pbkdf2DeriveAsync
}

// Deprecated. Use the named exports instead:
export default {
  scrypt,
  secp256k1,
  pbkdf2,
  moneroCore
}
