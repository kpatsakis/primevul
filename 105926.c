static void _free_result(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	pgsql_result_handle *pg_result = (pgsql_result_handle *)rsrc->ptr;

	PQclear(pg_result->result);
	efree(pg_result);
}
