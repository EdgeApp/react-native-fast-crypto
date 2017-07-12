import { NativeModules } from 'react-native'
import { base64 } from 'rfc4648'

const { RNFastCrypto } = NativeModules

async function scrypt (passwd, salt, N, r, p, size) {
  const retval = await RNFastCrypto.scrypt(passwd, salt, N, r, p, size)

  let uint8array = base64.parse(retval)
  return uint8array
}

const crypto = {
  scrypt
}

// export default crypto
export default crypto
