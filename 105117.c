static void php_session_rfc1867_early_find_sid(php_session_rfc1867_progress *progress TSRMLS_DC) /* {{{ */
{

	if (PS(use_cookies)) {
		sapi_module.treat_data(PARSE_COOKIE, NULL, NULL TSRMLS_CC);
		if (early_find_sid_in(&progress->sid, TRACK_VARS_COOKIE, progress TSRMLS_CC)) {
			progress->apply_trans_sid = 0;
			return;
		}
	}
	if (PS(use_only_cookies)) {
		return;
	}
	sapi_module.treat_data(PARSE_GET, NULL, NULL TSRMLS_CC);
	early_find_sid_in(&progress->sid, TRACK_VARS_GET, progress TSRMLS_CC);
} /* }}} */
