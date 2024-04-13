ins_compl_add_tv(typval_T *tv, int dir, int fast)
{
    char_u	*word;
    int		dup = FALSE;
    int		empty = FALSE;
    int		flags = fast ? CP_FAST : 0;
    char_u	*(cptext[CPT_COUNT]);
    typval_T	user_data;
    int		status;

    user_data.v_type = VAR_UNKNOWN;
    if (tv->v_type == VAR_DICT && tv->vval.v_dict != NULL)
    {
	word = dict_get_string(tv->vval.v_dict, (char_u *)"word", FALSE);
	cptext[CPT_ABBR] = dict_get_string(tv->vval.v_dict,
						     (char_u *)"abbr", FALSE);
	cptext[CPT_MENU] = dict_get_string(tv->vval.v_dict,
						     (char_u *)"menu", FALSE);
	cptext[CPT_KIND] = dict_get_string(tv->vval.v_dict,
						     (char_u *)"kind", FALSE);
	cptext[CPT_INFO] = dict_get_string(tv->vval.v_dict,
						     (char_u *)"info", FALSE);
	dict_get_tv(tv->vval.v_dict, (char_u *)"user_data", &user_data);
	if (dict_get_string(tv->vval.v_dict, (char_u *)"icase", FALSE) != NULL
			&& dict_get_number(tv->vval.v_dict, (char_u *)"icase"))
	    flags |= CP_ICASE;
	if (dict_get_string(tv->vval.v_dict, (char_u *)"dup", FALSE) != NULL)
	    dup = dict_get_number(tv->vval.v_dict, (char_u *)"dup");
	if (dict_get_string(tv->vval.v_dict, (char_u *)"empty", FALSE) != NULL)
	    empty = dict_get_number(tv->vval.v_dict, (char_u *)"empty");
	if (dict_get_string(tv->vval.v_dict, (char_u *)"equal", FALSE) != NULL
			&& dict_get_number(tv->vval.v_dict, (char_u *)"equal"))
	    flags |= CP_EQUAL;
    }
    else
    {
	word = tv_get_string_chk(tv);
	CLEAR_FIELD(cptext);
    }
    if (word == NULL || (!empty && *word == NUL))
    {
	clear_tv(&user_data);
	return FAIL;
    }
    status = ins_compl_add(word, -1, NULL, cptext, &user_data, dir, flags, dup);
    if (status != OK)
	clear_tv(&user_data);
    return status;
}