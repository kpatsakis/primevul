check_secure(void)
{
    if (secure)
    {
	secure = 2;
	emsg(_(e_command_not_allowed_from_vimrc_in_current_dir_or_tag_search));
	return TRUE;
    }
#ifdef HAVE_SANDBOX
    /*
     * In the sandbox more things are not allowed, including the things
     * disallowed in secure mode.
     */
    if (sandbox != 0)
    {
	emsg(_(e_not_allowed_in_sandbox));
	return TRUE;
    }
#endif
    return FALSE;
}