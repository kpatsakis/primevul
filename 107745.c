static TEE_Result set_tmem_param(const struct optee_msg_param_tmem *tmem,
				 uint32_t attr, struct param_mem *mem)
{
	struct mobj __maybe_unused **mobj;
	paddr_t pa = READ_ONCE(tmem->buf_ptr);
	size_t sz = READ_ONCE(tmem->size);

	/* NULL Memory Rerefence? */
	if (!pa && !sz) {
		mem->mobj = NULL;
		mem->offs = 0;
		mem->size = 0;
		return TEE_SUCCESS;
	}

	/* Non-contigous buffer from non sec DDR? */
	if (attr & OPTEE_MSG_ATTR_NONCONTIG) {
		uint64_t shm_ref = READ_ONCE(tmem->shm_ref);

		mem->mobj = msg_param_mobj_from_noncontig(pa, sz, shm_ref,
							  false);
		if (!mem->mobj)
			return TEE_ERROR_BAD_PARAMETERS;
		mem->offs = 0;
		mem->size = sz;
		return TEE_SUCCESS;
	}

	/* Belongs to nonsecure shared memory? */
	if (param_mem_from_mobj(mem, shm_mobj, pa, sz))
		return TEE_SUCCESS;

#ifdef CFG_SECURE_DATA_PATH
	/* Belongs to SDP memories? */
	for (mobj = sdp_mem_mobjs; *mobj; mobj++)
		if (param_mem_from_mobj(mem, *mobj, pa, sz))
			return TEE_SUCCESS;
#endif

	return TEE_ERROR_BAD_PARAMETERS;
}
