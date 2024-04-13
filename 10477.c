struct rtrs_clt_con *rtrs_permit_to_clt_con(struct rtrs_clt_path *clt_path,
					    struct rtrs_permit *permit)
{
	int id = 0;

	if (permit->con_type == RTRS_IO_CON)
		id = (permit->cpu_id % (clt_path->s.irq_con_num - 1)) + 1;

	return to_clt_con(clt_path->s.con[id]);
}