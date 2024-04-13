static inline int try_get_ioctx(struct kioctx *kioctx)
{
	return atomic_inc_not_zero(&kioctx->users);
}
