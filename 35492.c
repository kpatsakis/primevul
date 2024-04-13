static inline void ep_wake_up_nested(wait_queue_head_t *wqueue,
				     unsigned long events, int subclass)
{
	unsigned long flags;

	spin_lock_irqsave_nested(&wqueue->lock, flags, subclass);
	wake_up_locked_poll(wqueue, events);
	spin_unlock_irqrestore(&wqueue->lock, flags);
}
