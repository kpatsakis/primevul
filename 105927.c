static int _php_pgsql_detect_identifier_escape(const char *identifier, size_t len)
{
	size_t i;

	/* Handle edge case. Cannot be a escaped string */
	if (len <= 2) {
		return FAILURE;
	}
	/* Detect double qoutes */
	if (identifier[0] == '"' && identifier[len-1] == '"') {
		/* Detect wrong format of " inside of escaped string */
		for (i = 1; i < len-1; i++) {
			if (identifier[i] == '"' && (identifier[++i] != '"' || i == len-1)) {
				return FAILURE;
			}
		}
	} else {
		return FAILURE;
	}
	/* Escaped properly */
	return SUCCESS;
}
