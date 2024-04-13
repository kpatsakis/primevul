static void php_pgsql_set_default_link(zend_resource *res)
{
	GC_REFCOUNT(res)++;

	if (PGG(default_link) != NULL) {
		zend_list_delete(PGG(default_link));
	}

	PGG(default_link) = res;
}
