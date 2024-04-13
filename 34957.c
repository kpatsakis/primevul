file_reader(void *cls, uint64_t pos, char *buf, size_t max)
#else
static int
file_reader(void *cls, uint64_t pos, char *buf, int max)
#endif
{
	FILE *file = cls;

	fseeko(file, pos, SEEK_SET);
	return fread(buf, 1, max, file);
}
