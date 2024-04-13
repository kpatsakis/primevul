void xtables_unlock(int lock)
{
	if (lock >= 0)
		close(lock);
}
