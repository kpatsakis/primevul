static int ssh_closing(Plug plug, const char *error_msg, int error_code,
		       int calling_back)
{
    Ssh ssh = (Ssh) plug;
    int need_notify = ssh_do_close(ssh, FALSE);

    if (!error_msg) {
	if (!ssh->close_expected)
	    error_msg = "Server unexpectedly closed network connection";
	else
	    error_msg = "Server closed network connection";
    }

    if (ssh->close_expected && ssh->clean_exit && ssh->exitcode < 0)
	ssh->exitcode = 0;

    if (need_notify)
        notify_remote_exit(ssh->frontend);

    if (error_msg)
	logevent(error_msg);
    if (!ssh->close_expected || !ssh->clean_exit)
	connection_fatal(ssh->frontend, "%s", error_msg);
    return 0;
}
