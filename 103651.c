PHP_RINIT_FUNCTION(pgsql)
{
	PGG(default_link) = NULL;
	PGG(num_links) = PGG(num_persistent);
	return SUCCESS;
}
