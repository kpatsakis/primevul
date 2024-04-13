static int sample_conv_http_date(const struct arg *args, struct sample *smp, void *private)
{
	const char day[7][4] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
	const char mon[12][4] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	struct chunk *temp;
	struct tm *tm;
	/* With high numbers, the date returned can be negative, the 55 bits mask prevent this. */
	time_t curr_date = smp->data.u.sint & 0x007fffffffffffffLL;

	/* add offset */
	if (args && (args[0].type == ARGT_SINT))
		curr_date += args[0].data.sint;

	tm = gmtime(&curr_date);
	if (!tm)
		return 0;

	temp = get_trash_chunk();
	temp->len = snprintf(temp->str, temp->size - temp->len,
			     "%s, %02d %s %04d %02d:%02d:%02d GMT",
			     day[tm->tm_wday], tm->tm_mday, mon[tm->tm_mon], 1900+tm->tm_year,
			     tm->tm_hour, tm->tm_min, tm->tm_sec);

	smp->data.u.str = *temp;
	smp->data.type = SMP_T_STR;
	return 1;
}
