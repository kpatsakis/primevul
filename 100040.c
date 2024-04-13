mcs_disconnect(int reason)
{
	mcs_send_dpu(reason);
	iso_disconnect();
}
