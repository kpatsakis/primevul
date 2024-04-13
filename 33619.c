const void *OBJ_bsearch_(const void *key, const void *base, int num, int size,
			 int (*cmp)(const void *, const void *))
	{
	return OBJ_bsearch_ex_(key, base, num, size, cmp, 0);
	}
