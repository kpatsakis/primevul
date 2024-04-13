inCPS(prlist CPS, afs_int32 id)
{
    int i;

    for (i = (CPS.prlist_len - 1); i >= 0; i--) {
	if (CPS.prlist_val[i] == id)
	    return (1);
    }
    return (0);
}
