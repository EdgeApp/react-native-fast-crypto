/*
 *  Copyright (c) 2014, Airbitz
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms are permitted provided that
 *  the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, this
 *  list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *  this list of conditions and the following disclaimer in the documentation
 *  and/or other materials provided with the distribution.
 *  3. Redistribution or use of modified source code requires the express written
 *  permission of Airbitz Inc.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  The views and conclusions contained in the software and documentation are those
 *  of the authors and should not be interpreted as representing official policies,
 *  either expressed or implied, of the Airbitz Project.
 */
/**
 * @file
 * AirBitz public API. The wallet only calls functions found in this file.
 */

#ifndef ABC_h
#define ABC_h

#include <stdbool.h>
#include <stdint.h>

/** The maximum buffer length for default strings in the system */
#define ABC_MAX_STRING_LENGTH 256

/** The ABC_ParseAmount function returns this when a string is malformed. */
#define ABC_INVALID_AMOUNT           ((int64_t)-1)

/** The number of decimal-place shifts needed to convert satoshi to bitcoin. */
#define ABC_BITCOIN_DECIMAL_PLACES   8

/** Frequency of exchange rate updates **/
#define ABC_EXCHANGE_RATE_REFRESH_INTERVAL_SECONDS 20

/** Denomination Type **/
#define ABC_DENOMINATION_BTC 0
#define ABC_DENOMINATION_MBTC 1
#define ABC_DENOMINATION_UBTC 2

#define ABC_MIN_USERNAME_LENGTH 3
#define ABC_MIN_PASS_LENGTH 10
#define ABC_MIN_PIN_LENGTH 4

#define ABC_GET_TX_ALL_TIMES 0

#ifdef __cplusplus
extern "C" {
#endif

/**
 * AirBitz Core Condition Codes
 *
 * All AirBitz Core functions return this code.
 * ABC_CC_OK indicates that there was no issue.
 * All other values indication some issue.
 *
 */
typedef enum eABC_CC
{
    /** The function completed without an error */
    ABC_CC_Ok = 0,
    /** An error occured */
    ABC_CC_Error = 1,
    /** Unexpected NULL pointer */
    ABC_CC_NULLPtr = 2,
    /** Max number of accounts have been created */
    ABC_CC_NoAvailAccountSpace = 3,
    /** Could not read directory */
    ABC_CC_DirReadError = 4,
    /** Could not open file */
    ABC_CC_FileOpenError = 5,
    /** Could not read from file */
    ABC_CC_FileReadError = 6,
    /** Could not write to file */
    ABC_CC_FileWriteError = 7,
    /** No such file */
    ABC_CC_FileDoesNotExist = 8,
    /** Unknown crypto type */
    ABC_CC_UnknownCryptoType = 9,
    /** Invalid crypto type */
    ABC_CC_InvalidCryptoType = 10,
    /** Decryption error */
    ABC_CC_DecryptError = 11,
    /** Decryption failure due to incorrect key */
    ABC_CC_DecryptFailure = 12,
    /** Encryption error */
    ABC_CC_EncryptError = 13,
    /** Scrypt error */
    ABC_CC_ScryptError = 14,
    /** Account already exists */
    ABC_CC_AccountAlreadyExists = 15,
    /** Account does not exist */
    ABC_CC_AccountDoesNotExist = 16,
    /** JSON parsing error */
    ABC_CC_JSONError = 17,
    /** Incorrect password */
    ABC_CC_BadPassword = 18,
    /** Wallet already exists */
    ABC_CC_WalletAlreadyExists = 19,
    /** URL call failure */
    ABC_CC_URLError = 20,
    /** An call to an external API failed  */
    ABC_CC_SysError = 21,
    /** No required initialization made  */
    ABC_CC_NotInitialized = 22,
    /** Initialization after already initializing  */
    ABC_CC_Reinitialization = 23,
    /** Server error  */
    ABC_CC_ServerError = 24,
    /** The user has not set recovery questions */
    ABC_CC_NoRecoveryQuestions = 25,
    /** Functionality not supported */
    ABC_CC_NotSupported = 26,
    /** Mutex error if some type */
    ABC_CC_MutexError = 27,
    /** Transaction not found */
    ABC_CC_NoTransaction = 28,
    ABC_CC_Empty_Wallet = 28, /* Deprecated */
    /** Failed to parse input text */
    ABC_CC_ParseError = 29,
    /** Invalid wallet ID */
    ABC_CC_InvalidWalletID = 30,
    /** Request (address) not found */
    ABC_CC_NoRequest = 31,
    /** Not enough money to send transaction */
    ABC_CC_InsufficientFunds = 32,
    /** We are still sync-ing */
    ABC_CC_Synchronizing = 33,
    /** Problem with the PIN */
    ABC_CC_NonNumericPin = 34,
    /** Unable to find an address */
    ABC_CC_NoAvailableAddress = 35,
    /** The user has entered a bad PIN, and must wait. */
    ABC_CC_InvalidPinWait = 36, ABC_CC_PinExpired = 36,
    /** Two Factor required */
    ABC_CC_InvalidOTP = 37,
    /** Trying to send too little money. */
    ABC_CC_SpendDust = 38,
    /** The server says app is obsolete and needs to be upgraded. */
    ABC_CC_Obsolete = 1000
} tABC_CC;

/**
 * AirBitz Core Error Structure
 *
 * This structure contains the detailed information associated
 * with an error.
 * Most AirBitz Core functions should offer the option of passing
 * a pointer to this structure to be filled out in the event of
 * error.
 *
 */
typedef struct sABC_Error
{
    /** The condition code code */
    tABC_CC code;
    /** String containing a description of the error */
    char szDescription[ABC_MAX_STRING_LENGTH + 1];
    /** String containing the function in which the error occurred */
    char szSourceFunc[ABC_MAX_STRING_LENGTH + 1];
    /** String containing the source file in which the error occurred */
    char szSourceFile[ABC_MAX_STRING_LENGTH + 1];
    /** Line number in the source file in which the error occurred */
    int  nSourceLine;
} tABC_Error;

/**
 * AirBitz Core Asynchronous BitCoin Event Type
 *
 */
typedef enum eABC_AsyncEventType
{
    ABC_AsyncEventType_IncomingBitCoin,
    ABC_AsyncEventType_BlockHeightChange,
    ABC_AsyncEventType_BalanceUpdate,
    ABC_AsyncEventType_AddressCheckDone,
    ABC_AsyncEventType_IncomingSweep,
    ABC_AsyncEventType_TransactionUpdate,
} tABC_AsyncEventType;

/**
 * Mining fee slider settings.
 */
typedef enum eABC_SpendFeeLevel
{
    ABC_SpendFeeLevelLow = 0,
    ABC_SpendFeeLevelStandard,
    ABC_SpendFeeLevelHigh,
    ABC_SpendFeeLevelCustom,
} tABC_SpendFeeLevel;

/**
 * AirBitz Core Asynchronous Structure
 *
 * This structure contains the detailed information associated
 * with an asynchronous BitCoin event.
 *
 */
typedef struct sABC_AsyncBitCoinInfo
{
    /** Data pointer given by caller. */
    void *pData;

    /** Type of event that occurred. */
    tABC_AsyncEventType eventType;

    /** The success or failure of the event. */
    tABC_Error status;

    /** If the event involved a wallet, this is its ID. */
    const char *szWalletUUID;

    /** If the event involved a transaction, this is its ID. */
    const char *szTxID;

    /** The amount swept, if this is a sweep. */
    int64_t sweepSatoshi;
} tABC_AsyncBitCoinInfo;

/**
 * AirBitz Currency Structure
 *
 * This structure contains the id's and names of all the currencies.
 *
 */
typedef struct sABC_Currency
{
    /** currency ISO 4217 code */
    const char *szCode;
    /** currency ISO 4217 num */
    int         num;
    /** currency description */
    const char *szDescription;
    /** currency countries */
    const char *szCountries;
} tABC_Currency;

/**
 * AirBitz Question Choice Structure
 *
 * This structure contains a recovery question choice.
 *
 */
typedef struct sABC_QuestionChoice
{
    /** question */
    const char *szQuestion;
    /** question category */
    const char *szCategory;
    /** miniumum length of an answer for this question */
    unsigned int    minAnswerLength;
} tABC_QuestionChoice;

/**
 * AirBitz Question Choices Structure
 *
 * This structure contains a recovery question choices.
 *
 */
typedef struct sABC_QuestionChoices
{
    /** number of choices */
    unsigned int        numChoices;
    /** array of choices */
    tABC_QuestionChoice **aChoices;
} tABC_QuestionChoices;

/**
 * AirBitz Transaction Details
 *
 * This structure contains details for transactions.
 * It is used in both transactions and transaction
 * requests.
 *
 */
typedef struct sABC_TxDetails
{
    /** amount of bitcoins in satoshi (including fees if any) */
    int64_t amountSatoshi;
    /** airbitz fees in satoshi (DEPRECATED, moved to tABC_TxInfo) */
    int64_t amountFeesAirbitzSatoshi;
    /** miners fees in satoshi (DEPRECATED, moved to tABC_TxInfo) */
    int64_t amountFeesMinersSatoshi;
    /** amount in currency */
    double amountCurrency;
    /** payer or payee */
    char *szName;
    /** payee business-directory id (0 otherwise) */
    unsigned int bizId;
    /** category for the transaction */
    char *szCategory;
    /** notes for the transaction */
    char *szNotes;
} tABC_TxDetails;

/**
 * AirBitz Output Info
 *
 * Contains the outputs used in a transaction
 *
 */
typedef struct sABC_TxOutput
{
    /** Was this output used as an input to a tx? **/
    bool     input;
    /** The number of satoshis used in the transaction **/
    int64_t  value;
    /** The coin address **/
    const char *szAddress;
} tABC_TxOutput;

/**
 * AirBitz Transaction Info
 *
 * This structure contains info for a transaction.
 *
 */
typedef struct sABC_TxInfo
{
    /** transaction identifier */
    const char *szID;
    /** time of creation */
    int64_t timeCreation;
    /** The net impact on the wallet balance. */
    int64_t balance;
    /** The mining fees paid. */
    int64_t minerFee;
    /** count of bitcoin addresses associated with this transaciton */
    unsigned int countOutputs;
    /** bitcoin addresses associated with this transaction */
    tABC_TxOutput **aOutputs;
    /** transaction details */
    tABC_TxDetails *pDetails;

    /** The Airbitz fee owed for this transaction. */
    int64_t airbitzFeeWanted;
    /** The Airbitz fee actually sent with this transaction. */
    int64_t airbitzFeeSent;

    /** The confirmation height of the transaction, or 0 for unconfirmed. */
    unsigned long height;
    /** True if the transaction is a double-spend. */
    bool bDoubleSpent;
    /** True if the transaction is eligible for replacement-by-fee. */
    bool bReplaceByFee;
} tABC_TxInfo;

/**
 * AirBitz Password Rule
 *
 * This structure contains info for a password rule.
 * When a password is checked, an array of these will
 * be delivered to explain what has an hasn't passed
 * for password requirements.
 *
 */
typedef struct sABC_PasswordRule
{
    /** description of the rule */
    const char *szDescription;
    /** has the password passed this requirement */
    bool bPassed;
} tABC_PasswordRule;

/**
 * All the fields that can be found in a URI, bitcoin address, or private key.
 */
typedef struct sABC_ParsedUri
{
    // Top-level actions:
    char *szAddress;
    char *szWif;
    char *szPaymentProto;
    char *szBitidUri;

    // URI parameters:
    uint64_t amountSatoshi;
    char *szLabel;
    char *szMessage;
    char *szCategory; // Airbitz extension
    char *szRet; // Airbitz extension

    // BitID metadata requests:
    bool bitidPaymentAddress;
    bool bitidKYCProvider;
    bool bitidKYCRequest;
} tABC_ParsedUri;

/**
 * A BIP70 payment request, fetched from the network and verified.
 */
typedef struct sABC_PaymentRequest
{
    /** True if the request is digitally signed (not all are). */
    bool bSigned;
    /** The certificate validation domain name. */
    char *szDomain;
    /** The total amount being requested. */
    uint64_t amountSatoshi;
    /** The payment protocol `memo` field (might be null). */
    char *szMemo;
    /** The merchant name, guessed using a regex (might be blank). */
    char *szMerchant;
    /** Pointer to core internal data. Do not touch! */
    void *pInternal;
} tABC_PaymentRequest;

/**
 * AirBitz Bitcoin Denomination
 *
 * This structure contains the method for
 * displaying bitcoin.
 *
 */
typedef struct sABC_BitcoinDenomination
{
    /** label (e.g., mBTC) */
    int denominationType;
    /** number of satoshi per unit (e.g., 100,000) */
    int64_t satoshi;
} tABC_BitcoinDenomination;

/**
 * AirBitz Account Settings
 *
 * This structure contains the user settings
 * for an account.
 *
 */
typedef struct sABC_AccountSettings
{
    /** first name (optional) */
    char                        *szFirstName;
    /** last name (optional) */
    char                        *szLastName;
    /** nickname (optional) */
    char                        *szNickname;
    /** PIN */
    char                        *szPIN;
    /** should name be listed on payments */
    bool                        bNameOnPayments;
    /** how many seconds before auto logout */
    int                         secondsAutoLogout;
    /** Number of times we have reminded the user to setup recovery q's */
    int                         recoveryReminderCount;
    /** language (ISO 639-1) */
    char                        *szLanguage;
    /** default ISO 4217 currency code */
    int                         currencyNum;
    /** bitcoin exchange rate source */
    char                        *szExchangeRateSource;
    /** how to display bitcoin denomination */
    tABC_BitcoinDenomination    bitcoinDenomination;
    /** use advanced features (e.g., allow offline wallet creation) */
    bool                        bAdvancedFeatures;
    /** fullname (readonly. Set by core based on first, last, nick names) */
    char                        *szFullName;
    /** should a daily spend limit be enforced */
    bool                        bDailySpendLimit;
    /** daily spend limit */
    int64_t                     dailySpendLimitSatoshis;
    /** should a daily spend limit be enforced */
    bool                        bSpendRequirePin;
    /** daily spend limit */
    int64_t                     spendRequirePinSatoshis;
    /** should PIN re-login be disabled */
    bool                        bDisablePINLogin;
    /** Count of successful pin logins */
    int                         pinLoginCount;
    /** should fingerprint based logins be disabled (ie. TouchID */
    bool                        bDisableFingerprintLogin;
    /** Should use user override bitcoin servers */
    bool                        bOverrideBitcoinServers;
    /** List of override bitcoin servers */
    char                        *szOverrideBitcoinServerList;
} tABC_AccountSettings;

/**
 * AirBitz Asynchronous BitCoin event callback
 *
 * This is the form of the callback that will be called when there is an
 * asynchronous BitCoin event.
 *
 */
typedef void (*tABC_BitCoin_Event_Callback)(const tABC_AsyncBitCoinInfo *pInfo);

/* === Library lifetime: === */

/**
 * Initialize the AirBitz Core library.
 * @param szRootDir             The root directory for all files to be saved
 * @param szCaCertPath          CA Certificate Path
 * @param szApiKey              API Key for the AirBitz login servers
 * @param szHiddenBitsKey       XOR key for Hiddenbits encoding
 * @param szAccountType         The type ID for the account repo
 * @param pSeedData             Pointer to data to seed the random number generator
 * @param seedLength            Length of the seed data
 * @param pError                A pointer to the location to store the error if there is one
 */
tABC_CC ABC_Initialize(const char               *szRootDir,
                       const char               *szCaCertPath,
                       const char               *szApiKey,
                       const char               *szAccountType,
                       const char               *szHiddenBitsKey,
                       const unsigned char      *pSeedData,
                       unsigned int             seedLength,
                       tABC_Error               *pError);

void ABC_Terminate();

void ABC_Log(const char *szMessage);

tABC_CC ABC_Version(char **szVersion, tABC_Error *pError);

tABC_CC ABC_IsTestNet(bool *pResult, tABC_Error *pError);

/* === All data at once: === */
tABC_CC ABC_ClearKeyCache(tABC_Error *pError);

/* === General info: === */

/**
 * Fetches the general info from the auth server
 * (obelisk servers, mining fees, &c.).
 */
tABC_CC ABC_GeneralInfoUpdate(tABC_Error *pError);

tABC_CC ABC_GetCurrencies(tABC_Currency **paCurrencyArray,
                          int *pCount,
                          tABC_Error *pError);

tABC_CC ABC_GetQuestionChoices(tABC_QuestionChoices **pOut,
                               tABC_Error *pError);

void ABC_FreeQuestionChoices(tABC_QuestionChoices *pQuestionChoices);

/* === Tools: === */
tABC_CC ABC_ParseAmount(const char *szAmount,
                        uint64_t *pAmountOut,
                        unsigned decimalPlaces);

tABC_CC ABC_FormatAmount(int64_t amount,
                         char **pszAmountOut,
                         unsigned decimalPlaces,
                         bool bAddSign,
                         tABC_Error *pError);

tABC_CC ABC_CheckPassword(const char *szPassword,
                          double *pSecondsToCrack,
                          tABC_PasswordRule ***paRules,
                          unsigned int *pCountRules,
                          tABC_Error *pError);

void ABC_FreePasswordRuleArray(tABC_PasswordRule **aRules,
                               unsigned int nCount);

tABC_CC ABC_QrEncode(const char *szText,
                     unsigned char **paData,
                     unsigned int *pWidth,
                     tABC_Error *pError);

/**
 * Generates a random private key in the hbits format.
 */
tABC_CC ABC_CreateHbits(char **pszResult,
                        char **pszAddress,
                        tABC_Error *pError);

/**
 * Creates a URI for a payment request.
 * Pass this through `ABC_QrEncode` to get a QR code.
 * @param amountSatoshi optional amount. Ignored if zero.
 * @param szLabel optional label. Can be null.
 * @param szMessage optional message. Can be null.
 */
tABC_CC ABC_AddressUriEncode(const char *szAddress,
                             uint64_t amountSatoshi,
                             const char *szLabel,
                             const char *szMessage,
                             const char *szCategory,
                             const char *szRet,
                             char **pszResult,
                             tABC_Error *pError);

/**
 * Parses a bitcoin URI, bitid URI, address, or private key.
 */
tABC_CC ABC_ParseUri(char *szURI,
                     tABC_ParsedUri **ppResult,
                     tABC_Error *pError);

void ABC_FreeParsedUri(tABC_ParsedUri *pUri);

/**
 * Fetches a BIP70 payment request from a server.
 * @param szRequestUri The request URI,
 * typically parsed out of the `r` parameter of a larger bitcoin URI.
 */
tABC_CC ABC_FetchPaymentRequest(char *szRequestUri,
                                tABC_PaymentRequest **ppResult,
                                tABC_Error *pError);

void ABC_FreePaymentRequest(tABC_PaymentRequest *pRequest);

/* === Communications lobby: === */

/**
 * Frees a lobby handle obtained from `ABC_FetchLobby`.
 */
void ABC_FreeLobby(int hLobby);

/**
 * Fetches the contents of a communications lobby.
 * @param phResult A handle to the result.
 * @param szId The lobby ID, typically parsed out of a barcode or such.
 */
tABC_CC ABC_FetchLobby(char *szId,
                       int *phResult,
                       tABC_Error *pError);

/**
 * Returns the account request contained within a lobby
 * (or an error if there is none).
 */
tABC_CC ABC_GetLobbyAccountRequest(int hLobby,
                                   char **pszType,
                                   char **pszDisplayName,
                                   char **pszDisplayImageUrl,
                                   tABC_Error *pError);

/**
 * Approves the account request contained in the given lobby,
 * creating a repo if needed.
 */
tABC_CC ABC_ApproveLobbyAccountRequest(const char *szUserName,
                                       const char *szPassword,
                                       int hLobby,
                                       tABC_Error *pError);

/* === Login lifetime: === */

/**
 * Transforms a username into the internal format used for hashing.
 * This collapses spaces, converts things to lowercase,
 * and checks for invalid characters.
 */
tABC_CC ABC_FixUsername(char **pszResult,
                        const char *szUserName,
                        tABC_Error *pError);

/**
 * Grabs an array of message JSON objects from the auth server.
 * These objects contain status flags the user should be informed about.
 * @param pszJsonResult A string holding the JSON results.
 */
tABC_CC ABC_GetLoginMessages(char **pszJsonResult,
                             tABC_Error *pError);

/**
 * Grabs the login packages from disks.
 * Tbis is useful for brute-forcing forgotten passwords.
 */
tABC_CC ABC_GetLoginPackages(char **pszResult,
                             const char *szUserName,
                             tABC_Error *pError);

/**
 * Logs the user in using a password.
 * @param pszOtpResetToken returned OTP reset token, if any.
 * @param pszOtpResetDate returned OTP reset date, if any.
 */
tABC_CC ABC_PasswordLogin(const char *szUserName,
                          const char *szPassword,
                          char **pszOtpResetToken,
                          char **pszOtpResetDate,
                          tABC_Error *pError);

tABC_CC ABC_AccountAvailable(const char *szUserName,
                             tABC_Error *pError);

tABC_CC ABC_CreateAccount(const char *szUserName,
                          const char *szPassword,
                          tABC_Error *pError);

tABC_CC ABC_AccountDelete(const char *szUserName,
                          tABC_Error *pError);

tABC_CC ABC_GetRecoveryQuestions(const char *szUserName,
                                 char **pszQuestions,
                                 tABC_Error *pError);

/**
 * Logs the user in using their recovery answers.
 * @param szRecoveryAnswers newline-separated recovery answers.
 * @param pszOtpResetToken returned OTP reset token, if any.
 * @param pszOtpResetDate returned OTP reset date, if any.
 */
tABC_CC ABC_RecoveryLogin(const char *szUserName,
                          const char *szRecoveryAnswers,
                          char **pszOtpResetToken,
                          char **pszOtpResetDate,
                          tABC_Error *pError);

tABC_CC ABC_PinLoginExists(const char *szUserName,
                           bool *pbExists,
                           tABC_Error *pError);

/**
 * Performs a PIN-based login for the given user.
 * @param pWaitSeconds seconds before the next retry, if this one failed.
 */
tABC_CC ABC_PinLogin(const char *szUserName,
                     const char *szPin,
                     int *pWaitSeconds,
                     tABC_Error *pError);

/**
 * Sets up the data for a pin-based login, both on disk and on the server.
 */
tABC_CC ABC_PinSetup(const char *szUserName,
                     const char *szPassword,
                     const char *szPin,
                     tABC_Error *pError);

/**
 * Checks a PIN for correctness.
 * This is used to guard access to certain actions in the GUI.
 */
tABC_CC ABC_PinCheck(const char *szUserName,
                     const char *szPassword,
                     const char *szPin,
                     bool *pbResult,
                     tABC_Error *pError);

tABC_CC ABC_ListAccounts(char **pszUserNames,
                         tABC_Error *pError);

/**
 * Attempts to look up the recovery2Key for a particular user.
 */
tABC_CC ABC_Recovery2Key(const char *szUserName,
                         char **pszKey,
                         tABC_Error *pError);

/**
 * Obtains the recovery v2 questions from the server.
 * @param szKey the recovery key.
 */
tABC_CC ABC_Recovery2Questions(const char *szUserName,
                               const char *szKey,
                               char ***paszQuestions,
                               unsigned int *pCount,
                               tABC_Error *pError);

/**
 * Logs the user in using their recovery v2 key and answers.
 */
tABC_CC ABC_Recovery2Login(const char *szUserName,
                           const char *szKey,
                           char *aszAnswer1,
                           char *aszAnswer2,
                           char **pszOtpResetToken,
                           char **pszOtpResetDate,
                           tABC_Error *pError);

/**
 * Installs recovery 2 questions and answers into the given login.
 * @param pszKey The recovery 2 key. This should be stored in a safe place.
 */
tABC_CC ABC_Recovery2Setup(const char *szUserName,
                           const char *szPassword,
                           char *aszQuestion1,
                           char *aszAnswer1,
                           char *aszQuestion2,
                           char *aszAnswer2,
                           char **pszKey,
                           tABC_Error *pError);

/**
 * Removes the recovery v2 questions from the given login.
 */
tABC_CC ABC_Recovery2Delete(const char *szUserName,
                            const char *szPassword,
                            tABC_Error *pError);

/**
 * Obtains a hex-encoded decryption key that can be used for future logins.
 */
tABC_CC ABC_GetLoginKey(const char *szUserName,
                        const char *szPassword,
                        char **pszKey,
                        tABC_Error *pError);

/**
 * Logs the user in with a decryption key.
 */
tABC_CC ABC_KeyLogin(const char *szUserName,
                     const char *szKey,
                     tABC_Error *pError);

/* === Login data: === */
tABC_CC ABC_ChangePassword(const char *szUserName,
                           const char *szPassword,
                           const char *szNewPassword,
                           tABC_Error *pError);

tABC_CC ABC_SetAccountRecoveryQuestions(const char *szUserName,
                                        const char *szPassword,
                                        const char *szRecoveryQuestions,
                                        const char *szRecoveryAnswers,
                                        tABC_Error *pError);

tABC_CC ABC_PasswordOk(const char *szUserName,
                       const char *szPassword,
                       bool *pOk,
                       tABC_Error *pError);

tABC_CC ABC_PasswordExists(const char *szUserName,
                           bool *pExists,
                           tABC_Error *pError);

/* === OTP authentication: === */

/**
 * Obtains the OTP key stored for the given username, if any.
 * @param pszKey a pointer to receive the key. The caller frees this.
 * @return An error if the OTP token does not exist, or is unreadable.
 */
tABC_CC ABC_OtpKeyGet(const char *szUserName,
                      char **pszKey,
                      tABC_Error *pError);

/**
 * Associates an OTP key with the given username.
 * This will not write to disk until the user has successfully logged in
 * at least once.
 */
tABC_CC ABC_OtpKeySet(const char *szUserName,
                      char *szKey,
                      tABC_Error *pError);

/**
 * Removes the OTP key associated with the given username.
 * This will remove the key from disk as well.
 */
tABC_CC ABC_OtpKeyRemove(const char *szUserName,
                         tABC_Error *pError);

/**
 * Reads the OTP configuration from the server.
 */
tABC_CC ABC_OtpAuthGet(const char *szUserName,
                       const char *szPassword,
                       bool *pbEnabled,
                       long *pTimeout,
                       tABC_Error *pError);

/**
 * Sets up OTP authentication on the server.
 * This will generate a new token if the username doesn't already have one.
 * @param timeout Reset time, in seconds.
 */
tABC_CC ABC_OtpAuthSet(const char *szUserName,
                       const char *szPassword,
                       long timeout,
                       tABC_Error *pError);

/**
 * Removes the OTP authentication requirement from the server.
 */
tABC_CC ABC_OtpAuthRemove(const char *szUserName,
                          const char *szPassword,
                          tABC_Error *pError);

/**
 * Returns the reset status for all accounts currently on the device.
 * @return A newline-separated list of usernames with pending resets.
 * The caller frees this.
 */
tABC_CC ABC_OtpResetGet(char **szUsernames,
                        tABC_Error *pError);

/**
 * Launches an OTP reset timer on the server,
 * which will disable the OTP authentication requirement when it expires.
 *
 * This only works after the caller has successfully authenticated
 * with the server, such as through a password login,
 * but has failed to fully log in due to a missing OTP key.
 */
tABC_CC ABC_OtpResetSet(const char *szUserName,
                        const char *szToken,
                        tABC_Error *pError);

/**
 * Cancels an OTP reset timer.
 */
tABC_CC ABC_OtpResetRemove(const char *szUserName,
                           const char *szPassword,
                           tABC_Error *pError);

/** === BitID: === */

/**
 * Validates a BitID URI.
 * @param pszDomain returned domain name to show to the user for confirmation.
 */
tABC_CC ABC_BitidParseUri(const char *szUserName,
                          const char *szPassword,
                          const char *szBitidURI,
                          char **pszDomain,
                          char **pszBitidCallbackURI,
                          tABC_Error *pError);

tABC_CC ABC_BitidLogin(const char *szUserName,
                       const char *szPassword,
                       const char *szBitidURI,
                       tABC_Error *pError);

tABC_CC ABC_BitidLoginMeta(const char *szUserName,
                           const char *szPassword,
                           const char *szBitidURI,
                           const char *szWalletUUID,
                           const char *szBitIDKYCURI,
                           tABC_Error *pError);

/**
 * Signs a message using a BitID key.
 * @param szBitidURI the URI to derive the private key from.
 * @param szMessage the message to sign.
 * @param pszBitidAddress the public address derived from the key.
 * @param pszBitidSignature the signed message.
 */
tABC_CC ABC_BitidSign(const char *szUserName,
                      const char *szPassword,
                      const char *szBitidURI,
                      const char *szMessage,
                      char **pszBitidAddress,
                      char **pszBitidSignature,
                      tABC_Error *pError);

/* === Account sync data: === */

/**
 * Returns true if the sync directory exists for the given username.
 */
tABC_CC ABC_AccountSyncExists(const char *szUserName,
                              bool *pResult,
                              tABC_Error *pError);

tABC_CC ABC_LoadAccountSettings(const char *szUserName,
                                const char *szPassword,
                                tABC_AccountSettings **ppSettings,
                                tABC_Error *pError);

tABC_CC ABC_UpdateAccountSettings(const char *szUserName,
                                  const char *szPassword,
                                  tABC_AccountSettings *pSettings,
                                  tABC_Error *pError);

void ABC_FreeAccountSettings(tABC_AccountSettings *pSettings);

tABC_CC ABC_GetCategories(const char *szUserName,
                          const char *szPassword,
                          char ***paszCategories,
                          unsigned int *pCount,
                          tABC_Error *pError);

tABC_CC ABC_AddCategory(const char *szUserName,
                        const char *szPassword,
                        char *szCategory,
                        tABC_Error *pError);

tABC_CC ABC_RemoveCategory(const char *szUserName,
                           const char *szPassword,
                           char *szCategory,
                           tABC_Error *pError);

tABC_CC ABC_DataSyncAccount(const char *szUserName,
                            const char *szPassword,
                            bool *pbDirty,
                            bool *pbPasswordChanged,
                            tABC_Error *pError);

tABC_CC ABC_UploadLogs(const char *szUserName,
                       const char *szPassword,
                       tABC_Error *pError);

/** === Plugin data: === */

/**
 * Lists the plugin key/value stores in an account.
 *
 * Anybody who uses this is probably doing something wrong.
 * This mainly exists for diagnostics,
 * since the idea is that all plugins are sandboxed by their plugin id.
 * There shouldn't be any code that needs to list the plugin id's,
 * since the list should be static and know ahead of time.
 */
tABC_CC ABC_PluginDataList(const char *szUserName,
                           const char *szPassword,
                           char ***paszPlugins,
                           unsigned int *pCount,
                           tABC_Error *pError);

/**
 * Lists the keys in a plugin key/value store.
 * @param szPlugin The plugin's unique ID.
 */
tABC_CC ABC_PluginDataKeys(const char *szUserName,
                           const char *szPassword,
                           const char *szPlugin,
                           char ***paszKeys,
                           unsigned int *pCount,
                           tABC_Error *pError);

/**
 * Retreives an item from the plugin key/value store.
 * @param szPlugin The plugin's unique ID.
 * @param szKey The data location. Merges happen at the key level,
 * so the account may contain a mix of keys from different devices.
 * The key contents are atomic, however. Place data accordingly.
 * @param szData The value stored with the key.
 */
tABC_CC ABC_PluginDataGet(const char *szUserName,
                          const char *szPassword,
                          const char *szPlugin,
                          const char *szKey,
                          char **pszData,
                          tABC_Error *pError);

/**
 * Saves an item to the plugin key/value store.
 */
tABC_CC ABC_PluginDataSet(const char *szUserName,
                          const char *szPassword,
                          const char *szPlugin,
                          const char *szKey,
                          const char *szData,
                          tABC_Error *pError);

/**
 * Deletes an item from the plugin key/value store.
 */
tABC_CC ABC_PluginDataRemove(const char *szUserName,
                             const char *szPassword,
                             const char *szPlugin,
                             const char *szKey,
                             tABC_Error *pError);

/**
 * Removes the entire key/value store for a particular plugin.
 */
tABC_CC ABC_PluginDataClear(const char *szUserName,
                            const char *szPassword,
                            const char *szPlugin,
                            tABC_Error *pError);

/* === Account wallet list: === */

tABC_CC ABC_GetWalletUUIDs(const char *szUserName,
                           const char *szPassword,
                           char ***paWalletUUID,
                           unsigned int *pCount,
                           tABC_Error *pError);

tABC_CC ABC_SetWalletOrder(const char *szUserName,
                           const char *szPassword,
                           const char *szUUIDs,
                           tABC_Error *pError);

/**
 * Removes a wallet from the account wallet list.
 */
tABC_CC ABC_WalletRemove(const char *szUserName,
                         const char *szWalletUUID,
                         tABC_Error *pError);

/**
 * Determines whether or not the wallet is archived.
 */
tABC_CC ABC_WalletArchived(const char *szUserName,
                           const char *szWalletUUID,
                           bool *pResult,
                           tABC_Error *pError);

tABC_CC ABC_SetWalletArchived(const char *szUserName,
                              const char *szPassword,
                              const char *szUUID,
                              unsigned int archived,
                              tABC_Error *pError);

/* === Exchange rates: === */
tABC_CC ABC_RequestExchangeRateUpdate(const char *szUserName,
                                      const char *szPassword,
                                      int currencyNum,
                                      tABC_Error *pError);

tABC_CC ABC_SatoshiToCurrency(const char *szUserName,
                              const char *szPassword,
                              int64_t satoshi,
                              double *pCurrency,
                              int currencyNum,
                              tABC_Error *pError);

tABC_CC ABC_CurrencyToSatoshi(const char *szUserName,
                              const char *szPassword,
                              double currency,
                              int currencyNum,
                              int64_t *pSatoshi,
                              tABC_Error *pError);

/* === Wallet data: === */
tABC_CC ABC_CreateWallet(const char *szUserName,
                         const char *szPassword,
                         const char *szWalletName,
                         int        currencyNum,
                         char       **pszUuid,
                         tABC_Error *pError);

/**
 * Loads a wallet into memory, doing the initial sync if necessary.
 */
tABC_CC ABC_WalletLoad(const char *szUserName,
                       const char *szWalletUUID,
                       tABC_Error *pError);

/**
 * Obtains the wallet's text name.
 */
tABC_CC ABC_WalletName(const char *szUserName,
                       const char *szWalletUUID,
                       char **pszResult,
                       tABC_Error *pError);

/**
 * Obtains the wallet's currency code.
 */
tABC_CC ABC_WalletCurrency(const char *szUserName,
                           const char *szWalletUUID,
                           int *pResult,
                           tABC_Error *pError);

/**
 * Calculates the wallet's balance.
 */
tABC_CC ABC_WalletBalance(const char *szUserName,
                          const char *szWalletUUID,
                          int64_t *pResult,
                          tABC_Error *pError);

tABC_CC ABC_RenameWallet(const char *szUserName,
                         const char *szPassword,
                         const char *szUUID,
                         const char *szNewWalletName,
                         tABC_Error *pError);

tABC_CC ABC_ExportWalletSeed(const char *szUserName,
                             const char *szPassword,
                             const char *szUUID,
                             char **pszWalletSeed,
                             tABC_Error *pError);

/**
 * Export the HD public seed (XPub) used to generate all addresses within a wallet.
 */
tABC_CC ABC_ExportWalletXPub(const char *szUserName,
                             const char *szPassword,
                             const char *szWalletUUID,
                             char **pszWalletXPub,
                             tABC_Error *pError);

tABC_CC ABC_CsvExport(const char *szUserName,
                      const char *szPassword,
                      const char *szUUID,
                      int64_t startTime,
                      int64_t endTime,
                      char **szCsvData,
                      tABC_Error *pError);

tABC_CC ABC_QBOExport(const char *szUserName,
                      const char *szPassword,
                      const char *szUUID,
                      int64_t startTime,
                      int64_t endTime,
                      char **szQBOData,
                      tABC_Error *pError);

tABC_CC ABC_DataSyncWallet(const char *szUserName,
                           const char *szPassword,
                           const char *szWalletUUID,
                           bool *pbDirty,
                           tABC_Error *pError);

/* === Receiving: === */
tABC_CC ABC_CreateReceiveRequest(const char *szUserName,
                                 const char *szPassword,
                                 const char *szWalletUUID,
                                 char **pszRequestID,
                                 tABC_Error *pError);

tABC_CC ABC_ModifyReceiveRequest(const char *szUserName,
                                 const char *szPassword,
                                 const char *szWalletUUID,
                                 const char *szRequestID,
                                 tABC_TxDetails *pDetails,
                                 tABC_Error *pError);

tABC_CC ABC_FinalizeReceiveRequest(const char *szUserName,
                                   const char *szPassword,
                                   const char *szWalletUUID,
                                   const char *szRequestID,
                                   tABC_Error *pError);

tABC_CC ABC_CancelReceiveRequest(const char *szUserName,
                                 const char *szPassword,
                                 const char *szWalletUUID,
                                 const char *szRequestID,
                                 tABC_Error *pError);

/* === Spending: === */

void ABC_SpendFree(void *pSpend);

/**
 * Creates a new, blank spend object.
 */
tABC_CC ABC_SpendNew(const char *szUserName,
                     const char *szWalletUUID,
                     void **ppResult,
                     tABC_Error *pError);

/**
 * Adds a bitcoin payment address output to a spend.
 */
tABC_CC ABC_SpendAddAddress(void *pSpend,
                            const char *szAddress,
                            uint64_t amount,
                            tABC_Error *pError);

/**
 * Adds a BIP70 payment request output to a spend.
 */
tABC_CC ABC_SpendAddPaymentRequest(void *pSpend,
                                   tABC_PaymentRequest *pRequest,
                                   tABC_Error *pError);

/**
 * Adds a wallet-to-wallet transfer output to a spend.
 * @param szWalletUUID the destination wallet.
 * @param tABC_TxDetails metadata to save in the target wallet.
 * The fiat amount will be calculated from the amount if it is zero.
 * Otherwise, the fiat amount will be used as-is.
 */
tABC_CC ABC_SpendAddTransfer(void *pSpend,
                             const char *szWalletUUID,
                             uint64_t amount,
                             tABC_TxDetails *pDetails,
                             tABC_Error *pError);

/**
 * Provides metadata to be saved alongside the transaction.
 * The Airbitz fee will be updated to match whatever was sent,
 * and the fiat amount will be calculated from the total if it is zero.
 * Otherwise, the fiat amount will be used as-is.
 */
tABC_CC ABC_SpendSetMetadata(void *pSpend,
                             tABC_TxDetails *pDetails,
                             tABC_Error *pError);

/**
 * Change the desired fee level of the current spend
 */
tABC_CC ABC_SpendSetFee(void *pSpend,
                        tABC_SpendFeeLevel feeLevel,
                        uint64_t customFeeSatoshi,
                        tABC_Error *pError);

/**
 * Calculate the fee needed to perform this spend.
 * @return ABC_CC_InsufficientFunds if the source doesn't have enough money.
 */
tABC_CC ABC_SpendGetFee(void *pSpend,
                        uint64_t *pFee,
                        tABC_Error *pError);

/**
 * Finds the maximum amount that could be sent to this collection of outputs.
 */
tABC_CC ABC_SpendGetMax(void *pSpend,
                        uint64_t *pMax,
                        tABC_Error *pError);

/**
 * Creates and signs a transaction.
 * @param pszRawTx the resulting raw transaction, in hex.
 */
tABC_CC ABC_SpendSignTx(void *pSpend,
                        char **pszRawTx,
                        tABC_Error *pError);

/**
 * Broadcasts a transaction to the bitcoin network.
 */
tABC_CC ABC_SpendBroadcastTx(void *pSpend,
                             char *szRawTx,
                             tABC_Error *pError);

/**
 * Saves a transaction to the wallet database.
 */
tABC_CC ABC_SpendSaveTx(void *pSpend,
                        char *szRawTx,
                        char **pszTxId,
                        tABC_Error *pError);

/**
 * Signs, broadcasts, and saves a payment.
 * @param szWalletUUID the funds source.
 */
tABC_CC ABC_SpendApprove(void *pSpend,
                         char **pszTxId,
                         tABC_Error *pError);

/**
 * Sweeps a private key into the wallet.
 * The core will fire a callback when the sweep is done.
 * @param szKey  Private key in WIF format.
 * Use `ABC_ParseUri` to convert other formats into WIF.
 */
tABC_CC ABC_SweepKey(const char *szUsername,
                     const char *szPassword,
                     const char *szWalletUUID,
                     const char *szKey,
                     tABC_Error *pError);

/* === Transactions: === */
tABC_CC ABC_GetTransaction(const char *szUserName,
                           const char *szPassword,
                           const char *szWalletUUID,
                           const char *szID,
                           tABC_TxInfo **ppTransaction,
                           tABC_Error *pError);

tABC_CC ABC_GetTransactions(const char *szUserName,
                            const char *szPassword,
                            const char *szWalletUUID,
                            int64_t startTime,
                            int64_t endTime,
                            tABC_TxInfo ***paTransactions,
                            unsigned int *pCount,
                            tABC_Error *pError);

tABC_CC ABC_SearchTransactions(const char *szUserName,
                               const char *szPassword,
                               const char *szWalletUUID,
                               const char *szQuery,
                               tABC_TxInfo ***paTransactions,
                               unsigned int *pCount,
                               tABC_Error *pError);

void ABC_FreeTransaction(tABC_TxInfo *pTransaction);

void ABC_FreeTransactions(tABC_TxInfo **aTransactions,
                          unsigned int count);

tABC_CC ABC_SetTransactionDetails(const char *szUserName,
                                  const char *szPassword,
                                  const char *szWalletUUID,
                                  const char *szID,
                                  tABC_TxDetails *pDetails,
                                  tABC_Error *pError);

tABC_CC ABC_GetTransactionDetails(const char *szUserName,
                                  const char *szPassword,
                                  const char *szWalletUUID,
                                  const char *szID,
                                  tABC_TxDetails **ppDetails,
                                  tABC_Error *pError);

void ABC_FreeTxDetails(tABC_TxDetails *pDetails);

/* === Wallet watcher: === */
tABC_CC ABC_WatcherStart(const char *szUserName,
                         const char *szPassword,
                         const char *szWalletUUID,
                         tABC_Error *pError);

tABC_CC ABC_WatcherLoop(const char *szWalletUUID,
                        tABC_BitCoin_Event_Callback fAsyncBitCoinEventCallback,
                        void *pData,
                        tABC_Error *pError);

tABC_CC ABC_WatcherConnect(const char *szWalletUUID, tABC_Error *pError);

tABC_CC ABC_PrioritizeAddress(const char *szUserName, const char *szPassword,
                              const char *szWalletUUID, const char *szAddress,
                              tABC_Error *pError);

tABC_CC ABC_WatcherDisconnect(const char *szWalletUUID, tABC_Error *pError);

tABC_CC ABC_WatcherStop(const char *szWalletUUID, tABC_Error *pError);

tABC_CC ABC_WatcherDelete(const char *szWalletUUID, tABC_Error *pError);

tABC_CC ABC_WatcherDeleteCache(const char *szWalletUUID, tABC_Error *pError);

tABC_CC ABC_TxHeight(const char *szWalletUUID, const char *szTxId, int *height,
                     tABC_Error *pError);

tABC_CC ABC_BlockHeight(const char *szWalletUUID, int *height,
                        tABC_Error *pError);

#ifdef __cplusplus
}
#endif

#endif
