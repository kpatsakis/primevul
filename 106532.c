hstore_cmp(PG_FUNCTION_ARGS)
{
	HStore	   *hs1 = PG_GETARG_HS(0);
	HStore	   *hs2 = PG_GETARG_HS(1);
	int			hcount1 = HS_COUNT(hs1);
	int			hcount2 = HS_COUNT(hs2);
	int			res = 0;

	if (hcount1 == 0 || hcount2 == 0)
	{
		/*
		 * if either operand is empty, and the other is nonempty, the nonempty
		 * one is larger. If both are empty they are equal.
		 */
		if (hcount1 > 0)
			res = 1;
		else if (hcount2 > 0)
			res = -1;
	}
	else
	{
		/* here we know both operands are nonempty */
		char	   *str1 = STRPTR(hs1);
		char	   *str2 = STRPTR(hs2);
		HEntry	   *ent1 = ARRPTR(hs1);
		HEntry	   *ent2 = ARRPTR(hs2);
		size_t		len1 = HSE_ENDPOS(ent1[2 * hcount1 - 1]);
		size_t		len2 = HSE_ENDPOS(ent2[2 * hcount2 - 1]);

		res = memcmp(str1, str2, Min(len1, len2));

		if (res == 0)
		{
			if (len1 > len2)
				res = 1;
			else if (len1 < len2)
				res = -1;
			else if (hcount1 > hcount2)
				res = 1;
			else if (hcount2 > hcount1)
				res = -1;
			else
			{
				int			count = hcount1 * 2;
				int			i;

				for (i = 0; i < count; ++i)
					if (HSE_ENDPOS(ent1[i]) != HSE_ENDPOS(ent2[i]) ||
						HSE_ISNULL(ent1[i]) != HSE_ISNULL(ent2[i]))
						break;
				if (i < count)
				{
					if (HSE_ENDPOS(ent1[i]) < HSE_ENDPOS(ent2[i]))
						res = -1;
					else if (HSE_ENDPOS(ent1[i]) > HSE_ENDPOS(ent2[i]))
						res = 1;
					else if (HSE_ISNULL(ent1[i]))
						res = 1;
					else if (HSE_ISNULL(ent2[i]))
						res = -1;
				}
			}
		}
		else
		{
			res = (res > 0) ? 1 : -1;
		}
	}

	/*
	 * this is a btree support function; this is one of the few places where
	 * memory needs to be explicitly freed.
	 */
	PG_FREE_IF_COPY(hs1, 0);
	PG_FREE_IF_COPY(hs2, 1);
	PG_RETURN_INT32(res);
}
