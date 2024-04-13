static char * _php_pgsql_trim_message(const char *message, size_t *len)
{
	register size_t i = strlen(message)-1;

	if (i>1 && (message[i-1] == '\r' || message[i-1] == '\n') && message[i] == '.') {
		--i;
	}
	while (i>0 && (message[i] == '\r' || message[i] == '\n')) {
		--i;
	}
	++i;
	if (len) {
		*len = i;
	}
	return estrndup(message, i);
}
