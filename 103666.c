static int do_exec(smart_str *querystr, int expect, PGconn *pg_link, zend_ulong opt) /* {{{ */
{
	if (opt & PGSQL_DML_ASYNC) {
		if (PQsendQuery(pg_link, querystr->s->val)) {
			return 0;
		}
	}
	else {
		PGresult *pg_result;

		pg_result = PQexec(pg_link, querystr->s->val);
		if (PQresultStatus(pg_result) == expect) {
			PQclear(pg_result);
			return 0;
		} else {
			php_error_docref(NULL, E_WARNING, "%s", PQresultErrorMessage(pg_result));
			PQclear(pg_result);
		}
	}

	return -1;
}
/* }}} */
