sshpkt_fatal(struct ssh *ssh, const char *tag, int r)
{
	switch (r) {
	case SSH_ERR_CONN_CLOSED:
		logit("Connection closed by %.200s", ssh_remote_ipaddr(ssh));
		cleanup_exit(255);
	case SSH_ERR_CONN_TIMEOUT:
		logit("Connection to %.200s timed out", ssh_remote_ipaddr(ssh));
		cleanup_exit(255);
	case SSH_ERR_DISCONNECTED:
		logit("Disconnected from %.200s",
		    ssh_remote_ipaddr(ssh));
		cleanup_exit(255);
	case SSH_ERR_SYSTEM_ERROR:
		if (errno == ECONNRESET) {
			logit("Connection reset by %.200s",
			    ssh_remote_ipaddr(ssh));
			cleanup_exit(255);
		}
		/* FALLTHROUGH */
	case SSH_ERR_NO_CIPHER_ALG_MATCH:
	case SSH_ERR_NO_MAC_ALG_MATCH:
	case SSH_ERR_NO_COMPRESS_ALG_MATCH:
	case SSH_ERR_NO_KEX_ALG_MATCH:
	case SSH_ERR_NO_HOSTKEY_ALG_MATCH:
		if (ssh && ssh->kex && ssh->kex->failed_choice) {
			fatal("Unable to negotiate with %.200s: %s. "
			    "Their offer: %s", ssh_remote_ipaddr(ssh),
			    ssh_err(r), ssh->kex->failed_choice);
		}
		/* FALLTHROUGH */
	default:
		fatal("%s%sConnection to %.200s: %s",
		    tag != NULL ? tag : "", tag != NULL ? ": " : "",
		    ssh_remote_ipaddr(ssh), ssh_err(r));
	}
}
