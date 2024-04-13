static void _free_ptr(zend_resource *rsrc)
{
	pgLofp *lofp = (pgLofp *)rsrc->ptr;
	efree(lofp);
}
