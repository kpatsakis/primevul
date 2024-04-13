static void ppid2sid(zval **ppid TSRMLS_DC) {
	if (Z_TYPE_PP(ppid) != IS_STRING) {
		PS(id) = NULL;
		PS(send_cookie) = 1;
	} else {
		convert_to_string((*ppid));
		PS(id) = estrndup(Z_STRVAL_PP(ppid), Z_STRLEN_PP(ppid));
		PS(send_cookie) = 0;
	}
}
