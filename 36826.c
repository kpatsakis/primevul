int test_set_oom_score_adj(int new_val)
{
	struct sighand_struct *sighand = current->sighand;
	int old_val;

	spin_lock_irq(&sighand->siglock);
	old_val = current->signal->oom_score_adj;
	if (new_val != old_val) {
		if (new_val == OOM_SCORE_ADJ_MIN)
			atomic_inc(&current->mm->oom_disable_count);
		else if (old_val == OOM_SCORE_ADJ_MIN)
			atomic_dec(&current->mm->oom_disable_count);
		current->signal->oom_score_adj = new_val;
	}
	spin_unlock_irq(&sighand->siglock);

	return old_val;
}
