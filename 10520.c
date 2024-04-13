static int process_info_rsp(struct rtrs_clt_path *clt_path,
			    const struct rtrs_msg_info_rsp *msg)
{
	unsigned int sg_cnt, total_len;
	int i, sgi;

	sg_cnt = le16_to_cpu(msg->sg_cnt);
	if (!sg_cnt || (clt_path->queue_depth % sg_cnt)) {
		rtrs_err(clt_path->clt,
			  "Incorrect sg_cnt %d, is not multiple\n",
			  sg_cnt);
		return -EINVAL;
	}

	/*
	 * Check if IB immediate data size is enough to hold the mem_id and
	 * the offset inside the memory chunk.
	 */
	if ((ilog2(sg_cnt - 1) + 1) + (ilog2(clt_path->chunk_size - 1) + 1) >
	    MAX_IMM_PAYL_BITS) {
		rtrs_err(clt_path->clt,
			  "RDMA immediate size (%db) not enough to encode %d buffers of size %dB\n",
			  MAX_IMM_PAYL_BITS, sg_cnt, clt_path->chunk_size);
		return -EINVAL;
	}
	total_len = 0;
	for (sgi = 0, i = 0; sgi < sg_cnt && i < clt_path->queue_depth; sgi++) {
		const struct rtrs_sg_desc *desc = &msg->desc[sgi];
		u32 len, rkey;
		u64 addr;

		addr = le64_to_cpu(desc->addr);
		rkey = le32_to_cpu(desc->key);
		len  = le32_to_cpu(desc->len);

		total_len += len;

		if (!len || (len % clt_path->chunk_size)) {
			rtrs_err(clt_path->clt, "Incorrect [%d].len %d\n",
				  sgi,
				  len);
			return -EINVAL;
		}
		for ( ; len && i < clt_path->queue_depth; i++) {
			clt_path->rbufs[i].addr = addr;
			clt_path->rbufs[i].rkey = rkey;

			len  -= clt_path->chunk_size;
			addr += clt_path->chunk_size;
		}
	}
	/* Sanity check */
	if (sgi != sg_cnt || i != clt_path->queue_depth) {
		rtrs_err(clt_path->clt,
			 "Incorrect sg vector, not fully mapped\n");
		return -EINVAL;
	}
	if (total_len != clt_path->chunk_size * clt_path->queue_depth) {
		rtrs_err(clt_path->clt, "Incorrect total_len %d\n", total_len);
		return -EINVAL;
	}

	return 0;
}