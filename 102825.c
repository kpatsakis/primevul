static void add_assoc_name_entry(zval * val, char * key, X509_NAME * name, int shortname TSRMLS_DC) /* {{{ */
{
	zval *subitem, *subentries;
	int i, j = -1, last = -1, obj_cnt = 0;
	char *sname;
	int nid;
	X509_NAME_ENTRY * ne;
	ASN1_STRING * str = NULL;
	ASN1_OBJECT * obj;

	if (key != NULL) {
		MAKE_STD_ZVAL(subitem);
		array_init(subitem);
	} else {
		subitem = val;
	}
	
	for (i = 0; i < X509_NAME_entry_count(name); i++) {
		unsigned char *to_add;
		int to_add_len;


		ne  = X509_NAME_get_entry(name, i);
		obj = X509_NAME_ENTRY_get_object(ne);
		nid = OBJ_obj2nid(obj);
		obj_cnt = 0;

		if (shortname) {
			sname = (char *) OBJ_nid2sn(nid);
		} else {
			sname = (char *) OBJ_nid2ln(nid);
		}

		MAKE_STD_ZVAL(subentries);
		array_init(subentries);

		last = -1;
		for (;;) {
			j = X509_NAME_get_index_by_OBJ(name, obj, last);
			if (j < 0) {
				if (last != -1) break;
			} else {
				obj_cnt++;
				ne  = X509_NAME_get_entry(name, j);
				str = X509_NAME_ENTRY_get_data(ne);
				if (ASN1_STRING_type(str) != V_ASN1_UTF8STRING) {
					to_add_len = ASN1_STRING_to_UTF8(&to_add, str);
					if (to_add_len != -1) {
						add_next_index_stringl(subentries, (char *)to_add, to_add_len, 1);
					}
				} else {
					to_add = ASN1_STRING_data(str);
					to_add_len = ASN1_STRING_length(str);
					add_next_index_stringl(subentries, (char *)to_add, to_add_len, 1);
				}
			}
			last = j;
		}
		i = last;
		
		if (obj_cnt > 1) {
			add_assoc_zval_ex(subitem, sname, strlen(sname) + 1, subentries);
		} else {
			zval_dtor(subentries);
			FREE_ZVAL(subentries);
			if (obj_cnt && str && to_add_len > -1) {
				add_assoc_stringl(subitem, sname, (char *)to_add, to_add_len, 1);
			}
		}
	}
	if (key != NULL) {
		zend_hash_update(HASH_OF(val), key, strlen(key) + 1, (void *)&subitem, sizeof(subitem), NULL);
	}
}
/* }}} */
