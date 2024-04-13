int init_buffer()
{
	pool2_buffer = create_pool("buffer", sizeof (struct buffer) + global.tune.bufsize, MEM_F_SHARED);
	return pool2_buffer != NULL;
}
