void sock_update_classid(struct sock *sk)
{
	u32 classid;

	rcu_read_lock();  /* doing current task, which cannot vanish. */
	classid = task_cls_classid(current);
	rcu_read_unlock();
	if (classid && classid != sk->sk_classid)
		sk->sk_classid = classid;
}
