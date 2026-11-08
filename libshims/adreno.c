/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <openssl/evp.h>
#include <openssl/rsa.h>

/* The stock Adreno driver was linked against OpenSSL, which BoringSSL lacks. */
int RSA_verify_PKCS1_PSS(RSA *rsa, const uint8_t *m_hash, const EVP_MD *hash,
                         const uint8_t *em, int s_len)
{
    return RSA_verify_PKCS1_PSS_mgf1(rsa, m_hash, hash, NULL, em, s_len);
}
