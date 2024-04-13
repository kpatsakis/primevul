PHPAPI char *php_escape_shell_arg(char *str)
{
	int x, y = 0, l = strlen(str);
	char *cmd;
	size_t estimate = (4 * l) + 3;

	TSRMLS_FETCH();

	cmd = safe_emalloc(4, l, 3); /* worst case */

#ifdef PHP_WIN32
	cmd[y++] = '"';
#else
	cmd[y++] = '\'';
#endif

	for (x = 0; x < l; x++) {
		int mb_len = php_mblen(str + x, (l - x));

		/* skip non-valid multibyte characters */
		if (mb_len < 0) {
			continue;
		} else if (mb_len > 1) {
			memcpy(cmd + y, str + x, mb_len);
			y += mb_len;
			x += mb_len - 1;
			continue;
		}

		switch (str[x]) {
#ifdef PHP_WIN32
		case '"':
		case '%':
			cmd[y++] = ' ';
			break;
#else
		case '\'':
			cmd[y++] = '\'';
			cmd[y++] = '\\';
			cmd[y++] = '\'';
#endif
			/* fall-through */
		default:
			cmd[y++] = str[x];
		}
	}
#ifdef PHP_WIN32
	cmd[y++] = '"';
#else
	cmd[y++] = '\'';
#endif
	cmd[y] = '\0';

	if ((estimate - y) > 4096) {
		/* realloc if the estimate was way overill
		 * Arbitrary cutoff point of 4096 */
		cmd = erealloc(cmd, y + 1);
	}
	return cmd;
}