static int do_ssh_init(Ssh ssh, unsigned char c)
{
    static const char protoname[] = "SSH-";

    struct do_ssh_init_state {
	int crLine;
	int vslen;
	char version[10];
	char *vstring;
	int vstrsize;
	int i;
	int proto1, proto2;
    };
    crState(do_ssh_init_state);
    
    crBeginState;

    /* Search for a line beginning with the protocol name prefix in
     * the input. */
    for (;;) {
        for (s->i = 0; protoname[s->i]; s->i++) {
            if ((char)c != protoname[s->i]) goto no;
            crReturn(1);
        }
	break;
      no:
	while (c != '\012')
	    crReturn(1);
	crReturn(1);
    }

    ssh->session_started = TRUE;

    s->vstrsize = sizeof(protoname) + 16;
    s->vstring = snewn(s->vstrsize, char);
    strcpy(s->vstring, protoname);
    s->vslen = strlen(protoname);
    s->i = 0;
    while (1) {
	if (s->vslen >= s->vstrsize - 1) {
	    s->vstrsize += 16;
	    s->vstring = sresize(s->vstring, s->vstrsize, char);
	}
	s->vstring[s->vslen++] = c;
	if (s->i >= 0) {
	    if (c == '-') {
		s->version[s->i] = '\0';
		s->i = -1;
	    } else if (s->i < sizeof(s->version) - 1)
		s->version[s->i++] = c;
	} else if (c == '\012')
	    break;
	crReturn(1);		       /* get another char */
    }

    ssh->agentfwd_enabled = FALSE;
    ssh->rdpkt2_state.incoming_sequence = 0;

    s->vstring[s->vslen] = 0;
    s->vstring[strcspn(s->vstring, "\015\012")] = '\0';/* remove EOL chars */
    logeventf(ssh, "Server version: %s", s->vstring);
    ssh_detect_bugs(ssh, s->vstring);

    /*
     * Decide which SSH protocol version to support.
     */

    /* Anything strictly below "2.0" means protocol 1 is supported. */
    s->proto1 = ssh_versioncmp(s->version, "2.0") < 0;
    /* Anything greater or equal to "1.99" means protocol 2 is supported. */
    s->proto2 = ssh_versioncmp(s->version, "1.99") >= 0;

    if (conf_get_int(ssh->conf, CONF_sshprot) == 0) {
	if (!s->proto1) {
	    bombout(("SSH protocol version 1 required by our configuration "
		     "but not provided by server"));
	    crStop(0);
	}
    } else if (conf_get_int(ssh->conf, CONF_sshprot) == 3) {
	if (!s->proto2) {
	    bombout(("SSH protocol version 2 required by our configuration "
		     "but server only provides (old, insecure) SSH-1"));
	    crStop(0);
	}
    } else {
	/* No longer support values 1 or 2 for CONF_sshprot */
	assert(!"Unexpected value for CONF_sshprot");
    }

    if (s->proto2 && (conf_get_int(ssh->conf, CONF_sshprot) >= 2 || !s->proto1))
	ssh->version = 2;
    else
	ssh->version = 1;

    logeventf(ssh, "Using SSH protocol version %d", ssh->version);

    /* Send the version string, if we haven't already */
    if (conf_get_int(ssh->conf, CONF_sshprot) != 3)
	ssh_send_verstring(ssh, protoname, s->version);

    if (ssh->version == 2) {
	size_t len;
	/*
	 * Record their version string.
	 */
	len = strcspn(s->vstring, "\015\012");
	ssh->v_s = snewn(len + 1, char);
	memcpy(ssh->v_s, s->vstring, len);
	ssh->v_s[len] = 0;
	    
	/*
	 * Initialise SSH-2 protocol.
	 */
	ssh->protocol = ssh2_protocol;
	ssh2_protocol_setup(ssh);
	ssh->s_rdpkt = ssh2_rdpkt;
    } else {
	/*
	 * Initialise SSH-1 protocol.
	 */
	ssh->protocol = ssh1_protocol;
	ssh1_protocol_setup(ssh);
	ssh->s_rdpkt = ssh1_rdpkt;
    }
    if (ssh->version == 2)
	do_ssh2_transport(ssh, NULL, -1, NULL);

    update_specials_menu(ssh->frontend);
    ssh->state = SSH_STATE_BEFORE_SIZE;
    ssh->pinger = pinger_new(ssh->conf, &ssh_backend, ssh);

    sfree(s->vstring);

    crFinish(0);
}
