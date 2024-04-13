static bool vmci_transport_proto_to_notify_struct(struct sock *sk,
						  u16 *proto,
						  bool old_pkt_proto)
{
	struct vsock_sock *vsk = vsock_sk(sk);

	if (old_pkt_proto) {
		if (*proto != VSOCK_PROTO_INVALID) {
			pr_err("Can't set both an old and new protocol\n");
			return false;
		}
		vmci_trans(vsk)->notify_ops = &vmci_transport_notify_pkt_ops;
		goto exit;
	}

	switch (*proto) {
	case VSOCK_PROTO_PKT_ON_NOTIFY:
		vmci_trans(vsk)->notify_ops =
			&vmci_transport_notify_pkt_q_state_ops;
		break;
	default:
		pr_err("Unknown notify protocol version\n");
		return false;
	}

exit:
	vmci_trans(vsk)->notify_ops->socket_init(sk);
	return true;
}
