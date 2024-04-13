cifs_sb_tcon_pending_wait(void *unused)
{
	schedule();
	return signal_pending(current) ? -ERESTARTSYS : 0;
}
