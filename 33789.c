kex_to_blob(struct sshbuf *m, struct kex *kex)
{
	int r;

	if ((r = sshbuf_put_string(m, kex->session_id,
	    kex->session_id_len)) != 0 ||
	    (r = sshbuf_put_u32(m, kex->we_need)) != 0 ||
	    (r = sshbuf_put_u32(m, kex->hostkey_type)) != 0 ||
	    (r = sshbuf_put_u32(m, kex->kex_type)) != 0 ||
	    (r = sshbuf_put_stringb(m, kex->my)) != 0 ||
	    (r = sshbuf_put_stringb(m, kex->peer)) != 0 ||
	    (r = sshbuf_put_u32(m, kex->flags)) != 0 ||
	    (r = sshbuf_put_cstring(m, kex->client_version_string)) != 0 ||
	    (r = sshbuf_put_cstring(m, kex->server_version_string)) != 0)
		return r;
	return 0;
}
