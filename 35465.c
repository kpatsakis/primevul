static unsigned int ep_eventpoll_poll(struct file *file, poll_table *wait)
{
	int pollflags;
	struct eventpoll *ep = file->private_data;

	/* Insert inside our poll wait queue */
	poll_wait(file, &ep->poll_wait, wait);

	/*
	 * Proceed to find out if wanted events are really available inside
	 * the ready list. This need to be done under ep_call_nested()
	 * supervision, since the call to f_op->poll() done on listed files
	 * could re-enter here.
	 */
	pollflags = ep_call_nested(&poll_readywalk_ncalls, EP_MAX_NESTS,
				   ep_poll_readyevents_proc, ep, ep, current);

	return pollflags != -1 ? pollflags : 0;
}
