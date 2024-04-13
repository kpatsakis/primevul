static unsigned int userns_inum(void *ns)
{
	struct user_namespace *user_ns = ns;
	return user_ns->proc_inum;
}
