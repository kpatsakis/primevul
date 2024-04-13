static int extension_name_cmp(const zend_llist_element **f, const zend_llist_element **s TSRMLS_DC)
{
	return strcmp(	((zend_extension *)(*f)->data)->name,
					((zend_extension *)(*s)->data)->name);
}
