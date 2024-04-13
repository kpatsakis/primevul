void suhosin_unhook_header_handler()
{
	sapi_module.header_handler = orig_header_handler;
	orig_header_handler = NULL;
}
