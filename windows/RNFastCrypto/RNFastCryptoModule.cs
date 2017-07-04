using ReactNative.Bridge;
using System;
using System.Collections.Generic;
using Windows.ApplicationModel.Core;
using Windows.UI.Core;

namespace Com.Reactlibrary.RNFastCrypto
{
    /// <summary>
    /// A module that allows JS to share data.
    /// </summary>
    class RNFastCryptoModule : NativeModuleBase
    {
        /// <summary>
        /// Instantiates the <see cref="RNFastCryptoModule"/>.
        /// </summary>
        internal RNFastCryptoModule()
        {

        }

        /// <summary>
        /// The name of the native module.
        /// </summary>
        public override string Name
        {
            get
            {
                return "RNFastCrypto";
            }
        }
    }
}
