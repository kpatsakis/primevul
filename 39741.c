static void vmci_transport_handle_detach(struct sock *sk)
{
	struct vsock_sock *vsk;

	vsk = vsock_sk(sk);
	if (!vmci_handle_is_invalid(vmci_trans(vsk)->qp_handle)) {
		sock_set_flag(sk, SOCK_DONE);

		/* On a detach the peer will not be sending or receiving
		 * anymore.
		 */
		vsk->peer_shutdown = SHUTDOWN_MASK;

		/* We should not be sending anymore since the peer won't be
		 * there to receive, but we can still receive if there is data
		 * left in our consume queue.
		 */
		if (vsock_stream_has_data(vsk) <= 0) {
			if (sk->sk_state == SS_CONNECTING) {
				/* The peer may detach from a queue pair while
				 * we are still in the connecting state, i.e.,
				 * if the peer VM is killed after attaching to
				 * a queue pair, but before we complete the
				 * handshake. In that case, we treat the detach
				 * event like a reset.
				 */

				sk->sk_state = SS_UNCONNECTED;
				sk->sk_err = ECONNRESET;
				sk->sk_error_report(sk);
				return;
			}
			sk->sk_state = SS_UNCONNECTED;
		}
		sk->sk_state_change(sk);
	}
}
