static int php_openssl_parse_config(struct php_x509_request * req, zval * optional_args) /* {{{ */
{
	char * str;
	zval * item;

	SET_OPTIONAL_STRING_ARG("config", req->config_filename, default_ssl_conf_filename);
	SET_OPTIONAL_STRING_ARG("config_section_name", req->section_name, "req");
	req->global_config = CONF_load(NULL, default_ssl_conf_filename, NULL);
	if (req->global_config == NULL) {
		php_openssl_store_errors();
	}
	req->req_config = CONF_load(NULL, req->config_filename, NULL);
	if (req->req_config == NULL) {
		php_openssl_store_errors();
		return FAILURE;
	}

	/* read in the oids */
	str = CONF_get_string(req->req_config, NULL, "oid_file");
	if (str == NULL) {
		php_openssl_store_errors();
	} else if (!php_openssl_open_base_dir_chk(str)) {
		BIO *oid_bio = BIO_new_file(str, "r");
		if (oid_bio) {
			OBJ_create_objects(oid_bio);
			BIO_free(oid_bio);
			php_openssl_store_errors();
		}
	}
	if (add_oid_section(req) == FAILURE) {
		return FAILURE;
	}
	SET_OPTIONAL_STRING_ARG("digest_alg", req->digest_name,
		CONF_get_string(req->req_config, req->section_name, "default_md"));
	SET_OPTIONAL_STRING_ARG("x509_extensions", req->extensions_section,
		CONF_get_string(req->req_config, req->section_name, "x509_extensions"));
	SET_OPTIONAL_STRING_ARG("req_extensions", req->request_extensions_section,
		CONF_get_string(req->req_config, req->section_name, "req_extensions"));
	SET_OPTIONAL_LONG_ARG("private_key_bits", req->priv_key_bits,
		CONF_get_number(req->req_config, req->section_name, "default_bits"));

	SET_OPTIONAL_LONG_ARG("private_key_type", req->priv_key_type, OPENSSL_KEYTYPE_DEFAULT);

	if (optional_args && (item = zend_hash_str_find(Z_ARRVAL_P(optional_args), "encrypt_key", sizeof("encrypt_key")-1)) != NULL) {
		req->priv_key_encrypt = Z_TYPE_P(item) == IS_TRUE ? 1 : 0;
	} else {
		str = CONF_get_string(req->req_config, req->section_name, "encrypt_rsa_key");
		if (str == NULL) {
			str = CONF_get_string(req->req_config, req->section_name, "encrypt_key");
			/* it is sure that there are some errrors as str was NULL for encrypt_rsa_key */
			php_openssl_store_errors();
		}
		if (str != NULL && strcmp(str, "no") == 0) {
			req->priv_key_encrypt = 0;
		} else {
			req->priv_key_encrypt = 1;
		}
	}

	if (req->priv_key_encrypt && optional_args && (item = zend_hash_str_find(Z_ARRVAL_P(optional_args), "encrypt_key_cipher", sizeof("encrypt_key_cipher")-1)) != NULL
		&& Z_TYPE_P(item) == IS_LONG) {
		zend_long cipher_algo = Z_LVAL_P(item);
		const EVP_CIPHER* cipher = php_openssl_get_evp_cipher_from_algo(cipher_algo);
		if (cipher == NULL) {
			php_error_docref(NULL, E_WARNING, "Unknown cipher algorithm for private key.");
			return FAILURE;
		} else {
			req->priv_key_encrypt_cipher = cipher;
		}
	} else {
		req->priv_key_encrypt_cipher = NULL;
	}



	/* digest alg */
	if (req->digest_name == NULL) {
		req->digest_name = CONF_get_string(req->req_config, req->section_name, "default_md");
	}
	if (req->digest_name != NULL) {
		req->digest = req->md_alg = EVP_get_digestbyname(req->digest_name);
	} else {
		php_openssl_store_errors();
	}
	if (req->md_alg == NULL) {
		req->md_alg = req->digest = EVP_sha1();
		php_openssl_store_errors();
	}

	PHP_SSL_CONFIG_SYNTAX_CHECK(extensions_section);
#ifdef HAVE_EVP_PKEY_EC
	/* set the ec group curve name */
	req->curve_name = NID_undef;
	if (optional_args && (item = zend_hash_str_find(Z_ARRVAL_P(optional_args), "curve_name", sizeof("curve_name")-1)) != NULL
		&& Z_TYPE_P(item) == IS_STRING) {
		req->curve_name = OBJ_sn2nid(Z_STRVAL_P(item));
		if (req->curve_name == NID_undef) {
			php_error_docref(NULL, E_WARNING, "Unknown elliptic curve (short) name %s", Z_STRVAL_P(item));
			return FAILURE;
		}
	}
#endif

	/* set the string mask */
	str = CONF_get_string(req->req_config, req->section_name, "string_mask");
	if (str == NULL) {
		php_openssl_store_errors();
	} else if (!ASN1_STRING_set_default_mask_asc(str)) {
		php_error_docref(NULL, E_WARNING, "Invalid global string mask setting %s", str);
		return FAILURE;
	}

	PHP_SSL_CONFIG_SYNTAX_CHECK(request_extensions_section);

	return SUCCESS;
}
/* }}} */
