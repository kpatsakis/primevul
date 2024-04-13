static bool vmci_transport_old_proto_override(bool *old_pkt_proto)
{
	if (PROTOCOL_OVERRIDE != -1) {
		if (PROTOCOL_OVERRIDE == 0)
			*old_pkt_proto = true;
		else
			*old_pkt_proto = false;

		pr_info("Proto override in use\n");
		return true;
	}

	return false;
}
