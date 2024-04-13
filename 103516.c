PHP_FUNCTION(curl_copy_handle)
{
	CURL		*cp;
	zval		*zid;
	php_curl	*ch, *dupch;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "r", &zid) == FAILURE) {
		return;
	}

	ZEND_FETCH_RESOURCE(ch, php_curl *, zid, -1, le_curl_name, le_curl);

	cp = curl_easy_duphandle(ch->cp);
	if (!cp) {
		php_error_docref(NULL, E_WARNING, "Cannot duplicate cURL handle");
		RETURN_FALSE;
	}

	dupch = alloc_curl_handle();

	dupch->cp = cp;
	Z_ADDREF_P(zid);
	if (!Z_ISUNDEF(ch->handlers->write->stream)) {
		Z_ADDREF(ch->handlers->write->stream);
	}
	dupch->handlers->write->stream = ch->handlers->write->stream;
	dupch->handlers->write->method = ch->handlers->write->method;
	if (!Z_ISUNDEF(ch->handlers->read->stream)) {
		Z_ADDREF(ch->handlers->read->stream);
	}
	dupch->handlers->read->stream  = ch->handlers->read->stream;
	dupch->handlers->read->method  = ch->handlers->read->method;
	dupch->handlers->write_header->method = ch->handlers->write_header->method;
	if (!Z_ISUNDEF(ch->handlers->write_header->stream)) {
		Z_ADDREF(ch->handlers->write_header->stream);
	}
	dupch->handlers->write_header->stream = ch->handlers->write_header->stream;

	dupch->handlers->write->fp = ch->handlers->write->fp;
	dupch->handlers->write_header->fp = ch->handlers->write_header->fp;
	dupch->handlers->read->fp = ch->handlers->read->fp;
	dupch->handlers->read->res = ch->handlers->read->res;
#if CURLOPT_PASSWDDATA != 0
	if (!Z_ISUNDEF(ch->handlers->passwd)) {
		ZVAL_COPY(&dupch->handlers->passwd, &ch->handlers->passwd);
		curl_easy_setopt(ch->cp, CURLOPT_PASSWDDATA, (void *) dupch);
	}
#endif
	if (!Z_ISUNDEF(ch->handlers->write->func_name)) {
		ZVAL_COPY(&dupch->handlers->write->func_name, &ch->handlers->write->func_name);
	}
	if (!Z_ISUNDEF(ch->handlers->read->func_name)) {
		ZVAL_COPY(&dupch->handlers->read->func_name, &ch->handlers->read->func_name);
	}
	if (!Z_ISUNDEF(ch->handlers->write_header->func_name)) {
		ZVAL_COPY(&dupch->handlers->write_header->func_name, &ch->handlers->write_header->func_name);
	}

	curl_easy_setopt(dupch->cp, CURLOPT_ERRORBUFFER,       dupch->err.str);
	curl_easy_setopt(dupch->cp, CURLOPT_FILE,              (void *) dupch);
	curl_easy_setopt(dupch->cp, CURLOPT_INFILE,            (void *) dupch);
	curl_easy_setopt(dupch->cp, CURLOPT_WRITEHEADER,       (void *) dupch);

	if (ch->handlers->progress) {
		dupch->handlers->progress = ecalloc(1, sizeof(php_curl_progress));
		if (!Z_ISUNDEF(ch->handlers->progress->func_name)) {
			ZVAL_COPY(&dupch->handlers->progress->func_name, &ch->handlers->progress->func_name);
		}
		dupch->handlers->progress->method = ch->handlers->progress->method;
		curl_easy_setopt(dupch->cp, CURLOPT_PROGRESSDATA, (void *) dupch);
	}

/* Available since 7.21.0 */
#if LIBCURL_VERSION_NUM >= 0x071500
	if (ch->handlers->fnmatch) {
		dupch->handlers->fnmatch = ecalloc(1, sizeof(php_curl_fnmatch));
		if (!Z_ISUNDEF(ch->handlers->fnmatch->func_name)) {
			ZVAL_COPY(&dupch->handlers->fnmatch->func_name, &ch->handlers->fnmatch->func_name);
		}
		dupch->handlers->fnmatch->method = ch->handlers->fnmatch->method;
		curl_easy_setopt(dupch->cp, CURLOPT_FNMATCH_DATA, (void *) dupch);
	}
#endif

	efree(dupch->to_free->slist);
	efree(dupch->to_free);
	dupch->to_free = ch->to_free;

	/* Keep track of cloned copies to avoid invoking curl destructors for every clone */
	ch->clone++;

	ZEND_REGISTER_RESOURCE(return_value, dupch, le_curl);
	dupch->res = Z_RES_P(return_value);
}
