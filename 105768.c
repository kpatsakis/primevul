MYSQLND_METHOD(mysqlnd_conn_data, list_fields)(MYSQLND_CONN_DATA * conn, const char *table, const char *achtung_wild TSRMLS_DC)
{
	size_t this_func = STRUCT_OFFSET(struct st_mysqlnd_conn_data_methods, list_fields);
	/* db + \0 + wild + \0 (for wild) */
	zend_uchar buff[MYSQLND_MAX_ALLOWED_DB_LEN * 2 + 1 + 1], *p;
	size_t table_len, wild_len;
	MYSQLND_RES * result = NULL;
	DBG_ENTER("mysqlnd_conn_data::list_fields");
	DBG_INF_FMT("conn=%llu table=%s wild=%s", conn->thread_id, table? table:"",achtung_wild? achtung_wild:"");

	if (PASS == conn->m->local_tx_start(conn, this_func TSRMLS_CC)) {
		do {
			p = buff;
			if (table && (table_len = strlen(table))) {
				size_t to_copy = MIN(table_len, MYSQLND_MAX_ALLOWED_DB_LEN);
				memcpy(p, table, to_copy);
				p += to_copy;
				*p++ = '\0';
			}

			if (achtung_wild && (wild_len = strlen(achtung_wild))) {
				size_t to_copy = MIN(wild_len, MYSQLND_MAX_ALLOWED_DB_LEN);
				memcpy(p, achtung_wild, to_copy);
				p += to_copy;
				*p++ = '\0';
			}

			if (PASS != conn->m->simple_command(conn, COM_FIELD_LIST, buff, p - buff,
											   PROT_LAST /* we will handle the OK packet*/,
											   FALSE, TRUE TSRMLS_CC)) {
				conn->m->local_tx_end(conn, 0, FAIL TSRMLS_CC);
				break;
			}

			/*
			   Prepare for the worst case.
			   MyISAM goes to 2500 BIT columns, double it for safety.
			*/
			result = conn->m->result_init(5000, conn->persistent TSRMLS_CC);
			if (!result) {
				break;
			}

			if (FAIL == result->m.read_result_metadata(result, conn TSRMLS_CC)) {
				DBG_ERR("Error occurred while reading metadata");
				result->m.free_result(result, TRUE TSRMLS_CC);
				result = NULL;
				break;
			}

			result->type = MYSQLND_RES_NORMAL;
			result->m.fetch_row = result->m.fetch_row_normal_unbuffered;
			result->unbuf = mnd_ecalloc(1, sizeof(MYSQLND_RES_UNBUFFERED));
			if (!result->unbuf) {
				/* OOM */
				SET_OOM_ERROR(*conn->error_info);
				result->m.free_result(result, TRUE TSRMLS_CC);
				result = NULL;
				break;
			}
			result->unbuf->eof_reached = TRUE;
		} while (0);
		conn->m->local_tx_end(conn, this_func, result == NULL? FAIL:PASS TSRMLS_CC);
	}

	DBG_RETURN(result);
}
