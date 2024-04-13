void get_srv_from_appsession(struct session *s, const char *begin, int len)
{
	char *end_params, *first_param, *cur_param, *next_param;
	char separator;
	int value_len;

	int mode = s->be->options2 & PR_O2_AS_M_ANY;

	if (s->be->appsession_name == NULL ||
	    (s->txn.meth != HTTP_METH_GET && s->txn.meth != HTTP_METH_POST && s->txn.meth != HTTP_METH_HEAD)) {
		return;
	}

	first_param = NULL;
	switch (mode) {
	case PR_O2_AS_M_PP:
		first_param = memchr(begin, ';', len);
		break;
	case PR_O2_AS_M_QS:
		first_param = memchr(begin, '?', len);
		break;
	}

	if (first_param == NULL) {
		return;
	}

	switch (mode) {
	case PR_O2_AS_M_PP:
		if ((end_params = memchr(first_param, '?', len - (begin - first_param))) == NULL) {
			end_params = (char *) begin + len;
		}
		separator = ';';
		break;
	case PR_O2_AS_M_QS:
		end_params = (char *) begin + len;
		separator = '&';
		break;
	default:
		/* unknown mode, shouldn't happen */
		return;
	}
	
	cur_param = next_param = end_params;
	while (cur_param > first_param) {
		cur_param--;
		if ((cur_param[0] == separator) || (cur_param == first_param)) {
			/* let's see if this is the appsession parameter */
			if ((cur_param + s->be->appsession_name_len + 1 < next_param) &&
				((s->be->options2 & PR_O2_AS_PFX) || cur_param[s->be->appsession_name_len + 1] == '=') &&
				(strncasecmp(cur_param + 1, s->be->appsession_name, s->be->appsession_name_len) == 0)) {
				/* Cool... it's the right one */
				cur_param += s->be->appsession_name_len + (s->be->options2 & PR_O2_AS_PFX ? 1 : 2);
				value_len = MIN(s->be->appsession_len, next_param - cur_param);
				if (value_len > 0) {
					manage_client_side_appsession(s, cur_param, value_len);
				}
				break;
			}
			next_param = cur_param;
		}
	}
#if defined(DEBUG_HASH)
	Alert("get_srv_from_appsession\n");
	appsession_hash_dump(&(s->be->htbl_proxy));
#endif
}
