static void stroke_purge(private_stroke_socket_t *this,
						 stroke_msg_t *msg, FILE *out)
{
	if (msg->purge.flags & PURGE_OCSP)
	{
		lib->credmgr->flush_cache(lib->credmgr, CERT_X509_OCSP_RESPONSE);
	}
	if (msg->purge.flags & PURGE_CRLS)
	{
		lib->credmgr->flush_cache(lib->credmgr, CERT_X509_CRL);
	}
	if (msg->purge.flags & PURGE_CERTS)
	{
		lib->credmgr->flush_cache(lib->credmgr, CERT_X509);
	}
	if (msg->purge.flags & PURGE_IKE)
	{
		this->control->purge_ike(this->control, msg, out);
	}
}
