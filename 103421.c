static EVP_PKEY * php_openssl_generate_private_key(struct php_x509_request * req)
{
	char * randfile = NULL;
	int egdsocket, seeded;
	EVP_PKEY * return_val = NULL;

	if (req->priv_key_bits < MIN_KEY_LENGTH) {
		php_error_docref(NULL, E_WARNING, "private key length is too short; it needs to be at least %d bits, not %d",
				MIN_KEY_LENGTH, req->priv_key_bits);
		return NULL;
	}

	randfile = CONF_get_string(req->req_config, req->section_name, "RANDFILE");
	if (randfile == NULL) {
		php_openssl_store_errors();
	}
	php_openssl_load_rand_file(randfile, &egdsocket, &seeded);

	if ((req->priv_key = EVP_PKEY_new()) != NULL) {
		switch(req->priv_key_type) {
			case OPENSSL_KEYTYPE_RSA:
				{
					RSA* rsaparam;
#if OPENSSL_VERSION_NUMBER < 0x10002000L
					/* OpenSSL 1.0.2 deprecates RSA_generate_key */
					PHP_OPENSSL_RAND_ADD_TIME();
					rsaparam = (RSA*)RSA_generate_key(req->priv_key_bits, RSA_F4, NULL, NULL);
#else
					{
						BIGNUM *bne = (BIGNUM *)BN_new();
						if (BN_set_word(bne, RSA_F4) != 1) {
							BN_free(bne);
							php_error_docref(NULL, E_WARNING, "failed setting exponent");
							return NULL;
						}
						rsaparam = RSA_new();
						PHP_OPENSSL_RAND_ADD_TIME();
						if (rsaparam == NULL || !RSA_generate_key_ex(rsaparam, req->priv_key_bits, bne, NULL)) {
							php_openssl_store_errors();
						}
						BN_free(bne);
					}
#endif
					if (rsaparam && EVP_PKEY_assign_RSA(req->priv_key, rsaparam)) {
						return_val = req->priv_key;
					} else {
						php_openssl_store_errors();
					}
				}
				break;
#if !defined(NO_DSA)
			case OPENSSL_KEYTYPE_DSA:
				PHP_OPENSSL_RAND_ADD_TIME();
				{
					DSA *dsaparam = DSA_new();
					if (dsaparam && DSA_generate_parameters_ex(dsaparam, req->priv_key_bits, NULL, 0, NULL, NULL, NULL)) {
						DSA_set_method(dsaparam, DSA_get_default_method());
						if (DSA_generate_key(dsaparam)) {
							if (EVP_PKEY_assign_DSA(req->priv_key, dsaparam)) {
								return_val = req->priv_key;
							} else {
								php_openssl_store_errors();
							}
						} else {
							php_openssl_store_errors();
							DSA_free(dsaparam);
						}
					} else {
						php_openssl_store_errors();
					}
				}
				break;
#endif
#if !defined(NO_DH)
			case OPENSSL_KEYTYPE_DH:
				PHP_OPENSSL_RAND_ADD_TIME();
				{
					int codes = 0;
					DH *dhparam = DH_new();
					if (dhparam && DH_generate_parameters_ex(dhparam, req->priv_key_bits, 2, NULL)) {
						DH_set_method(dhparam, DH_get_default_method());
						if (DH_check(dhparam, &codes) && codes == 0 && DH_generate_key(dhparam)) {
							if (EVP_PKEY_assign_DH(req->priv_key, dhparam)) {
								return_val = req->priv_key;
							} else {
								php_openssl_store_errors();
							}
						} else {
							php_openssl_store_errors();
							DH_free(dhparam);
						}
					} else {
						php_openssl_store_errors();
					}
				}
				break;
#endif
#ifdef HAVE_EVP_PKEY_EC
			case OPENSSL_KEYTYPE_EC:
				{
					if (req->curve_name == NID_undef) {
						php_error_docref(NULL, E_WARNING, "Missing configuration value: 'curve_name' not set");
						return NULL;
					}
					EC_KEY *eckey = EC_KEY_new_by_curve_name(req->curve_name);
					if (eckey) {
						EC_KEY_set_asn1_flag(eckey, OPENSSL_EC_NAMED_CURVE);
						if (EC_KEY_generate_key(eckey) &&
							EVP_PKEY_assign_EC_KEY(req->priv_key, eckey)) {
							return_val = req->priv_key;
						} else {
							EC_KEY_free(eckey);
						}
					}
				}
				break;
#endif
			default:
				php_error_docref(NULL, E_WARNING, "Unsupported private key type");
		}
	} else {
		php_openssl_store_errors();
	}

	php_openssl_write_rand_file(randfile, egdsocket, seeded);

	if (return_val == NULL) {
		EVP_PKEY_free(req->priv_key);
		req->priv_key = NULL;
		return NULL;
	}

	return return_val;
}
