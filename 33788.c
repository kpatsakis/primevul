kex_from_blob(struct sshbuf *m, struct kex **kexp)
{
	struct kex *kex;
	int r;

	if ((kex = calloc(1, sizeof(struct kex))) == NULL ||
	    (kex->my = sshbuf_new()) == NULL ||
	    (kex->peer = sshbuf_new()) == NULL) {
		r = SSH_ERR_ALLOC_FAIL;
		goto out;
	}
	if ((r = sshbuf_get_string(m, &kex->session_id, &kex->session_id_len)) != 0 ||
	    (r = sshbuf_get_u32(m, &kex->we_need)) != 0 ||
	    (r = sshbuf_get_u32(m, (u_int *)&kex->hostkey_type)) != 0 ||
	    (r = sshbuf_get_u32(m, &kex->kex_type)) != 0 ||
	    (r = sshbuf_get_stringb(m, kex->my)) != 0 ||
	    (r = sshbuf_get_stringb(m, kex->peer)) != 0 ||
	    (r = sshbuf_get_u32(m, &kex->flags)) != 0 ||
	    (r = sshbuf_get_cstring(m, &kex->client_version_string, NULL)) != 0 ||
	    (r = sshbuf_get_cstring(m, &kex->server_version_string, NULL)) != 0)
		goto out;
	kex->server = 1;
	kex->done = 1;
	r = 0;
 out:
	if (r != 0 || kexp == NULL) {
		if (kex != NULL) {
			if (kex->my != NULL)
				sshbuf_free(kex->my);
			if (kex->peer != NULL)
				sshbuf_free(kex->peer);
			free(kex);
		}
		if (kexp != NULL)
			*kexp = NULL;
	} else {
		*kexp = kex;
	}
	return r;
}
