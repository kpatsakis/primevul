static u16 vmci_transport_new_proto_supported_versions(void)
{
	if (PROTOCOL_OVERRIDE != -1)
		return PROTOCOL_OVERRIDE;

	return VSOCK_PROTO_ALL_SUPPORTED;
}
