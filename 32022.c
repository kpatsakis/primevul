static void parse_ttymodes(Ssh ssh,
                           void (*do_mode)(void *data,
                                           const struct ssh_ttymode *mode,
                                           char *val),
			   void *data)
{
    int i;
    const struct ssh_ttymode *mode;
    char *val;
    char default_val[2];

    strcpy(default_val, "A");

    for (i = 0; i < lenof(ssh_ttymodes); i++) {
        mode = ssh_ttymodes + i;
        val = conf_get_str_str_opt(ssh->conf, CONF_ttymodes, mode->mode);
        if (!val)
            val = default_val;

	/*
	 * val[0] is either 'V', indicating that an explicit value
	 * follows it, or 'A' indicating that we should pass the
	 * value through from the local environment via get_ttymode.
	 */
	if (val[0] == 'A') {
	    val = get_ttymode(ssh->frontend, mode->mode);
	    if (val) {
		do_mode(data, mode, val);
		sfree(val);
	    }
	} else
            do_mode(data, mode, val + 1);              /* skip the 'V' */
    }
}
