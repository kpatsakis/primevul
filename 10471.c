static bool rtrs_clt_change_state_get_old(struct rtrs_clt_path *clt_path,
					   enum rtrs_clt_state new_state,
					   enum rtrs_clt_state *old_state)
{
	bool changed;

	spin_lock_irq(&clt_path->state_wq.lock);
	if (old_state)
		*old_state = clt_path->state;
	changed = rtrs_clt_change_state(clt_path, new_state);
	spin_unlock_irq(&clt_path->state_wq.lock);

	return changed;
}