static int cap_compare(cap_t acap, cap_t bcap)
{
    int rc = 0;
    size_t asize = cap_size(acap);
    size_t bsize = cap_size(bcap);

    if (asize != bsize) {
	rc = 1;
    } else {
	char *abuf = xcalloc(asize, sizeof(*abuf));
	char *bbuf = xcalloc(bsize, sizeof(*bbuf));
	cap_copy_ext(abuf, acap, asize);
	cap_copy_ext(bbuf, bcap, bsize);
	rc = memcmp(abuf, bbuf, asize);
	free(abuf);
	free(bbuf);
    }
    return rc;
}
