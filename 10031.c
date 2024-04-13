do_wqall(exarg_T *eap)
{
    buf_T	*buf;
    int		error = 0;
    int		save_forceit = eap->forceit;

    if (eap->cmdidx == CMD_xall || eap->cmdidx == CMD_wqall)
	exiting = TRUE;

    FOR_ALL_BUFFERS(buf)
    {
#ifdef FEAT_TERMINAL
	if (exiting && term_job_running(buf->b_term))
	{
	    no_write_message_nobang(buf);
	    ++error;
	}
	else
#endif
	if (bufIsChanged(buf) && !bt_dontwrite(buf))
	{
	    /*
	     * Check if there is a reason the buffer cannot be written:
	     * 1. if the 'write' option is set
	     * 2. if there is no file name (even after browsing)
	     * 3. if the 'readonly' is set (even after a dialog)
	     * 4. if overwriting is allowed (even after a dialog)
	     */
	    if (not_writing())
	    {
		++error;
		break;
	    }
#ifdef FEAT_BROWSE
	    // ":browse wall": ask for file name if there isn't one
	    if (buf->b_ffname == NULL && (cmdmod.cmod_flags & CMOD_BROWSE))
		browse_save_fname(buf);
#endif
	    if (buf->b_ffname == NULL)
	    {
		semsg(_(e_no_file_name_for_buffer_nr), (long)buf->b_fnum);
		++error;
	    }
	    else if (check_readonly(&eap->forceit, buf)
		    || check_overwrite(eap, buf, buf->b_fname, buf->b_ffname,
							       FALSE) == FAIL)
	    {
		++error;
	    }
	    else
	    {
		bufref_T bufref;

		set_bufref(&bufref, buf);
		if (buf_write_all(buf, eap->forceit) == FAIL)
		    ++error;
		// an autocommand may have deleted the buffer
		if (!bufref_valid(&bufref))
		    buf = firstbuf;
	    }
	    eap->forceit = save_forceit;    // check_overwrite() may set it
	}
    }
    if (exiting)
    {
	if (!error)
	    getout(0);		// exit Vim
	not_exiting();
    }
}