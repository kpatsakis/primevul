ppp_unregister_channel(struct ppp_channel *chan)
{
	struct channel *pch = chan->ppp;

	if (!pch)
		return;		/* should never happen */
	chan->ppp = NULL;

	/*
	 * This ensures that we have returned from any calls into the
	 * the channel's start_xmit or ioctl routine before we proceed.
	 */
	down_write(&pch->chan_sem);
	spin_lock_bh(&pch->downl);
	pch->chan = NULL;
	spin_unlock_bh(&pch->downl);
	up_write(&pch->chan_sem);
	ppp_disconnect_channel(pch);
	spin_lock_bh(&all_channels_lock);
	list_del(&pch->list);
	spin_unlock_bh(&all_channels_lock);
	pch->file.dead = 1;
	wake_up_interruptible(&pch->file.rwait);
	if (atomic_dec_and_test(&pch->file.refcnt))
		ppp_destroy_channel(pch);
}