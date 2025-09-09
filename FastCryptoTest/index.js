/**
 * FastCryptoTest - React Native 0.81.4 with 16KB support
 * Direct entry point to crypto test suite
 * @format
 */

import {AppRegistry} from 'react-native';
import {FastCryptoTest} from './src/FastCryptoTest';
import {name as appName} from './app.json';

AppRegistry.registerComponent(appName, () => FastCryptoTest);
