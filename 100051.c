sec_disconnect(void)
{
	/* Perform a User-initiated disconnect sequence, see
	   [MS-RDPBCGR] 1.3.1.4 Disconnect Sequences */
	mcs_disconnect(RN_USER_REQUESTED);
}
