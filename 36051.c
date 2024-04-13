static void __exit xfrm6_tunnel_fini(void)
{
	xfrm6_tunnel_spi_fini();
	xfrm6_tunnel_deregister(&xfrm46_tunnel_handler, AF_INET);
	xfrm6_tunnel_deregister(&xfrm6_tunnel_handler, AF_INET6);
	xfrm_unregister_type(&xfrm6_tunnel_type, AF_INET6);
}
