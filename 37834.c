find_inlist_lock_noload(struct list_head *head, const char *name, int *error,
   struct mutex *mutex)
{
	struct {
		struct list_head list;
		char name[EBT_FUNCTION_MAXNAMELEN];
	} *e;

	*error = mutex_lock_interruptible(mutex);
	if (*error != 0)
		return NULL;

	list_for_each_entry(e, head, list) {
		if (strcmp(e->name, name) == 0)
			return e;
	}
	*error = -ENOENT;
	mutex_unlock(mutex);
	return NULL;
}
