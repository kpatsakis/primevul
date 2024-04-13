PHP_FUNCTION(openssl_encrypt)
{
	zend_long options = 0;
	char *data, *method, *password, *iv = "";
	size_t data_len, method_len, password_len, iv_len = 0, max_iv_len;
	const EVP_CIPHER *cipher_type;
	EVP_CIPHER_CTX *cipher_ctx;
	int i=0, keylen;
	size_t outlen;
	zend_string *outbuf;
	unsigned char *key;
	zend_bool free_iv;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sss|ls", &data, &data_len, &method, &method_len, &password, &password_len, &options, &iv, &iv_len) == FAILURE) {
		return;
	}
	cipher_type = EVP_get_cipherbyname(method);
	if (!cipher_type) {
		php_error_docref(NULL, E_WARNING, "Unknown cipher algorithm");
		RETURN_FALSE;
	}

	PHP_OPENSSL_CHECK_SIZE_T_TO_INT(data_len, data);
	PHP_OPENSSL_CHECK_SIZE_T_TO_INT(password_len, password);

	cipher_ctx = EVP_CIPHER_CTX_new();
	if (!cipher_ctx) {
		php_error_docref(NULL, E_WARNING, "Failed to create cipher context");
		RETURN_FALSE;
	}

	keylen = EVP_CIPHER_key_length(cipher_type);
	if (keylen > password_len) {
		key = emalloc(keylen);
		memset(key, 0, keylen);
		memcpy(key, password, password_len);
	} else {
		key = (unsigned char*)password;
	}

	max_iv_len = EVP_CIPHER_iv_length(cipher_type);
	if (iv_len == 0 && max_iv_len > 0) {
		php_error_docref(NULL, E_WARNING, "Using an empty Initialization Vector (iv) is potentially insecure and not recommended");
	}
	free_iv = php_openssl_validate_iv(&iv, &iv_len, max_iv_len);

	outlen = data_len + EVP_CIPHER_block_size(cipher_type);
	outbuf = zend_string_alloc(outlen, 0);

	EVP_EncryptInit(cipher_ctx, cipher_type, NULL, NULL);
	if (password_len > keylen) {
		EVP_CIPHER_CTX_set_key_length(cipher_ctx, (int)password_len);
	}
	EVP_EncryptInit_ex(cipher_ctx, NULL, NULL, key, (unsigned char *)iv);
	if (options & OPENSSL_ZERO_PADDING) {
		EVP_CIPHER_CTX_set_padding(cipher_ctx, 0);
	}
	if (data_len > 0) {
		EVP_EncryptUpdate(cipher_ctx, (unsigned char*)ZSTR_VAL(outbuf), &i, (unsigned char *)data, (int)data_len);
	}
	outlen = i;
	if (EVP_EncryptFinal(cipher_ctx, (unsigned char *)ZSTR_VAL(outbuf) + i, &i)) {
		outlen += i;
		if (options & OPENSSL_RAW_DATA) {
			ZSTR_VAL(outbuf)[outlen] = '\0';
			ZSTR_LEN(outbuf) = outlen;
			RETVAL_STR(outbuf);
		} else {
			zend_string *base64_str;

			base64_str = php_base64_encode((unsigned char*)ZSTR_VAL(outbuf), outlen);
			zend_string_release(outbuf);
			RETVAL_STR(base64_str);
		}
	} else {
		zend_string_release(outbuf);
		RETVAL_FALSE;
	}
	if (key != (unsigned char*)password) {
		efree(key);
	}
	if (free_iv) {
		efree(iv);
	}
	EVP_CIPHER_CTX_free(cipher_ctx);
}
