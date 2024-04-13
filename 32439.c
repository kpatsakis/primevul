gettime1900d(void)
{
	struct timeval tv;
	gettimeofday(&tv, NULL); /* never fails */
	G.cur_time = tv.tv_sec + (1.0e-6 * tv.tv_usec) + OFFSET_1900_1970;
	return G.cur_time;
}
