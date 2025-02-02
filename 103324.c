PHP_MSHUTDOWN_FUNCTION(openssl)
{
	EVP_cleanup();

#if OPENSSL_VERSION_NUMBER >= 0x00090805f
	/* prevent accessing locking callback from unloaded extension */
	CRYPTO_set_locking_callback(NULL);
	/* free allocated error strings */
	ERR_free_strings();
#endif

	php_unregister_url_stream_wrapper("https");
	php_unregister_url_stream_wrapper("ftps");

	php_stream_xport_unregister("ssl");
#ifndef OPENSSL_NO_SSL2
	php_stream_xport_unregister("sslv2");
#endif
#ifndef OPENSSL_NO_SSL3
	php_stream_xport_unregister("sslv3");
#endif
	php_stream_xport_unregister("tls");
	php_stream_xport_unregister("tlsv1.0");
#if OPENSSL_VERSION_NUMBER >= 0x10001001L
	php_stream_xport_unregister("tlsv1.1");
	php_stream_xport_unregister("tlsv1.2");
#endif

	/* reinstate the default tcp handler */
	php_stream_xport_register("tcp", php_stream_generic_socket_factory);

	UNREGISTER_INI_ENTRIES();

	return SUCCESS;
}
