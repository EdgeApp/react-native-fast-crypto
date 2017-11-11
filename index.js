import { NativeModules } from 'react-native'
import { base64 } from 'rfc4648'

const { RNFastCrypto } = NativeModules

async function scrypt (passwd, salt, N, r, p, size) {
  passwd = base64.stringify(passwd)
  salt = base64.stringify(salt)

  console.log('RNFS:scrypt(' + N.toString() + ', ' + r.toString() + ', ' + p.toString())
  const t = Date.now()
  const retval:string = await RNFastCrypto.scrypt(passwd, salt, N, r, p, size)
  const elapsed = Date.now() - 1
  console.log('RNFS:script finished in ' + elapsed + 'ms')

  let uint8array =  base64.parse(retval)
  return uint8array.slice(0, size)
}

const crypto = {
  scrypt
}

// export default crypto
export default crypto
