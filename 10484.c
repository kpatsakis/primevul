struct rtrs_permit *rtrs_clt_get_permit(struct rtrs_clt_sess *clt,
					  enum rtrs_clt_con_type con_type,
					  enum wait_type can_wait)
{
	struct rtrs_permit *permit;
	DEFINE_WAIT(wait);

	permit = __rtrs_get_permit(clt, con_type);
	if (permit || !can_wait)
		return permit;

	do {
		prepare_to_wait(&clt->permits_wait, &wait,
				TASK_UNINTERRUPTIBLE);
		permit = __rtrs_get_permit(clt, con_type);
		if (permit)
			break;

		io_schedule();
	} while (1);

	finish_wait(&clt->permits_wait, &wait);

	return permit;
}