static void release_tgcred(struct cred *cred)
{
#ifdef CONFIG_KEYS
	struct thread_group_cred *tgcred = cred->tgcred;

	if (atomic_dec_and_test(&tgcred->usage))
		call_rcu(&tgcred->rcu, release_tgcred_rcu);
#endif
}
