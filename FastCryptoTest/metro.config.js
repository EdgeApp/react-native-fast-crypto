const {getDefaultConfig, mergeConfig} = require('@react-native/metro-config');
const path = require('path');

/**
 * Metro configuration for React Native 0.81.4 - FastCryptoTest
 * Modern version of FastCryptoTest with TRUE 16KB support
 */
const projectRoot = __dirname;
const parentRoot = path.resolve(projectRoot, '..');

const config = {
  projectRoot,
  watchFolders: [parentRoot],
  resolver: {
    // Use ONLY our own node_modules to avoid conflicts with parent project
    nodeModulesPaths: [
      path.resolve(projectRoot, 'node_modules'),
    ],
    alias: {
      'react-native-fast-crypto': parentRoot,
    },
    // Disable hierarchical lookup to avoid parent node_modules
    disableHierarchicalLookup: true,
  },
};

module.exports = mergeConfig(getDefaultConfig(__dirname), config);
