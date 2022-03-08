#!/bin/sh
# Copies the latest library code into the test app
(
  cd $(dirname $0)

  if [ ! -d ./FastCryptoTest/node_modules/react-native-fast-crypto/ ]; then
    echo 'Please run yarn inside the FastCryptoTest folder first'
    exit 1
  fi

  cp -r \
    ./android \
    ./react-native-fast-crypto.podspec \
    ./ios \
    ./package.json \
    ./index.js \
    ./FastCryptoTest/node_modules/react-native-fast-crypto/
)
