// Run this script as `node -r sucrase/register ./scripts/build-native.ts`
//
// It will:
// - Download libsecp256k1.
// - Assemble Android shared libraries for each platform.
// - Assemble an iOS universal static xcframework.
//

import { mkdir, rm } from 'fs/promises'
import { cpus } from 'os'
import { join } from 'path'

import { getNdkPath } from './utils/android-tools'
import { getRepo, loudExec, quietExec, tmpPath } from './utils/common'
import { getObjcopyPath } from './utils/ios-tools'

const srcPath = join(__dirname, '../src')

async function main(): Promise<void> {
  await mkdir(tmpPath, { recursive: true })
  await downloadSources()

  // Android:
  for (const platform of androidPlatforms) {
    await buildAndroid(platform)
  }

  // iOS:
  for (const platform of iosPlatforms) {
    await buildIos(platform)
  }
  await packageIos()
}

async function downloadSources(): Promise<void> {
  // libsecp256k1 0.6.0:
  await getRepo(
    'libsecp256k1',
    'https://github.com/bitcoin-core/secp256k1.git',
    '0cdc758a56360bf58a851fe91085a327ec97685a'
  )

  // ios-cmake 4.5.0:
  await getRepo(
    'ios-cmake',
    'https://github.com/leetal/ios-cmake.git',
    '99f99df73dc752c7e88916fc1f9df6b9f8c40f8d'
  )
}

// Compiler options:
const includePaths: string[] = ['libsecp256k1/include']

// Source list (from src/):
const sources: string[] = [
  'native-crypto.cpp',
  'scrypt/crypto_scrypt.c',
  'scrypt/sha256.c'
]

interface AndroidPlatform {
  arch: string
  triple: string
}

interface IosPlatform {
  sdk: 'iphoneos' | 'iphonesimulator'
  arch: string
  cmakePlatform: string
}

const androidPlatforms: AndroidPlatform[] = [
  { arch: 'arm64-v8a', triple: 'aarch64-linux-android23' },
  { arch: 'armeabi-v7a', triple: 'armv7a-linux-androideabi23' },
  { arch: 'x86', triple: 'i686-linux-android23' },
  { arch: 'x86_64', triple: 'x86_64-linux-android23' }
]

const iosPlatforms: IosPlatform[] = [
  { sdk: 'iphoneos', arch: 'arm64', cmakePlatform: 'OS64' },
  { sdk: 'iphonesimulator', arch: 'arm64', cmakePlatform: 'SIMULATORARM64' },
  { sdk: 'iphonesimulator', arch: 'x86_64', cmakePlatform: 'SIMULATOR64' }
]

const iosSdkTriples: Record<string, string> = {
  iphoneos: '%arch%-apple-ios13.0',
  iphonesimulator: '%arch%-apple-ios13.0-simulator'
}

/**
 * Invokes CMake to build libsecp256k1,
 * followed by clang++ to build the shared library.
 */
async function buildAndroid(platform: AndroidPlatform): Promise<void> {
  const { arch, triple } = platform
  const ndkPath = await getNdkPath()
  const working = join(tmpPath, `android-${arch}`)

  // Build libsecp256k1 with CMake:
  await loudExec('cmake', [
    // Source directory:
    `-S${join(tmpPath, 'libsecp256k1')}`,
    // Build directory:
    `-B${join(working, 'libsecp256k1')}`,
    // Build options:
    `-DCMAKE_ANDROID_ARCH_ABI=${arch}`,
    `-DCMAKE_ANDROID_NDK=${ndkPath}`,
    `-DCMAKE_ANDROID_STL_TYPE=c++_shared`,
    `-DCMAKE_BUILD_TYPE=Release`,
    `-DCMAKE_INSTALL_PREFIX=${working}`,
    `-DCMAKE_SYSTEM_NAME=Android`,
    `-DCMAKE_SYSTEM_VERSION=23`,
    // libsecp256k1 options:
    `-DSECP256K1_ECMULT_WINDOW_SIZE=12`,
    `-DSECP256K1_DISABLE_SHARED=ON`
  ])
  await loudExec('cmake', [
    '--build',
    join(working, 'libsecp256k1'),
    '--config',
    'Release',
    '--target',
    'install',
    '--',
    `-j${cpus().length}`
  ])

  // Build the library:
  const cxxPath = join(
    ndkPath,
    `toolchains/llvm/prebuilt/darwin-x86_64/bin/${triple}-clang++`
  )
  const outPath = join(tmpPath, '../android/src/main/jniLibs/', arch)
  await mkdir(outPath, { recursive: true })
  const jniSources = [...sources, 'jni/jni.cpp']

  console.log(`Linking libfastcrypto.so for Android ${arch}`)
  await loudExec(cxxPath, [
    '-shared',
    '-fPIC',
    `-o${join(outPath, 'libfastcrypto.so')}`,
    join(working, 'lib/libsecp256k1.a'),
    ...includePaths.map(path => `-I${join(tmpPath, path)}`),
    ...jniSources.flatMap(source => [
      '-x',
      source.endsWith('.c') ? 'c' : 'c++',
      join(srcPath, source)
    ]),
    '-llog',
    `-Wl,--version-script=${join(srcPath, 'jni/exports.map')}`,
    '-Wl,--no-undefined',
    '-Wl,-z,max-page-size=16384'
  ])
}

/**
 * Invokes CMake to build libsecp256k1,
 * then combines everything into a big .o file,
 * which we shove into a static .a file.
 */
async function buildIos(platform: IosPlatform): Promise<void> {
  const { sdk, arch, cmakePlatform } = platform
  const working = join(tmpPath, `${sdk}-${arch}`)
  await mkdir(working, { recursive: true })

  // Build libsecp256k1 with CMake:
  const iosToolchain = join(tmpPath, 'ios-cmake/ios.toolchain.cmake')
  await loudExec('cmake', [
    // Source directory:
    `-S${join(tmpPath, 'libsecp256k1')}`,
    // Build directory:
    `-B${join(working, 'libsecp256k1')}`,
    // Build options:
    `-DCMAKE_BUILD_TYPE=Release`,
    `-DCMAKE_INSTALL_PREFIX=${working}`,
    `-DCMAKE_SYSTEM_NAME=iOS`,
    `-DCMAKE_TOOLCHAIN_FILE=${iosToolchain}`,
    `-DCMAKE_XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH=NO`,
    `-DPLATFORM=${cmakePlatform}`,
    `-GXcode`,
    // libsecp256k1 options:
    `-DSECP256K1_ECMULT_WINDOW_SIZE=12`,
    `-DSECP256K1_DISABLE_SHARED=ON`
  ])
  await loudExec('cmake', [
    '--build',
    join(working, 'libsecp256k1'),
    '--config',
    'Release',
    '--target',
    'install'
  ])

  // Find platform tools:
  const ar = await quietExec('xcrun', ['--sdk', sdk, '--find', 'ar'])
  const cc = await quietExec('xcrun', ['--sdk', sdk, '--find', 'clang'])
  const cxx = await quietExec('xcrun', ['--sdk', sdk, '--find', 'clang++'])
  const ld = await quietExec('xcrun', ['--sdk', sdk, '--find', 'ld'])
  const objcopy = await getObjcopyPath()
  const sdkFlags = [
    '-arch',
    arch,
    '-target',
    iosSdkTriples[sdk].replace('%arch%', arch),
    '-isysroot',
    await quietExec('xcrun', ['--sdk', sdk, '--show-sdk-path'])
  ]
  const cflags = [
    ...includePaths.map(path => `-I${join(tmpPath, path)}`),
    '-miphoneos-version-min=13.0',
    '-O2',
    '-Werror=partial-availability'
  ]
  const cxxflags = [...cflags, '-std=c++11']

  // Compile our sources:
  const objects: string[] = []
  for (const source of sources) {
    console.log(`Compiling ${source} for ${sdk}-${arch}...`)

    // Figure out the object file name:
    const object = join(
      working,
      source.replace(/^.*\//, '').replace(/\.c$|\.cc$|\.cpp$/, '.o')
    )
    objects.push(object)

    const useCxx = /\.cpp$|\.cc$/.test(source)
    await loudExec(useCxx ? cxx : cc, [
      '-c',
      ...(useCxx ? cxxflags : cflags),
      ...sdkFlags,
      `-o${object}`,
      join(srcPath, source)
    ])
  }

  // Link everything together into a single giant .o file:
  console.log(`Linking fastcrypto.o for ${sdk} ${arch}`)
  const objectPath = join(working, 'fastcrypto.o')
  await loudExec(ld, [
    '-r',
    '-o',
    objectPath,
    ...objects,
    join(working, 'lib/libsecp256k1.a')
  ])

  // Localize all symbols except the ones we really want,
  // hiding them from future linking steps:
  await loudExec(objcopy, [objectPath, '-w', '-L*', '-L!_fast_crypto_*'])

  // Generate a static library:
  console.log(`Building static library for ${sdk}-${arch}...`)
  const library = join(working, `libfastcrypto.a`)
  await rm(library, { force: true })
  await loudExec(ar, ['rcs', library, objectPath])
}

/**
 * Creates a unified xcframework file out of the per-platform
 * static libraries that `buildIos` creates.
 */
async function packageIos(): Promise<void> {
  const sdks = new Set(iosPlatforms.map(row => row.sdk))

  // Merge the platforms into a fat library:
  const merged: string[] = []
  for (const sdk of sdks) {
    console.log(`Merging libraries for ${sdk}...`)
    const outPath = join(tmpPath, `${sdk}-lipo`)
    await mkdir(outPath, { recursive: true })
    const output = join(outPath, 'libfastcrypto.a')

    await loudExec('lipo', [
      '-create',
      '-output',
      output,
      ...iosPlatforms
        .filter(platform => platform.sdk === sdk)
        .map(({ sdk, arch }) =>
          join(tmpPath, `${sdk}-${arch}`, 'libfastcrypto.a')
        )
    ])
    merged.push('-library', output)
  }

  // Bundle those into an XCFramework:
  console.log('Creating XCFramework...')
  await rm('ios/RNFastCrypto.xcframework', { recursive: true, force: true })
  await loudExec('xcodebuild', [
    '-create-xcframework',
    ...merged,
    '-output',
    join(__dirname, '../ios/RNFastCrypto.xcframework')
  ])
}

main().catch((error: unknown) => {
  console.log(error)
})
