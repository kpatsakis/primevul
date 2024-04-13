static void _php_pgsql_notice_ptr_dtor(zval *el) 
{
	php_pgsql_notice *notice = (php_pgsql_notice *)Z_PTR_P(el);
	if (notice) {
		efree(notice->message);
		efree(notice);
	}
}
