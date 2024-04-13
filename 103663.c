static inline char * _php_pgsql_trim_result(PGconn * pgsql, char **buf)
{
	return *buf = _php_pgsql_trim_message(PQerrorMessage(pgsql), NULL);
}
