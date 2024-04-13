static const char * stateStr(rpmfileState fstate)
{
    switch (fstate) {
    case RPMFILE_STATE_NORMAL:
	return NULL;
    case RPMFILE_STATE_NOTINSTALLED:
	return rpmIsVerbose() ? _("not installed") : NULL;
    case RPMFILE_STATE_NETSHARED:
	return rpmIsVerbose() ? _("net shared") : NULL;
    case RPMFILE_STATE_WRONGCOLOR:
	return rpmIsVerbose() ? _("wrong color") : NULL;
    case RPMFILE_STATE_REPLACED:
	return _("replaced");
    case RPMFILE_STATE_MISSING:
	return _("no state");
    }
    return _("unknown state");
}
