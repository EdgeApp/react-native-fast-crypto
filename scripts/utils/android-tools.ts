import { join } from 'path'

import { fileExists, loudExec } from './common'

// Matches the Edge app NDK version
const NDK_VERSION = '26.1.10909125'

export async function getNdkPath(): Promise<string> {
  const { ANDROID_HOME } = process.env
  if (ANDROID_HOME == null) {
    throw new Error('ANDROID_HOME is not set in the environment.')
  }

  // Find the NDK:
  const ndkPath = join(ANDROID_HOME, 'ndk', NDK_VERSION)
  const hasNdk = await fileExists(ndkPath)

  // Install the NDK if we need it:
  if (!hasNdk) {
    console.log(`Installing NDK ${NDK_VERSION}...`)
    const sdkManagerPath = join(
      ANDROID_HOME,
      'cmdline-tools/latest/bin/sdkmanager'
    )
    await loudExec(sdkManagerPath, [`"ndk;${NDK_VERSION}"`])
  }

  return ndkPath
}
