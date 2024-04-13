static int sample_conv_http_date(const struct arg *args, struct sample *smp)
{
	const char day[7][4] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
	const char mon[12][4] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	struct chunk *temp;
	struct tm *tm;
	time_t curr_date = smp->data.uint;

	/* add offset */
	if (args && (args[0].type == ARGT_SINT || args[0].type == ARGT_UINT))
		curr_date += args[0].data.sint;

	tm = gmtime(&curr_date);

	temp = get_trash_chunk();
	temp->len = snprintf(temp->str, temp->size - temp->len,
			     "%s, %02d %s %04d %02d:%02d:%02d GMT",
			     day[tm->tm_wday], tm->tm_mday, mon[tm->tm_mon], 1900+tm->tm_year,
			     tm->tm_hour, tm->tm_min, tm->tm_sec);

	smp->data.str = *temp;
	smp->type = SMP_T_STR;
	return 1;
}
