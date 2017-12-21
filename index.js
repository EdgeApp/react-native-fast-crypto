import { NativeModules } from 'react-native'
import { base64 } from 'rfc4648'

const { RNFastCrypto } = NativeModules
const Buffer = require('buffer/').Buffer

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

async function publicKeyCreate (privateKey: string, compressed: boolean) {
  const privateKeyHex = privateKey.toString('hex')
  const publicKeyHex: string = await RNFastCrypto.secp256k1EcPubkeyCreate(privateKeyHex, compressed)
  const outBuf = Buffer.from(publicKeyHex, 'hex')
  return outBuf
}

async function privateKeyTweakAdd (privateKey: string, tweak: string) {
  const privateKeyHex = privateKey.toString('hex')
  const tweakHex = tweak.toString('hex')
  const privateKeyTweakedHex: string = await RNFastCrypto.secp256k1EcPrivkeyTweakAdd(privateKeyHex, tweakHex)
  const outBuf = Buffer.from(privateKeyTweakedHex, 'hex')
  return outBuf
}

async function publicKeyTweakAdd (publicKey: string, tweak: string, compressed: boolean) {
  const publicKeyHex = publicKey.toString('hex')
  const tweakHex = tweak.toString('hex')
  const publickKeyTweakedHex: string = await RNFastCrypto.secp256k1EcPubkeyTweakAdd(publicKeyHex, tweakHex, compressed)
  const outBuf = Buffer.from(publickKeyTweakedHex, 'hex')
  return outBuf
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
