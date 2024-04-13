static inline void free_copy(struct msg_msg *copy)
{
	if (copy)
		free_msg(copy);
}
