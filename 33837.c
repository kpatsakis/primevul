ssh_set_newkeys(struct ssh *ssh, int mode)
{
	struct session_state *state = ssh->state;
	struct sshenc *enc;
	struct sshmac *mac;
	struct sshcomp *comp;
	struct sshcipher_ctx *cc;
	u_int64_t *max_blocks;
	const char *wmsg;
	int r, crypt_type;

	debug2("set_newkeys: mode %d", mode);

	if (mode == MODE_OUT) {
		cc = &state->send_context;
		crypt_type = CIPHER_ENCRYPT;
		state->p_send.packets = state->p_send.blocks = 0;
		max_blocks = &state->max_blocks_out;
	} else {
		cc = &state->receive_context;
		crypt_type = CIPHER_DECRYPT;
		state->p_read.packets = state->p_read.blocks = 0;
		max_blocks = &state->max_blocks_in;
	}
	if (state->newkeys[mode] != NULL) {
		debug("set_newkeys: rekeying");
		if ((r = cipher_cleanup(cc)) != 0)
			return r;
		enc  = &state->newkeys[mode]->enc;
		mac  = &state->newkeys[mode]->mac;
		comp = &state->newkeys[mode]->comp;
		mac_clear(mac);
		explicit_bzero(enc->iv,  enc->iv_len);
		explicit_bzero(enc->key, enc->key_len);
		explicit_bzero(mac->key, mac->key_len);
		free(enc->name);
		free(enc->iv);
		free(enc->key);
		free(mac->name);
		free(mac->key);
		free(comp->name);
		free(state->newkeys[mode]);
	}
	/* move newkeys from kex to state */
	if ((state->newkeys[mode] = ssh->kex->newkeys[mode]) == NULL)
		return SSH_ERR_INTERNAL_ERROR;
	ssh->kex->newkeys[mode] = NULL;
	enc  = &state->newkeys[mode]->enc;
	mac  = &state->newkeys[mode]->mac;
	comp = &state->newkeys[mode]->comp;
	if (cipher_authlen(enc->cipher) == 0) {
		if ((r = mac_init(mac)) != 0)
			return r;
	}
	mac->enabled = 1;
	DBG(debug("cipher_init_context: %d", mode));
	if ((r = cipher_init(cc, enc->cipher, enc->key, enc->key_len,
	    enc->iv, enc->iv_len, crypt_type)) != 0)
		return r;
	if (!state->cipher_warning_done &&
	    (wmsg = cipher_warning_message(cc)) != NULL) {
		error("Warning: %s", wmsg);
		state->cipher_warning_done = 1;
	}
	/* Deleting the keys does not gain extra security */
	/* explicit_bzero(enc->iv,  enc->block_size);
	   explicit_bzero(enc->key, enc->key_len);
	   explicit_bzero(mac->key, mac->key_len); */
	if ((comp->type == COMP_ZLIB ||
	    (comp->type == COMP_DELAYED &&
	     state->after_authentication)) && comp->enabled == 0) {
		if ((r = ssh_packet_init_compression(ssh)) < 0)
			return r;
		if (mode == MODE_OUT) {
			if ((r = start_compression_out(ssh, 6)) != 0)
				return r;
		} else {
			if ((r = start_compression_in(ssh)) != 0)
				return r;
		}
		comp->enabled = 1;
	}
	/*
	 * The 2^(blocksize*2) limit is too expensive for 3DES,
	 * blowfish, etc, so enforce a 1GB limit for small blocksizes.
	 */
	if (enc->block_size >= 16)
		*max_blocks = (u_int64_t)1 << (enc->block_size*2);
	else
		*max_blocks = ((u_int64_t)1 << 30) / enc->block_size;
	if (state->rekey_limit)
		*max_blocks = MIN(*max_blocks,
		    state->rekey_limit / enc->block_size);
	return 0;
}
