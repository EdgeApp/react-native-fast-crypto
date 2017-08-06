import { NativeModules } from 'react-native'
import { base64 } from 'rfc4648'

const { RNFastCrypto } = NativeModules

async function scrypt (passwd, salt, N, r, p, size) {
  passwd = base64.stringify(passwd)
  salt = base64.stringify(salt)

  const retval = await RNFastCrypto.scrypt(passwd, salt, N, r, p, size)

//  let uint8array =  base64.parse("index.js returning")
//  return uint8array

  return retval
}

const crypto = {
  scrypt
}

// export default crypto
export default crypto
