vmci_transport_recv_connecting_client_invalid(struct sock *sk,
					      struct vmci_transport_packet *pkt)
{
	int err = 0;
	struct vsock_sock *vsk = vsock_sk(sk);

	if (vsk->sent_request) {
		vsk->sent_request = false;
		vsk->ignore_connecting_rst = true;

		err = vmci_transport_send_conn_request(
			sk, vmci_trans(vsk)->queue_pair_size);
		if (err < 0)
			err = vmci_transport_error_to_vsock_error(err);
		else
			err = 0;

	}

	return err;
}
