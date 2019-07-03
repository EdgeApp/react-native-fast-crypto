import { NativeModules } from 'react-native'

const { RNFastCrypto } = NativeModules

export async function methodByString(method: string, jsonParams: string) {
  const result = await RNFastCrypto.moneroCore(method, jsonParams)
  return result
}
