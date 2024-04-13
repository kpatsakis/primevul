static int init_added(void)
	{
	if (added != NULL) return(1);
	added=lh_ADDED_OBJ_new();
	return(added != NULL);
	}
