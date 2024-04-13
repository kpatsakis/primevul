static void ssh_process_queued_incoming_data(Ssh ssh)
{
    void *vdata;
    const unsigned char *data;
    int len, origlen;

    while (!ssh->frozen && bufchain_size(&ssh->queued_incoming_data)) {
	bufchain_prefix(&ssh->queued_incoming_data, &vdata, &len);
	data = vdata;
	origlen = len;

	while (!ssh->frozen && len > 0)
	    ssh_process_incoming_data(ssh, &data, &len);

	if (origlen > len)
	    bufchain_consume(&ssh->queued_incoming_data, origlen - len);
    }
}
