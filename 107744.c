static void register_shm(struct thread_smc_args *smc_args,
			 struct optee_msg_arg *arg, uint32_t num_params)
{
	arg->ret = TEE_ERROR_BAD_PARAMETERS;
	smc_args->a0 = OPTEE_SMC_RETURN_OK;

	if (num_params != 1 ||
	    (arg->params[0].attr !=
	     (OPTEE_MSG_ATTR_TYPE_TMEM_OUTPUT | OPTEE_MSG_ATTR_NONCONTIG)))
		return;

	struct optee_msg_param_tmem *tmem = &arg->params[0].u.tmem;
	struct mobj *mobj = msg_param_mobj_from_noncontig(tmem->buf_ptr,
							  tmem->size,
							  tmem->shm_ref, false);

	if (!mobj)
		return;

	mobj_reg_shm_unguard(mobj);
	arg->ret = TEE_SUCCESS;
}
