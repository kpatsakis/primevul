static void __http_protocol_init(void)
{
	acl_register_keywords(&acl_kws);
	sample_register_fetches(&sample_fetch_keywords);
	sample_register_convs(&sample_conv_kws);
	http_req_keywords_register(&http_req_actions);
	http_res_keywords_register(&http_res_actions);
	cli_register_kw(&cli_kws);
}
