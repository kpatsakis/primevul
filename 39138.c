static int sysvipc_sem_proc_show(struct seq_file *s, void *it)
{
	struct user_namespace *user_ns = seq_user_ns(s);
	struct sem_array *sma = it;

	return seq_printf(s,
			  "%10d %10d  %4o %10u %5u %5u %5u %5u %10lu %10lu\n",
			  sma->sem_perm.key,
			  sma->sem_perm.id,
			  sma->sem_perm.mode,
			  sma->sem_nsems,
			  from_kuid_munged(user_ns, sma->sem_perm.uid),
			  from_kgid_munged(user_ns, sma->sem_perm.gid),
			  from_kuid_munged(user_ns, sma->sem_perm.cuid),
			  from_kgid_munged(user_ns, sma->sem_perm.cgid),
			  sma->sem_otime,
			  sma->sem_ctime);
}
