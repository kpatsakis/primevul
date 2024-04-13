process_pdu_logon(STREAM s)
{
	uint32 infotype;
	in_uint32_le(s, infotype);

	switch (infotype)
	{
		case INFOTYPE_LOGON_PLAINNOTIFY:	/* TS_PLAIN_NOTIFY */
			logger(Protocol, Debug,
			       "process_pdu_logon(), Received TS_LOGIN_PLAIN_NOTIFY");
			in_uint8s(s, 576);	/* pad */
			break;

		case INFOTYPE_LOGON_EXTENDED_INF:	/* TS_LOGON_INFO_EXTENDED */
			logger(Protocol, Debug,
			       "process_pdu_logon(), Received TS_LOGIN_INFO_EXTENDED");
			process_ts_logon_info_extended(s);
			break;

		default:
			logger(Protocol, Warning,
			       "process_pdu_logon(), Unhandled login infotype %d", infotype);
	}
}
