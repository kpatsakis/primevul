static void _free_ptr(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	pgLofp *lofp = (pgLofp *)rsrc->ptr;
	efree(lofp);
}
