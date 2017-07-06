
import { NativeModules } from 'react-native'
import Base64Binary from './base64-binary.js'

const { RNFastCrypto } = NativeModules

async function scrypt (passwd, salt, N, r, p, size) {
  const retval = await RNFastCrypto.scrypt(passwd, salt, N, r, p, size)

  let uint8array = Base64Binary.decode(retval)
  return uint8array
}

const crypto = {
  scrypt
}

// export default crypto
export default crypto;
