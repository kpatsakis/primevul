static int audit_log_single_execve_arg(struct audit_context *context,
static void audit_log_execve_info(struct audit_context *context,
				  struct audit_buffer **ab)
 {
	long len_max;
	long len_rem;
	long len_full;
	long len_buf;
	long len_abuf;
	long len_tmp;
	bool require_data;
	bool encode;
	unsigned int iter;
	unsigned int arg;
	char *buf_head;
	char *buf;
	const char __user *p = (const char __user *)current->mm->arg_start;

	/* NOTE: this buffer needs to be large enough to hold all the non-arg
	 *       data we put in the audit record for this argument (see the
	 *       code below) ... at this point in time 96 is plenty */
	char abuf[96];

	/* NOTE: we set MAX_EXECVE_AUDIT_LEN to a rather arbitrary limit, the
	 *       current value of 7500 is not as important as the fact that it
	 *       is less than 8k, a setting of 7500 gives us plenty of wiggle
	 *       room if we go over a little bit in the logging below */
	WARN_ON_ONCE(MAX_EXECVE_AUDIT_LEN > 7500);
	len_max = MAX_EXECVE_AUDIT_LEN;

	/* scratch buffer to hold the userspace args */
	buf_head = kmalloc(MAX_EXECVE_AUDIT_LEN + 1, GFP_KERNEL);
	if (!buf_head) {
		audit_panic("out of memory for argv string");
		return;
 	}
	buf = buf_head;
 
	audit_log_format(*ab, "argc=%d", context->execve.argc);

	len_rem = len_max;
	len_buf = 0;
	len_full = 0;
	require_data = true;
	encode = false;
	iter = 0;
	arg = 0;
 	do {
		/* NOTE: we don't ever want to trust this value for anything
		 *       serious, but the audit record format insists we
		 *       provide an argument length for really long arguments,
		 *       e.g. > MAX_EXECVE_AUDIT_LEN, so we have no choice but
		 *       to use strncpy_from_user() to obtain this value for
		 *       recording in the log, although we don't use it
		 *       anywhere here to avoid a double-fetch problem */
		if (len_full == 0)
			len_full = strnlen_user(p, MAX_ARG_STRLEN) - 1;

		/* read more data from userspace */
		if (require_data) {
			/* can we make more room in the buffer? */
			if (buf != buf_head) {
				memmove(buf_head, buf, len_buf);
				buf = buf_head;
			}

			/* fetch as much as we can of the argument */
			len_tmp = strncpy_from_user(&buf_head[len_buf], p,
						    len_max - len_buf);
			if (len_tmp == -EFAULT) {
				/* unable to copy from userspace */
				send_sig(SIGKILL, current, 0);
				goto out;
			} else if (len_tmp == (len_max - len_buf)) {
				/* buffer is not large enough */
				require_data = true;
				/* NOTE: if we are going to span multiple
				 *       buffers force the encoding so we stand
				 *       a chance at a sane len_full value and
				 *       consistent record encoding */
				encode = true;
				len_full = len_full * 2;
				p += len_tmp;
			} else {
				require_data = false;
				if (!encode)
					encode = audit_string_contains_control(
								buf, len_tmp);
				/* try to use a trusted value for len_full */
				if (len_full < len_max)
					len_full = (encode ?
						    len_tmp * 2 : len_tmp);
				p += len_tmp + 1;
			}
			len_buf += len_tmp;
			buf_head[len_buf] = '\0';
 
			/* length of the buffer in the audit record? */
			len_abuf = (encode ? len_buf * 2 : len_buf + 2);
 		}
