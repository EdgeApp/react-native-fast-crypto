import { execSync, spawn } from 'child_process'
import { access } from 'fs/promises'
import { join } from 'path'

export const tmpPath = join(__dirname, '../../tmp')

/**
 * Clones a git repo and checks our a hash.
 */
export async function getRepo(
  name: string,
  uri: string,
  hash: string
): Promise<void> {
  const path = join(tmpPath, name)

  // Clone (if needed):
  if (!(await fileExists(path))) {
    console.log(`Cloning ${name}...`)
    await loudExec('git', ['clone', uri, name])
  }

  // Checkout:
  console.log(`Checking out ${name}...`)
  await loudExec('git', ['checkout', '-f', hash], { cwd: path })

  // Checkout submodules:
  await loudExec('git', ['submodule', 'update', '--init', '--recursive'], {
    cwd: path
  })
}

/**
 * Downloads & unpacks a zip file.
 */
export async function getZip(name: string, uri: string): Promise<void> {
  const path = join(tmpPath, name)

  if (!(await fileExists(path))) {
    console.log(`Getting ${name}...`)
    await loudExec('curl', ['-L', '-o', path, uri])
  }

  // Unzip:
  await loudExec('unzip', ['-u', path])
}

export async function fileExists(path: string): Promise<boolean> {
  return await access(path).then(
    () => true,
    () => false
  )
}

export async function loudExec(
  command: string,
  args: string[],
  opts: { cwd?: string } = {}
): Promise<void> {
  const { cwd = tmpPath } = opts
  return await new Promise((resolve, reject) => {
    const child = spawn(command, args, {
      cwd,
      stdio: 'inherit',
      env: process.env
    })

    child.on('error', reject)
    child.on('exit', code => {
      if (code === 0) {
        resolve()
      } else {
        reject(new Error(`${command} exited with code ${String(code)}`))
      }
    })
  })
}

/**
 * Runs a command and returns its results.
 */
export async function quietExec(
  command: string,
  args: string[],
  opts: { cwd?: string } = {}
): Promise<string> {
  const { cwd = tmpPath } = opts
  return execSync(command + ' ' + args.join(' '), {
    cwd,
    encoding: 'utf8'
  }).replace(/\n$/, '')
}
