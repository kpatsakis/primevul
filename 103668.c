static char *php_pgsql_PQescapeInternal(PGconn *conn, const char *str, size_t len, int escape_literal, int safe) /* {{{ */
{
	char *result, *rp, *s;
	size_t tmp_len;

	if (!conn) {
		return NULL;
	}

	/* allocate enough memory */
	rp = result = (char *)safe_emalloc(len, 2, 5); /* leading " E" needs extra 2 bytes + quote_chars on both end for 2 bytes + NULL */

	if (escape_literal) {
		size_t new_len;

		if (safe) {
			char *tmp = (char *)safe_emalloc(len, 2, 1);
			*rp++ = '\'';
			/* PQescapeString does not escape \, but it handles multibyte chars safely.
			   This escape is incompatible with PQescapeLiteral. */
			new_len = PQescapeStringConn(conn, tmp, str, len, NULL);
			strncpy(rp, tmp, new_len);
			efree(tmp);
			rp += new_len;
		} else {
			char *encoding;
			/* This is compatible with PQescapeLiteral, but it cannot handle multbyte chars
			   such as SJIS, BIG5. Raise warning and return NULL by checking
			   client_encoding. */
			encoding = (char *) pg_encoding_to_char(PQclientEncoding(conn));
			if (!strncmp(encoding, "SJIS", sizeof("SJIS")-1) ||
				!strncmp(encoding, "SHIFT_JIS_2004", sizeof("SHIFT_JIS_2004")-1) ||
				!strncmp(encoding, "BIG5", sizeof("BIG5")-1) ||
				!strncmp(encoding, "GB18030", sizeof("GB18030")-1) ||
				!strncmp(encoding, "GBK", sizeof("GBK")-1) ||
				!strncmp(encoding, "JOHAB", sizeof("JOHAB")-1) ||
				!strncmp(encoding, "UHC", sizeof("UHC")-1) ) {
			
				php_error_docref(NULL, E_WARNING, "Unsafe encoding is used. Do not use '%s' encoding or use PostgreSQL 9.0 or later libpq.", encoding);
			}
			/* check backslashes */
			tmp_len = strspn(str, "\\");
			if (tmp_len != len) {
				/* add " E" for escaping slashes */
				*rp++ = ' ';
				*rp++ = 'E';
			}
			*rp++ = '\'';
			for (s = (char *)str; s - str < len; ++s) {
				if (*s == '\'' || *s == '\\') {
					*rp++ = *s;
					*rp++ = *s;
				} else {
					*rp++ = *s;
				}
			}
		}
		*rp++ = '\'';
	} else {
		/* Identifier escape. */
		*rp++ = '"';
		for (s = (char *)str; s - str < len; ++s) {
			if (*s == '"') {
				*rp++ = '"';
				*rp++ = '"';
			} else {
				*rp++ = *s;
			}
		}
		*rp++ = '"';
	}
	*rp = '\0';

	return result;
}
/* }}} */
