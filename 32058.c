static void ssh_detect_bugs(Ssh ssh, char *vstring)
{
    char *imp;			       /* pointer to implementation part */
    imp = vstring;
    imp += strcspn(imp, "-");
    if (*imp) imp++;
    imp += strcspn(imp, "-");
    if (*imp) imp++;

    ssh->remote_bugs = 0;

    /*
     * General notes on server version strings:
     *  - Not all servers reporting "Cisco-1.25" have all the bugs listed
     *    here -- in particular, we've heard of one that's perfectly happy
     *    with SSH1_MSG_IGNOREs -- but this string never seems to change,
     *    so we can't distinguish them.
     */
    if (conf_get_int(ssh->conf, CONF_sshbug_ignore1) == FORCE_ON ||
	(conf_get_int(ssh->conf, CONF_sshbug_ignore1) == AUTO &&
	 (!strcmp(imp, "1.2.18") || !strcmp(imp, "1.2.19") ||
	  !strcmp(imp, "1.2.20") || !strcmp(imp, "1.2.21") ||
	  !strcmp(imp, "1.2.22") || !strcmp(imp, "Cisco-1.25") ||
	  !strcmp(imp, "OSU_1.4alpha3") || !strcmp(imp, "OSU_1.5alpha4")))) {
	/*
	 * These versions don't support SSH1_MSG_IGNORE, so we have
	 * to use a different defence against password length
	 * sniffing.
	 */
	ssh->remote_bugs |= BUG_CHOKES_ON_SSH1_IGNORE;
	logevent("We believe remote version has SSH-1 ignore bug");
    }

    if (conf_get_int(ssh->conf, CONF_sshbug_plainpw1) == FORCE_ON ||
	(conf_get_int(ssh->conf, CONF_sshbug_plainpw1) == AUTO &&
	 (!strcmp(imp, "Cisco-1.25") || !strcmp(imp, "OSU_1.4alpha3")))) {
	/*
	 * These versions need a plain password sent; they can't
	 * handle having a null and a random length of data after
	 * the password.
	 */
	ssh->remote_bugs |= BUG_NEEDS_SSH1_PLAIN_PASSWORD;
	logevent("We believe remote version needs a plain SSH-1 password");
    }

    if (conf_get_int(ssh->conf, CONF_sshbug_rsa1) == FORCE_ON ||
	(conf_get_int(ssh->conf, CONF_sshbug_rsa1) == AUTO &&
	 (!strcmp(imp, "Cisco-1.25")))) {
	/*
	 * These versions apparently have no clue whatever about
	 * RSA authentication and will panic and die if they see
	 * an AUTH_RSA message.
	 */
	ssh->remote_bugs |= BUG_CHOKES_ON_RSA;
	logevent("We believe remote version can't handle SSH-1 RSA authentication");
    }

    if (conf_get_int(ssh->conf, CONF_sshbug_hmac2) == FORCE_ON ||
	(conf_get_int(ssh->conf, CONF_sshbug_hmac2) == AUTO &&
	 !wc_match("* VShell", imp) &&
	 (wc_match("2.1.0*", imp) || wc_match("2.0.*", imp) ||
	  wc_match("2.2.0*", imp) || wc_match("2.3.0*", imp) ||
	  wc_match("2.1 *", imp)))) {
	/*
	 * These versions have the HMAC bug.
	 */
	ssh->remote_bugs |= BUG_SSH2_HMAC;
	logevent("We believe remote version has SSH-2 HMAC bug");
    }

    if (conf_get_int(ssh->conf, CONF_sshbug_derivekey2) == FORCE_ON ||
	(conf_get_int(ssh->conf, CONF_sshbug_derivekey2) == AUTO &&
	 !wc_match("* VShell", imp) &&
	 (wc_match("2.0.0*", imp) || wc_match("2.0.10*", imp) ))) {
	/*
	 * These versions have the key-derivation bug (failing to
	 * include the literal shared secret in the hashes that
	 * generate the keys).
	 */
	ssh->remote_bugs |= BUG_SSH2_DERIVEKEY;
	logevent("We believe remote version has SSH-2 key-derivation bug");
    }

    if (conf_get_int(ssh->conf, CONF_sshbug_rsapad2) == FORCE_ON ||
	(conf_get_int(ssh->conf, CONF_sshbug_rsapad2) == AUTO &&
	 (wc_match("OpenSSH_2.[5-9]*", imp) ||
	  wc_match("OpenSSH_3.[0-2]*", imp) ||
	  wc_match("mod_sftp/0.[0-8]*", imp) ||
	  wc_match("mod_sftp/0.9.[0-8]", imp)))) {
	/*
	 * These versions have the SSH-2 RSA padding bug.
	 */
	ssh->remote_bugs |= BUG_SSH2_RSA_PADDING;
	logevent("We believe remote version has SSH-2 RSA padding bug");
    }

    if (conf_get_int(ssh->conf, CONF_sshbug_pksessid2) == FORCE_ON ||
	(conf_get_int(ssh->conf, CONF_sshbug_pksessid2) == AUTO &&
	 wc_match("OpenSSH_2.[0-2]*", imp))) {
	/*
	 * These versions have the SSH-2 session-ID bug in
	 * public-key authentication.
	 */
	ssh->remote_bugs |= BUG_SSH2_PK_SESSIONID;
	logevent("We believe remote version has SSH-2 public-key-session-ID bug");
    }

    if (conf_get_int(ssh->conf, CONF_sshbug_rekey2) == FORCE_ON ||
	(conf_get_int(ssh->conf, CONF_sshbug_rekey2) == AUTO &&
	 (wc_match("DigiSSH_2.0", imp) ||
	  wc_match("OpenSSH_2.[0-4]*", imp) ||
	  wc_match("OpenSSH_2.5.[0-3]*", imp) ||
	  wc_match("Sun_SSH_1.0", imp) ||
	  wc_match("Sun_SSH_1.0.1", imp) ||
	  /* All versions <= 1.2.6 (they changed their format in 1.2.7) */
	  wc_match("WeOnlyDo-*", imp)))) {
	/*
	 * These versions have the SSH-2 rekey bug.
	 */
	ssh->remote_bugs |= BUG_SSH2_REKEY;
	logevent("We believe remote version has SSH-2 rekey bug");
    }

    if (conf_get_int(ssh->conf, CONF_sshbug_maxpkt2) == FORCE_ON ||
	(conf_get_int(ssh->conf, CONF_sshbug_maxpkt2) == AUTO &&
	 (wc_match("1.36_sshlib GlobalSCAPE", imp) ||
          wc_match("1.36 sshlib: GlobalScape", imp)))) {
	/*
	 * This version ignores our makpkt and needs to be throttled.
	 */
	ssh->remote_bugs |= BUG_SSH2_MAXPKT;
	logevent("We believe remote version ignores SSH-2 maximum packet size");
    }

    if (conf_get_int(ssh->conf, CONF_sshbug_ignore2) == FORCE_ON) {
	/*
	 * Servers that don't support SSH2_MSG_IGNORE. Currently,
	 * none detected automatically.
	 */
	ssh->remote_bugs |= BUG_CHOKES_ON_SSH2_IGNORE;
	logevent("We believe remote version has SSH-2 ignore bug");
    }

    if (conf_get_int(ssh->conf, CONF_sshbug_oldgex2) == FORCE_ON ||
	(conf_get_int(ssh->conf, CONF_sshbug_oldgex2) == AUTO &&
	 (wc_match("OpenSSH_2.[235]*", imp)))) {
	/*
	 * These versions only support the original (pre-RFC4419)
	 * SSH-2 GEX request, and disconnect with a protocol error if
	 * we use the newer version.
	 */
	ssh->remote_bugs |= BUG_SSH2_OLDGEX;
	logevent("We believe remote version has outdated SSH-2 GEX");
    }

    if (conf_get_int(ssh->conf, CONF_sshbug_winadj) == FORCE_ON) {
	/*
	 * Servers that don't support our winadj request for one
	 * reason or another. Currently, none detected automatically.
	 */
	ssh->remote_bugs |= BUG_CHOKES_ON_WINADJ;
	logevent("We believe remote version has winadj bug");
    }

    if (conf_get_int(ssh->conf, CONF_sshbug_chanreq) == FORCE_ON ||
	(conf_get_int(ssh->conf, CONF_sshbug_chanreq) == AUTO &&
	 (wc_match("OpenSSH_[2-5].*", imp) ||
	  wc_match("OpenSSH_6.[0-6]*", imp) ||
	  wc_match("dropbear_0.[2-4][0-9]*", imp) ||
	  wc_match("dropbear_0.5[01]*", imp)))) {
	/*
	 * These versions have the SSH-2 channel request bug.
	 * OpenSSH 6.7 and above do not:
	 * https://bugzilla.mindrot.org/show_bug.cgi?id=1818
	 * dropbear_0.52 and above do not:
	 * https://secure.ucc.asn.au/hg/dropbear/rev/cd02449b709c
	 */
	ssh->remote_bugs |= BUG_SENDS_LATE_REQUEST_REPLY;
	logevent("We believe remote version has SSH-2 channel request bug");
    }
}
