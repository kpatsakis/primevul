PHPAPI int _php_error_log_ex(int opt_err, char *message, int message_len, char *opt, char *headers TSRMLS_DC) /* {{{ */
{
	php_stream *stream = NULL;

	switch (opt_err)
	{
		case 1:		/*send an email */
			if (!php_mail(opt, "PHP error_log message", message, headers, NULL TSRMLS_CC)) {
				return FAILURE;
			}
			break;

		case 2:		/*send to an address */
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "TCP/IP option not available!");
			return FAILURE;
			break;

		case 3:		/*save to a file */
			stream = php_stream_open_wrapper(opt, "a", IGNORE_URL_WIN | REPORT_ERRORS, NULL);
			if (!stream) {
				return FAILURE;
			}
			php_stream_write(stream, message, message_len);
			php_stream_close(stream);
			break;

		case 4: /* send to SAPI */
			if (sapi_module.log_message) {
				sapi_module.log_message(message TSRMLS_CC);
			} else {
				return FAILURE;
			}
			break;

		default:
			php_log_err(message TSRMLS_CC);
			break;
	}
	return SUCCESS;
}
/* }}} */
