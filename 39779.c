static int vmci_transport_send_negotiate2(struct sock *sk, size_t size,
					  u16 version)
{
	return vmci_transport_send_control_pkt(
					sk,
					VMCI_TRANSPORT_PACKET_TYPE_NEGOTIATE2,
					size, 0, NULL, version,
					VMCI_INVALID_HANDLE);
}
