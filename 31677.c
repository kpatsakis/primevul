smp_fetch_param(char delim, const char *name, int name_len, const struct arg *args, struct sample *smp, const char *kw, void *private)
{
	const char *vstart, *vend;
	struct chunk *temp;
	const char **chunks = (const char **)smp->ctx.a;

	if (!find_next_url_param(chunks,
	                         name, name_len,
	                         &vstart, &vend,
	                         delim))
		return 0;

	/* Create sample. If the value is contiguous, return the pointer as CONST,
	 * if the value is wrapped, copy-it in a buffer.
	 */
	smp->data.type = SMP_T_STR;
	if (chunks[2] &&
	    vstart >= chunks[0] && vstart <= chunks[1] &&
	    vend >= chunks[2] && vend <= chunks[3]) {
		/* Wrapped case. */
		temp = get_trash_chunk();
		memcpy(temp->str, vstart, chunks[1] - vstart);
		memcpy(temp->str + ( chunks[1] - vstart ), chunks[2], vend - chunks[2]);
		smp->data.u.str.str = temp->str;
		smp->data.u.str.len = ( chunks[1] - vstart ) + ( vend - chunks[2] );
	} else {
		/* Contiguous case. */
		smp->data.u.str.str = (char *)vstart;
		smp->data.u.str.len = vend - vstart;
		smp->flags = SMP_F_VOL_1ST | SMP_F_CONST;
	}

	/* Update context, check wrapping. */
	chunks[0] = vend;
	if (chunks[2] && vend >= chunks[2] && vend <= chunks[3]) {
		chunks[1] = chunks[3];
		chunks[2] = NULL;
	}

	if (chunks[0] < chunks[1])
		smp->flags |= SMP_F_NOT_LAST;

	return 1;
}
