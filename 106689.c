line_decode(const char *str, LINE *line)
{
	char	   *tail;

	while (isspace((unsigned char) *str))
		str++;
	if (*str++ != '{')
		return false;
	line->A = strtod(str, &tail);
	if (tail <= str)
		return false;
	str = tail;
	while (isspace((unsigned char) *str))
		str++;
	if (*str++ != DELIM)
		return false;
	line->B = strtod(str, &tail);
	if (tail <= str)
		return false;
	str = tail;
	while (isspace((unsigned char) *str))
		str++;
	if (*str++ != DELIM)
		return false;
	line->C = strtod(str, &tail);
	if (tail <= str)
		return false;
	str = tail;
	while (isspace((unsigned char) *str))
		str++;
	if (*str++ != '}')
		return false;
	while (isspace((unsigned char) *str))
		str++;
	if (*str)
		return false;

	return true;
}
