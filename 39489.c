smb_sendv(struct TCP_Server_Info *server, struct kvec *iov, int n_vec)
{
	struct smb_rqst rqst = { .rq_iov = iov,
				 .rq_nvec = n_vec };

	return smb_send_rqst(server, &rqst);
}
