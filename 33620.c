void OBJ_cleanup(void)
	{
	if (obj_cleanup_defer)
		{
		obj_cleanup_defer = 2;
		return ;
		}
	if (added == NULL) return;
	lh_ADDED_OBJ_down_load(added) = 0;
	lh_ADDED_OBJ_doall(added,LHASH_DOALL_FN(cleanup1)); /* zero counters */
	lh_ADDED_OBJ_doall(added,LHASH_DOALL_FN(cleanup2)); /* set counters */
	lh_ADDED_OBJ_doall(added,LHASH_DOALL_FN(cleanup3)); /* free objects */
	lh_ADDED_OBJ_free(added);
	added=NULL;
	}
