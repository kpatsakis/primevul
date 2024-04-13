session_env_req(Session *s)
{
	char *name, *val;
	u_int name_len, val_len, i;

	name = packet_get_cstring(&name_len);
	val = packet_get_cstring(&val_len);
	packet_check_eom();

	/* Don't set too many environment variables */
	if (s->num_env > 128) {
		debug2("Ignoring env request %s: too many env vars", name);
		goto fail;
	}

	for (i = 0; i < options.num_accept_env; i++) {
		if (match_pattern(name, options.accept_env[i])) {
			debug2("Setting env %d: %s=%s", s->num_env, name, val);
			s->env = xreallocarray(s->env, s->num_env + 1,
			    sizeof(*s->env));
			s->env[s->num_env].name = name;
			s->env[s->num_env].val = val;
			s->num_env++;
			return (1);
		}
	}
	debug2("Ignoring env request %s: disallowed name", name);

 fail:
	free(name);
	free(val);
	return (0);
}
