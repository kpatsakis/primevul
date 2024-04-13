static bool ldm_compare_tocblocks (const struct tocblock *toc1,
				   const struct tocblock *toc2)
{
	BUG_ON (!toc1 || !toc2);

	return ((toc1->bitmap1_start == toc2->bitmap1_start)	&&
		(toc1->bitmap1_size  == toc2->bitmap1_size)	&&
		(toc1->bitmap2_start == toc2->bitmap2_start)	&&
		(toc1->bitmap2_size  == toc2->bitmap2_size)	&&
		!strncmp (toc1->bitmap1_name, toc2->bitmap1_name,
			sizeof (toc1->bitmap1_name))		&&
		!strncmp (toc1->bitmap2_name, toc2->bitmap2_name,
			sizeof (toc1->bitmap2_name)));
}
