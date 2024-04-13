static inline cputime_t cputime_div_non_zero(cputime_t time, unsigned long div)
{
	cputime_t res = cputime_div(time, div);

	return max_t(cputime_t, res, 1);
}
