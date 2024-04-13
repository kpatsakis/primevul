void udp_encap_enable(void)
{
	if (!static_key_enabled(&udp_encap_needed))
		static_key_slow_inc(&udp_encap_needed);
}
