import { NativeModules } from 'react-native'
import { base16, base64 } from 'rfc4648'

const { RNFastCrypto } = NativeModules
const Buffer = require('buffer/').Buffer

async function pbkdf2DeriveAsync(
  data: Uint8Array,
  salt: Uint8Array,
  iterations: number,
  size: number,
  alg: string
): Promise<Uint8Array> {
  if (alg !== 'sha512') {
    throw new Error('ErrorUnsupportedPbkdf2Algorithm: ' + alg)
  }
  const out = await RNFastCrypto.pbkdf2Sha512(
    base64.stringify(data),
    base64.stringify(salt),
    iterations,
    size
  )
  return base64.parse(out, { out: Buffer.allocUnsafe })
}

export async function scrypt(
  passwdBytes: Uint8Array,
  saltBytes: Uint8Array,
  N: number,
  r: number,
  p: number,
  size: number
): Promise<Uint8Array> {
  const passwd = base64.stringify(passwdBytes)
  const salt = base64.stringify(saltBytes)

  console.log(
    'RNFS:scrypt(' + N.toString() + ', ' + r.toString() + ', ' + p.toString()
  )
  const t = Date.now()
  const retval: string = await RNFastCrypto.scrypt(passwd, salt, N, r, p, size)
  const elapsed = Date.now() - t
  console.log('RNFS:script finished in ' + elapsed + 'ms')

  const uint8array = base64.parse(retval)
  return uint8array.subarray(0, size)
}

async function publicKeyCreate(
  privateKey: Uint8Array,
  compressed: boolean
): Promise<Uint8Array> {
  const privateKeyHex = base16.stringify(privateKey)
  const publicKeyHex: string = await RNFastCrypto.secp256k1EcPubkeyCreate(
    privateKeyHex,
    compressed
  )
  const outBuf = base16.parse(publicKeyHex, { out: Buffer.allocUnsafe })
  return outBuf
}

async function privateKeyTweakAdd(
  privateKey: Uint8Array,
  tweak: Uint8Array
): Promise<Uint8Array> {
  const privateKeyHex = base16.stringify(privateKey)
  const tweakHex = base16.stringify(tweak)
  const privateKeyTweakedHex: string =
    await RNFastCrypto.secp256k1EcPrivkeyTweakAdd(privateKeyHex, tweakHex)
  const outBuf = base16.parse(privateKeyTweakedHex, {
    out: Buffer.allocUnsafe
  })
  return outBuf
}

async function publicKeyTweakAdd(
  publicKey: Uint8Array,
  tweak: Uint8Array,
  compressed: boolean
): Promise<Uint8Array> {
  const publicKeyHex = base16.stringify(publicKey)
  const tweakHex = base16.stringify(tweak)
  const publickKeyTweakedHex: string =
    await RNFastCrypto.secp256k1EcPubkeyTweakAdd(
      publicKeyHex,
      tweakHex,
      compressed
    )
  const outBuf = base16.parse(publickKeyTweakedHex, {
    out: Buffer.allocUnsafe
  })
  return outBuf
}

export const secp256k1 = {
  publicKeyCreate,
  privateKeyTweakAdd,
  publicKeyTweakAdd
}

export const pbkdf2 = {
  deriveAsync: pbkdf2DeriveAsync
}
