static void _php_pgsql_notice_ptr_dtor(void **ptr) 
{
	php_pgsql_notice *notice = (php_pgsql_notice *)*ptr;
	if (notice) {
		efree(notice->message);
		efree(notice);
		notice = NULL;
	}
}
