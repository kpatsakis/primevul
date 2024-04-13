void suhosin_hook_header_handler()
{
	if (orig_header_handler == NULL) {
		orig_header_handler = sapi_module.header_handler;
		sapi_module.header_handler = suhosin_header_handler;
	}
}
