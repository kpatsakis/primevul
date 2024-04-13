static int vsock_shutdown(struct socket *sock, int mode)
{
	int err;
	struct sock *sk;

	/* User level uses SHUT_RD (0) and SHUT_WR (1), but the kernel uses
	 * RCV_SHUTDOWN (1) and SEND_SHUTDOWN (2), so we must increment mode
	 * here like the other address families do.  Note also that the
	 * increment makes SHUT_RDWR (2) into RCV_SHUTDOWN | SEND_SHUTDOWN (3),
	 * which is what we want.
	 */
	mode++;

	if ((mode & ~SHUTDOWN_MASK) || !mode)
		return -EINVAL;

	/* If this is a STREAM socket and it is not connected then bail out
	 * immediately.  If it is a DGRAM socket then we must first kick the
	 * socket so that it wakes up from any sleeping calls, for example
	 * recv(), and then afterwards return the error.
	 */

	sk = sock->sk;
	if (sock->state == SS_UNCONNECTED) {
		err = -ENOTCONN;
		if (sk->sk_type == SOCK_STREAM)
			return err;
	} else {
		sock->state = SS_DISCONNECTING;
		err = 0;
	}

	/* Receive and send shutdowns are treated alike. */
	mode = mode & (RCV_SHUTDOWN | SEND_SHUTDOWN);
	if (mode) {
		lock_sock(sk);
		sk->sk_shutdown |= mode;
		sk->sk_state_change(sk);
		release_sock(sk);

		if (sk->sk_type == SOCK_STREAM) {
			sock_reset_flag(sk, SOCK_DONE);
			vsock_send_shutdown(sk, mode);
		}
	}

	return err;
}
