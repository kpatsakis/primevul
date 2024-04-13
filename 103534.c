static void _php_curl_close_ex(php_curl *ch)
{
#if PHP_CURL_DEBUG
	fprintf(stderr, "DTOR CALLED, ch = %x\n", ch);
#endif

	_php_curl_verify_handlers(ch, 0);

	/*
	 * Libcurl is doing connection caching. When easy handle is cleaned up,
	 * if the handle was previously used by the curl_multi_api, the connection
	 * remains open un the curl multi handle is cleaned up. Some protocols are
	 * sending content like the FTP one, and libcurl try to use the
	 * WRITEFUNCTION or the HEADERFUNCTION. Since structures used in those
	 * callback are freed, we need to use an other callback to which avoid
	 * segfaults.
	 *
	 * Libcurl commit d021f2e8a00 fix this issue and should be part of 7.28.2
	 */
	curl_easy_setopt(ch->cp, CURLOPT_HEADERFUNCTION, curl_write_nothing);
	curl_easy_setopt(ch->cp, CURLOPT_WRITEFUNCTION, curl_write_nothing);

	curl_easy_cleanup(ch->cp);

	/* cURL destructors should be invoked only by last curl handle */
	if (--ch->clone == 0) {
		zend_llist_clean(&ch->to_free->str);
		zend_llist_clean(&ch->to_free->post);
		zend_hash_destroy(ch->to_free->slist);
		efree(ch->to_free->slist);
		efree(ch->to_free);
	}

	smart_str_free(&ch->handlers->write->buf);
	zval_ptr_dtor(&ch->handlers->write->func_name);
	zval_ptr_dtor(&ch->handlers->read->func_name);
	zval_ptr_dtor(&ch->handlers->write_header->func_name);
#if CURLOPT_PASSWDFUNCTION != 0
	zval_ptr_dtor(&ch->handlers->passwd);
#endif
	zval_ptr_dtor(&ch->handlers->std_err);
	if (ch->header.str) {
		zend_string_release(ch->header.str);
	}

	zval_ptr_dtor(&ch->handlers->write_header->stream);
	zval_ptr_dtor(&ch->handlers->write->stream);
	zval_ptr_dtor(&ch->handlers->read->stream);

	efree(ch->handlers->write);
	efree(ch->handlers->write_header);
	efree(ch->handlers->read);

	if (ch->handlers->progress) {
		zval_ptr_dtor(&ch->handlers->progress->func_name);
		efree(ch->handlers->progress);
	}

#if LIBCURL_VERSION_NUM >= 0x071500 /* Available since 7.21.0 */
	if (ch->handlers->fnmatch) {
		zval_ptr_dtor(&ch->handlers->fnmatch->func_name);
		efree(ch->handlers->fnmatch);
	}
#endif

	efree(ch->handlers);
	efree(ch);
}
