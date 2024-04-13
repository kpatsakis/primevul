check_readonly(int *forceit, buf_T *buf)
{
    stat_T	st;

    // Handle a file being readonly when the 'readonly' option is set or when
    // the file exists and permissions are read-only.
    // We will send 0777 to check_file_readonly(), as the "perm" variable is
    // important for device checks but not here.
    if (!*forceit && (buf->b_p_ro
		|| (mch_stat((char *)buf->b_ffname, &st) >= 0
		    && check_file_readonly(buf->b_ffname, 0777))))
    {
#if defined(FEAT_GUI_DIALOG) || defined(FEAT_CON_DIALOG)
	if ((p_confirm || (cmdmod.cmod_flags & CMOD_CONFIRM))
						       && buf->b_fname != NULL)
	{
	    char_u	buff[DIALOG_MSG_SIZE];

	    if (buf->b_p_ro)
		dialog_msg(buff, _("'readonly' option is set for \"%s\".\nDo you wish to write anyway?"),
		    buf->b_fname);
	    else
		dialog_msg(buff, _("File permissions of \"%s\" are read-only.\nIt may still be possible to write it.\nDo you wish to try?"),
		    buf->b_fname);

	    if (vim_dialog_yesno(VIM_QUESTION, NULL, buff, 2) == VIM_YES)
	    {
		// Set forceit, to force the writing of a readonly file
		*forceit = TRUE;
		return FALSE;
	    }
	    else
		return TRUE;
	}
	else
#endif
	if (buf->b_p_ro)
	    emsg(_(e_readonly_option_is_set_add_bang_to_override));
	else
	    semsg(_(e_str_is_read_only_add_bang_to_override), buf->b_fname);
	return TRUE;
    }

    return FALSE;
}