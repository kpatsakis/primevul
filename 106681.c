has_interpt_sl(LSEG *lseg, LINE *line)
{
	Point	   *tmp;

	tmp = interpt_sl(lseg, line);
	if (tmp)
		return true;
	return false;
}
