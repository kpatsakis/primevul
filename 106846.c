sort_snapshot(TxidSnapshot *snap)
{
	if (snap->nxip > 1)
		qsort(snap->xip, snap->nxip, sizeof(txid), cmp_txid);
}
