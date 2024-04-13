check_writable(char_u *fname)
{
    if (mch_nodetype(fname) == NODE_OTHER)
    {
	semsg(_(e_str_is_not_file_or_writable_device), fname);
	return FAIL;
    }
    return OK;
}