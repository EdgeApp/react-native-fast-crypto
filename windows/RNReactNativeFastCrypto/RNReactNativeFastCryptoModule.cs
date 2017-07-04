using ReactNative.Bridge;
using System;
using System.Collections.Generic;
using Windows.ApplicationModel.Core;
using Windows.UI.Core;

namespace Com.Reactlibrary.RNReactNativeFastCrypto
{
    /// <summary>
    /// A module that allows JS to share data.
    /// </summary>
    class RNReactNativeFastCryptoModule : NativeModuleBase
    {
        /// <summary>
        /// Instantiates the <see cref="RNReactNativeFastCryptoModule"/>.
        /// </summary>
        internal RNReactNativeFastCryptoModule()
        {

        }

        /// <summary>
        /// The name of the native module.
        /// </summary>
        public override string Name
        {
            get
            {
                return "RNReactNativeFastCrypto";
            }
        }
    }
}
