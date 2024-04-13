preprocs_left(void)
{
    return
# ifdef FEAT_SMARTINDENT
#  ifdef FEAT_CINDENT
	(curbuf->b_p_si && !curbuf->b_p_cin) ||
#  else
	curbuf->b_p_si
#  endif
# endif
# ifdef FEAT_CINDENT
	(curbuf->b_p_cin && in_cinkeys('#', ' ', TRUE)
					   && curbuf->b_ind_hash_comment == 0)
# endif
	;
}