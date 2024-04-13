alert_match_text (char *text, char *masks)
{
	unsigned char *p = text;
	unsigned char endchar;
	int res;

	if (masks[0] == 0)
		return FALSE;

	while (1)
	{
		if (*p >= '0' && *p <= '9')
		{
			p++;
			continue;
		}

		/* if it's RFC1459 <special>, it can be inside a word */
		switch (*p)
		{
		case '-': case '[': case ']': case '\\':
		case '`': case '^': case '{': case '}':
		case '_': case '|':
			p++;
			continue;
		}

		/* if it's a 0, space or comma, the word has ended. */
		if (*p == 0 || *p == ' ' || *p == ',' ||
			/* if it's anything BUT a letter, the word has ended. */
			 (!g_unichar_isalpha (g_utf8_get_char (p))))
		{
			endchar = *p;
			*p = 0;
			res = alert_match_word (text, masks);
			*p = endchar;

			if (res)
				return TRUE;	/* yes, matched! */

			text = p + g_utf8_skip [p[0]];
			if (*p == 0)
				return FALSE;
		}

		p += g_utf8_skip [p[0]];
	}
}