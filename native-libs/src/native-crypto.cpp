/*
 * Copyright (c) 2014, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */

#include "native-crypto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <serial_bridge_index.hpp>
#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>

void fast_crypto_monero_core(const char *szMethod, const char *szJsonParams, char **pszResult) {
    std::string strParams = szJsonParams;
    std::string method = szMethod;
    std::string result;

    try {
        if (method.compare("send_step1__prepare_params_for_get_decoys") == 0) {
            result = serial_bridge::send_step1__prepare_params_for_get_decoys(strParams);
        } else if (method.compare("send_step2__try_create_transaction") == 0) {
            result = serial_bridge::send_step2__try_create_transaction(strParams);
        } else if (method.compare("decode_address") == 0) {
            result = serial_bridge::decode_address(strParams);
        } else if (method.compare("is_subaddress") == 0) {
            result = serial_bridge::is_subaddress(strParams);
        } else if (method.compare("is_integrated_address") == 0) {
            result = serial_bridge::is_integrated_address(strParams);
        } else if (method.compare("new_integrated_address") == 0) {
            result = serial_bridge::new_integrated_address(strParams);
        } else if (method.compare("new_payment_id") == 0) {
            result = serial_bridge::new_payment_id(strParams);
        } else if (method.compare("newly_created_wallet") == 0) {
            result = serial_bridge::newly_created_wallet(strParams);
        } else if (method.compare("are_equal_mnemonics") == 0) {
            result = serial_bridge::are_equal_mnemonics(strParams);
        } else if (method.compare("address_and_keys_from_seed") == 0) {
            result = serial_bridge::address_and_keys_from_seed(strParams);
        } else if (method.compare("mnemonic_from_seed") == 0) {
            result = serial_bridge::mnemonic_from_seed(strParams);
        } else if (method.compare("seed_and_keys_from_mnemonic") == 0) {
            result = serial_bridge::seed_and_keys_from_mnemonic(strParams);
        } else if (method.compare("validate_components_for_login") == 0) {
            result = serial_bridge::validate_components_for_login(strParams);
        } else if (method.compare("estimated_tx_network_fee") == 0) {
            result = serial_bridge::estimated_tx_network_fee(strParams);
        } else if (method.compare("generate_key_image") == 0) {
            result = serial_bridge::generate_key_image(strParams);
        } else if (method.compare("generate_key_derivation") == 0) {
            result = serial_bridge::generate_key_derivation(strParams);
        } else if (method.compare("derive_public_key") == 0) {
            result = serial_bridge::derive_public_key(strParams);
        } else if (method.compare("derive_subaddress_public_key") == 0) {
            result = serial_bridge::derive_subaddress_public_key(strParams);
        } else if (method.compare("decodeRct") == 0) {
            result = serial_bridge::decodeRct(strParams);
        } else if (method.compare("decodeRctSimple") == 0) {
            result = serial_bridge::decodeRctSimple(strParams);
        } else if (method.compare("derivation_to_scalar") == 0) {
            result = serial_bridge::derivation_to_scalar(strParams);
        } else if (method.compare("encrypt_payment_id") == 0) {
            result = serial_bridge::encrypt_payment_id(strParams);
        } else if (method.compare("extract_utxos") == 0) {
            result = serial_bridge::extract_utxos(strParams);
        } else if (method.compare("sleep") == 0) {
            boost::this_thread::sleep_for(boost::chrono::milliseconds(15 * 1000));
            result = "{}";
        } else {
            *pszResult = NULL;
            return;
        }
    } catch (...) {
        result = "{\"err_msg\":\"mymonero-core-cpp threw an exception\"}";
    }
    int size = result.length() + 1;
    *pszResult = (char *) malloc(sizeof(char) * size);
    memcpy(*pszResult, result.c_str(), result.length() + 1);
}

