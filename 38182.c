static ssize_t map_size_show(struct uio_mem *mem, char *buf)
{
	return sprintf(buf, "0x%lx\n", mem->size);
}
