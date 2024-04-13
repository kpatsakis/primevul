mysqlnd_old_escape_string(char * newstr, const char * escapestr, size_t escapestr_len TSRMLS_DC)
{
	DBG_ENTER("mysqlnd_old_escape_string");
	DBG_RETURN(mysqlnd_cset_escape_slashes(mysqlnd_find_charset_name("latin1"), newstr, escapestr, escapestr_len TSRMLS_CC));
}
