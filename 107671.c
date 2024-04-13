static TEE_Result check_ta_store(void)
{
	const struct user_ta_store_ops *op = NULL;

	SCATTERED_ARRAY_FOREACH(op, ta_stores, struct user_ta_store_ops)
		DMSG("TA store: \"%s\"", op->description);

	return TEE_SUCCESS;
}
