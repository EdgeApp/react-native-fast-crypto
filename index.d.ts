declare module 'react-native-fast-crypto' {
  /**
   * Derives a key using the scrypt key derivation function.
   *
   * @param password - The password as a Uint8Array
   * @param salt - The salt value as a Uint8Array
   * @param N - CPU/memory cost parameter (must be power of 2, e.g., 16384)
   * @param r - Block size parameter (typically 8)
   * @param p - Parallelization parameter (typically 1)
   * @param keyLength - Desired length of the derived key in bytes
   * @returns Promise resolving to derived key as Uint8Array
   *
   * @example
   * ```typescript
   * const password = new TextEncoder().encode('password123');
   * const salt = crypto.getRandomValues(new Uint8Array(32));
   * const key = await scrypt(password, salt, 16384, 8, 1, 32);
   * ```
   */
  export function scrypt(
    password: Uint8Array,
    salt: Uint8Array,
    N: number,
    r: number,
    p: number,
    keyLength: number
  ): Promise<Uint8Array>

  /**
   * PBKDF2 (Password-Based Key Derivation Function 2) namespace
   */
  export namespace pbkdf2 {
    /**
     * Derives a key using PBKDF2 with SHA-512.
     *
     * @param data - The password/passphrase as a Uint8Array
     * @param salt - The salt value as a Uint8Array
     * @param iterations - Number of iterations (recommended: 2048 or higher)
     * @param keyLength - Desired length of the derived key in bytes
     * @param algorithm - Must be 'sha512' (only supported algorithm)
     * @returns Promise resolving to derived key as Uint8Array
     * @throws {Error} If algorithm is not 'sha512'
     *
     * @example
     * ```typescript
     * const password = new TextEncoder().encode('my secure password');
     * const salt = new TextEncoder().encode('salt value');
     * const key = await pbkdf2.deriveAsync(password, salt, 2048, 64, 'sha512');
     * ```
     */
    function deriveAsync(
      data: Uint8Array,
      salt: Uint8Array,
      iterations: number,
      keyLength: number,
      algorithm: 'sha512'
    ): Promise<Uint8Array>
  }

  /**
   * secp256k1 elliptic curve operations namespace
   */
  export namespace secp256k1 {
    /**
     * Creates a public key from a private key.
     *
     * @param privateKey - The private key as a Uint8Array (must be 32 bytes)
     * @param compressed - Whether to return compressed (33 bytes) or uncompressed (65 bytes) public key
     * @returns Promise resolving to public key as Uint8Array
     *
     * @example
     * ```typescript
     * const privateKey = crypto.getRandomValues(new Uint8Array(32));
     * const publicKey = await secp256k1.publicKeyCreate(privateKey, true);
     * ```
     */
    function publicKeyCreate(
      privateKey: Uint8Array,
      compressed: boolean
    ): Promise<Uint8Array>

    /**
     * Adds a tweak value to a private key (used in HD wallet derivation).
     *
     * @param privateKey - The private key as a Uint8Array (must be 32 bytes)
     * @param tweak - The tweak value as a Uint8Array (must be 32 bytes)
     * @returns Promise resolving to tweaked private key as Uint8Array
     *
     * @example
     * ```typescript
     * const privateKey = new Uint8Array(32); // Your private key
     * const tweak = new Uint8Array(32); // Your tweak value
     * const tweakedKey = await secp256k1.privateKeyTweakAdd(privateKey, tweak);
     * ```
     */
    function privateKeyTweakAdd(
      privateKey: Uint8Array,
      tweak: Uint8Array
    ): Promise<Uint8Array>

    /**
     * Adds a tweak value to a public key (used in HD wallet derivation).
     *
     * @param publicKey - The public key as a Uint8Array (33 or 65 bytes)
     * @param tweak - The tweak value as a Uint8Array (must be 32 bytes)
     * @param compressed - Whether to return compressed or uncompressed public key
     * @returns Promise resolving to tweaked public key as Uint8Array
     *
     * @example
     * ```typescript
     * const publicKey = new Uint8Array(33); // Your compressed public key
     * const tweak = new Uint8Array(32); // Your tweak value
     * const tweakedPubKey = await secp256k1.publicKeyTweakAdd(publicKey, tweak, true);
     * ```
     */
    function publicKeyTweakAdd(
      publicKey: Uint8Array,
      tweak: Uint8Array,
      compressed: boolean
    ): Promise<Uint8Array>
  }
}
