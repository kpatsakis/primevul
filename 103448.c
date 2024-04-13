static void release_wddx_packet_rsrc(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
	smart_str *str = (smart_str *)rsrc->ptr;
	smart_str_free(str);
	efree(str);
}
