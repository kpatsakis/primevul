static const char *ssh_pkt_type(Ssh ssh, int type)
{
    if (ssh->version == 1)
	return ssh1_pkt_type(type);
    else
	return ssh2_pkt_type(ssh->pkt_kctx, ssh->pkt_actx, type);
}
