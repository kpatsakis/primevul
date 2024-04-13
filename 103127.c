PHP_FUNCTION(connection_status)
{
	RETURN_LONG(PG(connection_status));
}
