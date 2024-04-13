delbuf_msg(char_u *name)
{
    semsg(_(e_autocommands_unexpectedly_deleted_new_buffer_str),
					   name == NULL ? (char_u *)"" : name);
    vim_free(name);
    au_new_curbuf.br_buf = NULL;
    au_new_curbuf.br_buf_free_count = 0;
}