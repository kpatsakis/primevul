PHP_FUNCTION(pg_field_name)
{
	php_pgsql_get_field_info(INTERNAL_FUNCTION_PARAM_PASSTHRU,PHP_PG_FIELD_NAME);
}
