static void create_certinfo(struct curl_certinfo *ci, zval *listcode)
{
	int i;

	if (ci) {
		zval certhash;

		for (i=0; i<ci->num_of_certs; i++) {
			struct curl_slist *slist;

			array_init(&certhash);
			for (slist = ci->certinfo[i]; slist; slist = slist->next) {
				int len;
				char s[64];
				char *tmp;
				strncpy(s, slist->data, 64);
				tmp = memchr(s, ':', 64);
				if(tmp) {
					*tmp = '\0';
					len = strlen(s);
					if (!strcmp(s, "Subject") || !strcmp(s, "Issuer")) {
						zval hash;

						array_init(&hash);

						split_certinfo(&slist->data[len+1], &hash);
						add_assoc_zval(&certhash, s, &hash);
					} else {
						add_assoc_string(&certhash, s, &slist->data[len+1]);
					}
				} else {
					php_error_docref(NULL, E_WARNING, "Could not extract hash key from certificate info");
				}
			}
			add_next_index_zval(listcode, &certhash);
		}
	}
}
