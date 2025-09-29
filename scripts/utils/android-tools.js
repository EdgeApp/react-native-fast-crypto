const { join } = require('path')
const { existsSync } = require('fs')
const { execSync } = require('child_process')

// Lock to a specific NDK version for consistent builds
const NDK_VERSION = '28.0.12674087'

function fileExists(path) {
  return existsSync(path)
}

function loudExec(command, args = []) {
  const fullCommand = `${command} ${args.join(' ')}`
  console.log(`Executing: ${fullCommand}`)
  execSync(fullCommand, { stdio: 'inherit' })
}

async function getNdkPath() {
  const { ANDROID_HOME } = process.env
  if (ANDROID_HOME == null) {
    throw new Error('ANDROID_HOME is not set in the environment.')
  }

  // Find the NDK:
  const ndkPath = join(ANDROID_HOME, 'ndk', NDK_VERSION)
  const hasNdk = fileExists(ndkPath)

  // Install the NDK if we need it:
  if (!hasNdk) {
    console.log(`Installing NDK ${NDK_VERSION}...`)
    const sdkManagerPath = join(
      ANDROID_HOME,
      'cmdline-tools/latest/bin/sdkmanager'
    )
    loudExec(sdkManagerPath, [`"ndk;${NDK_VERSION}"`])
  }

  return ndkPath
}

function getNdkVersion() {
  return NDK_VERSION
}

module.exports = {
  getNdkPath,
  getNdkVersion
}
