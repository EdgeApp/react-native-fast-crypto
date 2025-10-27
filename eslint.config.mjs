import standardConfig from 'eslint-config-standard-kit'

export default [
  ...standardConfig({
    prettier: true,
    sortImports: true,
    node: true,
    typescript: true
  }),

  // Global ignores need to be in their own block:
  {
    ignores: ['lib/*']
  }
]
