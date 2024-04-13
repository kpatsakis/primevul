http_get_path_from_string(char *str)
{
	char *ptr = str;

	/* RFC2616, par. 5.1.2 :
	 * Request-URI = "*" | absuri | abspath | authority
	 */

	if (*ptr == '*')
		return NULL;

	if (isalpha((unsigned char)*ptr)) {
		/* this is a scheme as described by RFC3986, par. 3.1 */
		ptr++;
		while (isalnum((unsigned char)*ptr) || *ptr == '+' || *ptr == '-' || *ptr == '.')
			ptr++;
		/* skip '://' */
		if (*ptr == '\0' || *ptr++ != ':')
			return NULL;
		if (*ptr == '\0' || *ptr++ != '/')
			return NULL;
		if (*ptr == '\0' || *ptr++ != '/')
			return NULL;
	}
	/* skip [user[:passwd]@]host[:[port]] */

	while (*ptr != '\0' && *ptr != ' ' && *ptr != '/')
		ptr++;

	if (*ptr == '\0' || *ptr == ' ')
		return NULL;

	/* OK, we got the '/' ! */
	return ptr;
}
