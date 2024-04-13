cmp_txid(const void *aa, const void *bb)
{
	txid		a = *(const txid *) aa;
	txid		b = *(const txid *) bb;

	if (a < b)
		return -1;
	if (a > b)
		return 1;
	return 0;
}
