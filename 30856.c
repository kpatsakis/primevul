void save_argv(void)
{
	unsigned int i;

	while (oldargc)
		free(oldargv[--oldargc]);

	oldargc = newargc;
	newargc = 0;
	for (i = 0; i < oldargc; i++) {
		oldargv[i] = newargv[i];
 	}
 }
