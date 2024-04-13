static void pop_end(stroke_msg_t *msg, const char* label, stroke_end_t *end)
{
	pop_string(msg, &end->address);
	pop_string(msg, &end->subnets);
	pop_string(msg, &end->sourceip);
	pop_string(msg, &end->dns);
	pop_string(msg, &end->auth);
	pop_string(msg, &end->auth2);
	pop_string(msg, &end->id);
	pop_string(msg, &end->id2);
	pop_string(msg, &end->rsakey);
	pop_string(msg, &end->cert);
	pop_string(msg, &end->cert2);
	pop_string(msg, &end->ca);
	pop_string(msg, &end->ca2);
	pop_string(msg, &end->groups);
	pop_string(msg, &end->groups2);
	pop_string(msg, &end->cert_policy);
	pop_string(msg, &end->updown);

	DBG_OPT("  %s=%s", label, end->address);
	DBG_OPT("  %ssubnet=%s", label, end->subnets);
	DBG_OPT("  %ssourceip=%s", label, end->sourceip);
	DBG_OPT("  %sdns=%s", label, end->dns);
	DBG_OPT("  %sauth=%s", label, end->auth);
	DBG_OPT("  %sauth2=%s", label, end->auth2);
	DBG_OPT("  %sid=%s", label, end->id);
	DBG_OPT("  %sid2=%s", label, end->id2);
	DBG_OPT("  %srsakey=%s", label, end->rsakey);
	DBG_OPT("  %scert=%s", label, end->cert);
	DBG_OPT("  %scert2=%s", label, end->cert2);
	DBG_OPT("  %sca=%s", label, end->ca);
	DBG_OPT("  %sca2=%s", label, end->ca2);
	DBG_OPT("  %sgroups=%s", label, end->groups);
	DBG_OPT("  %sgroups2=%s", label, end->groups2);
	DBG_OPT("  %supdown=%s", label, end->updown);
}
