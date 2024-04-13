int vmci_transport_send_waiting_write(struct sock *sk,
				      struct vmci_transport_waiting_info *wait)
{
	return vmci_transport_send_control_pkt(
				sk, VMCI_TRANSPORT_PACKET_TYPE_WAITING_WRITE,
				0, 0, wait, VSOCK_PROTO_INVALID,
				VMCI_INVALID_HANDLE);
}
