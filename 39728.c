static int vmci_transport_connect(struct vsock_sock *vsk)
{
	int err;
	bool old_pkt_proto = false;
	struct sock *sk = &vsk->sk;

	if (vmci_transport_old_proto_override(&old_pkt_proto) &&
		old_pkt_proto) {
		err = vmci_transport_send_conn_request(
			sk, vmci_trans(vsk)->queue_pair_size);
		if (err < 0) {
			sk->sk_state = SS_UNCONNECTED;
			return err;
		}
	} else {
		int supported_proto_versions =
			vmci_transport_new_proto_supported_versions();
		err = vmci_transport_send_conn_request2(
				sk, vmci_trans(vsk)->queue_pair_size,
				supported_proto_versions);
		if (err < 0) {
			sk->sk_state = SS_UNCONNECTED;
			return err;
		}

		vsk->sent_request = true;
	}

	return err;
}
