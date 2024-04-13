static int sample_conv_q_prefered(const struct arg *args, struct sample *smp, void *private)
{
	const char *al = smp->data.u.str.str;
	const char *end = al + smp->data.u.str.len;
	const char *token;
	int toklen;
	int qvalue;
	const char *str;
	const char *w;
	int best_q = 0;

	/* Set the constant to the sample, because the output of the
	 * function will be peek in the constant configuration string.
	 */
	smp->flags |= SMP_F_CONST;
	smp->data.u.str.size = 0;
	smp->data.u.str.str = "";
	smp->data.u.str.len = 0;

	/* Parse the accept language */
	while (1) {

		/* Jump spaces, quit if the end is detected. */
		while (al < end && isspace((unsigned char)*al))
			al++;
		if (al >= end)
			break;

		/* Start of the fisrt word. */
		token = al;

		/* Look for separator: isspace(), ',' or ';'. Next value if 0 length word. */
		while (al < end && *al != ';' && *al != ',' && !isspace((unsigned char)*al))
			al++;
		if (al == token)
			goto expect_comma;

		/* Length of the token. */
		toklen = al - token;
		qvalue = 1000;

		/* Check if the token exists in the list. If the token not exists,
		 * jump to the next token.
		 */
		str = args[0].data.str.str;
		w = str;
		while (1) {
			if (*str == ';' || *str == '\0') {
				if (language_range_match(token, toklen, w, str-w))
					goto look_for_q;
				if (*str == '\0')
					goto expect_comma;
				w = str + 1;
			}
			str++;
		}
		goto expect_comma;

look_for_q:

		/* Jump spaces, quit if the end is detected. */
		while (al < end && isspace((unsigned char)*al))
			al++;
		if (al >= end)
			goto process_value;

		/* If ',' is found, process the result */
		if (*al == ',')
			goto process_value;

		/* If the character is different from ';', look
		 * for the end of the header part in best effort.
		 */
		if (*al != ';')
			goto expect_comma;

		/* Assumes that the char is ';', now expect "q=". */
		al++;

		/* Jump spaces, process value if the end is detected. */
		while (al < end && isspace((unsigned char)*al))
			al++;
		if (al >= end)
			goto process_value;

		/* Expect 'q'. If no 'q', continue in best effort */
		if (*al != 'q')
			goto process_value;
		al++;

		/* Jump spaces, process value if the end is detected. */
		while (al < end && isspace((unsigned char)*al))
			al++;
		if (al >= end)
			goto process_value;

		/* Expect '='. If no '=', continue in best effort */
		if (*al != '=')
			goto process_value;
		al++;

		/* Jump spaces, process value if the end is detected. */
		while (al < end && isspace((unsigned char)*al))
			al++;
		if (al >= end)
			goto process_value;

		/* Parse the q value. */
		qvalue = parse_qvalue(al, &al);

process_value:

		/* If the new q value is the best q value, then store the associated
		 * language in the response. If qvalue is the biggest value (1000),
		 * break the process.
		 */
		if (qvalue > best_q) {
			smp->data.u.str.str = (char *)w;
			smp->data.u.str.len = str - w;
			if (qvalue >= 1000)
				break;
			best_q = qvalue;
		}

expect_comma:

		/* Expect comma or end. If the end is detected, quit the loop. */
		while (al < end && *al != ',')
			al++;
		if (al >= end)
			break;

		/* Comma is found, jump it and restart the analyzer. */
		al++;
	}

	/* Set default value if required. */
	if (smp->data.u.str.len == 0 && args[1].type == ARGT_STR) {
		smp->data.u.str.str = args[1].data.str.str;
		smp->data.u.str.len = args[1].data.str.len;
	}

	/* Return true only if a matching language was found. */
	return smp->data.u.str.len != 0;
}
