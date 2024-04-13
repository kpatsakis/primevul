static void _php_curl_close(zend_resource *rsrc)
{
	php_curl *ch = (php_curl *) rsrc->ptr;
	_php_curl_close_ex(ch);
}
