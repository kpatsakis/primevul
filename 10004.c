check_restricted(void)
{
    if (restricted)
    {
	emsg(_(e_shell_commands_and_some_functionality_not_allowed_in_rvim));
	return TRUE;
    }
    return FALSE;
}