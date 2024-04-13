static void ssh_queue_incoming_data(Ssh ssh,
				    const unsigned char **data, int *datalen)
{
    bufchain_add(&ssh->queued_incoming_data, *data, *datalen);
    *data += *datalen;
    *datalen = 0;
}
