import { fileExists, quietExec } from './common'

/**
 * Finds llvm-objcopy in Homebrew.
 *
 * Homebrew doesn't symlink LLVM into the normal location,
 * to avoid conflicting with Xcode's built-in tools.
 * We can get around this by looking in the right places.
 */
export async function getObjcopyPath(): Promise<string> {
  const whichPath = await quietExec('which', ['llvm-objcopy']).catch(() => {})
  if (whichPath != null) return whichPath

  const paths = [
    '/opt/homebrew/opt/llvm/bin/llvm-objcopy',
    '/usr/local/opt/llvm/bin/llvm-objcopy' // for Intel Macs
  ]
  for (const path of paths) {
    if (await fileExists(path)) return path
  }

  throw new Error('Please install `llvm-objcopy` using `brew install llvm`.')
}
