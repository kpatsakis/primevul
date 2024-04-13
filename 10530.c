static bool rtrs_clt_change_state_from_to(struct rtrs_clt_path *clt_path,
					   enum rtrs_clt_state old_state,
					   enum rtrs_clt_state new_state)
{
	bool changed = false;

	spin_lock_irq(&clt_path->state_wq.lock);
	if (clt_path->state == old_state)
		changed = rtrs_clt_change_state(clt_path, new_state);
	spin_unlock_irq(&clt_path->state_wq.lock);

	return changed;
}