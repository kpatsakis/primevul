static void curl_free_slist(zval *el)
{
	curl_slist_free_all(((struct curl_slist *)Z_PTR_P(el)));
}
