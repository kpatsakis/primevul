PHP_FUNCTION(openssl_seal)
{
	zval *pubkeys, **pubkey, *sealdata, *ekeys;
	HashTable *pubkeysht;
	HashPosition pos;
	EVP_PKEY **pkeys;
	long * key_resources;	/* so we know what to cleanup */
	int i, len1, len2, *eksl, nkeys;
	unsigned char *buf = NULL, **eks;
	char * data; int data_len;
	char *method =NULL;
	int method_len = 0;
	const EVP_CIPHER *cipher;
	EVP_CIPHER_CTX ctx;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "szza/|s", &data, &data_len, &sealdata, &ekeys, &pubkeys, &method, &method_len) == FAILURE) {
		return;
	}
	
	pubkeysht = HASH_OF(pubkeys);
	nkeys = pubkeysht ? zend_hash_num_elements(pubkeysht) : 0;
	if (!nkeys) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Fourth argument to openssl_seal() must be a non-empty array");
		RETURN_FALSE;
	}

	if (method) {
		cipher = EVP_get_cipherbyname(method);
		if (!cipher) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Unknown signature algorithm.");
			RETURN_FALSE;
		}
	} else {
		cipher = EVP_rc4();
	}

	pkeys = safe_emalloc(nkeys, sizeof(*pkeys), 0);
	eksl = safe_emalloc(nkeys, sizeof(*eksl), 0);
	eks = safe_emalloc(nkeys, sizeof(*eks), 0);
	memset(eks, 0, sizeof(*eks) * nkeys);
	key_resources = safe_emalloc(nkeys, sizeof(long), 0);
	memset(key_resources, 0, sizeof(*key_resources) * nkeys);

	/* get the public keys we are using to seal this data */
	zend_hash_internal_pointer_reset_ex(pubkeysht, &pos);
	i = 0;
	while (zend_hash_get_current_data_ex(pubkeysht, (void **) &pubkey,
				&pos) == SUCCESS) {
		pkeys[i] = php_openssl_evp_from_zval(pubkey, 1, NULL, 0, &key_resources[i] TSRMLS_CC);
		if (pkeys[i] == NULL) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "not a public key (%dth member of pubkeys)", i+1);
			RETVAL_FALSE;
			goto clean_exit;
		}
		eks[i] = emalloc(EVP_PKEY_size(pkeys[i]) + 1);
		zend_hash_move_forward_ex(pubkeysht, &pos);
		i++;
	}

	if (!EVP_EncryptInit(&ctx,cipher,NULL,NULL)) {
		RETVAL_FALSE;
		goto clean_exit;
	}

#if 0
	/* Need this if allow ciphers that require initialization vector */
	ivlen = EVP_CIPHER_CTX_iv_length(&ctx);
	iv = ivlen ? emalloc(ivlen + 1) : NULL;
#endif
	/* allocate one byte extra to make room for \0 */
	buf = emalloc(data_len + EVP_CIPHER_CTX_block_size(&ctx));

	if (!EVP_SealInit(&ctx, cipher, eks, eksl, NULL, pkeys, nkeys) || !EVP_SealUpdate(&ctx, buf, &len1, (unsigned char *)data, data_len)) {
		RETVAL_FALSE;
		efree(buf);
		goto clean_exit;
	}

	EVP_SealFinal(&ctx, buf + len1, &len2);

	if (len1 + len2 > 0) {
		zval_dtor(sealdata);
		buf[len1 + len2] = '\0';
		buf = erealloc(buf, len1 + len2 + 1);
		ZVAL_STRINGL(sealdata, (char *)buf, len1 + len2, 0);

		zval_dtor(ekeys);
		array_init(ekeys);
		for (i=0; i<nkeys; i++) {
			eks[i][eksl[i]] = '\0';
			add_next_index_stringl(ekeys, erealloc(eks[i], eksl[i] + 1), eksl[i], 0);
			eks[i] = NULL;
		}
#if 0
		/* If allow ciphers that need IV, we need this */
		zval_dtor(*ivec);
		if (ivlen) {
			iv[ivlen] = '\0';
			ZVAL_STRINGL(*ivec, erealloc(iv, ivlen + 1), ivlen, 0);
		} else {
			ZVAL_EMPTY_STRING(*ivec);
		}
#endif
	} else {
		efree(buf);
	}
	RETVAL_LONG(len1 + len2);

clean_exit:
	for (i=0; i<nkeys; i++) {
		if (key_resources[i] == -1) {
			EVP_PKEY_free(pkeys[i]);
		}
		if (eks[i]) { 
			efree(eks[i]);
		}
	}
	efree(eks);
	efree(eksl);
	efree(pkeys);
	efree(key_resources);
}
