addWildCards(struct ubik_trans *tt, prlist *alist, afs_uint32 host)
{
    afs_int32 temp;
    struct prentry tentry;
    prlist wlist;
    unsigned wild = htonl(0xffffff00);
    struct in_addr iaddr;
    afs_int32 hostid;
    int size = 0, i, code;
    int added = 0;
    char hoststr[16];

    while ((host = (host & wild))) {
	wild = htonl(ntohl(wild) << 8);
	iaddr.s_addr = host;
	code = NameToID(tt, afs_inet_ntoa_r(iaddr.s_addr, hoststr), &hostid);
	if (code == PRSUCCESS && hostid != 0) {
	    temp = FindByID(tt, hostid);
	    if (temp) {
		code = pr_ReadEntry(tt, 0, temp, &tentry);
		if (code != PRSUCCESS)
		    continue;
	    } else
		continue;
	} else
	    continue;
	wlist.prlist_len = 0;
	wlist.prlist_val = NULL;

	code = GetList(tt, &tentry, &wlist, 0);
	if (code)
	    return code;
	added += wlist.prlist_len;
	for (i = 0; i < wlist.prlist_len; i++) {
	    if (!inCPS(*alist, wlist.prlist_val[i]))
		if ((code = AddToPRList(alist, &size, wlist.prlist_val[i]))) {
		    free(wlist.prlist_val);
		    return (code);
		}
	}
	if (wlist.prlist_val)
	    free(wlist.prlist_val);
    }
    if (added)
	qsort(alist->prlist_val, alist->prlist_len, sizeof(afs_int32), IDCmp);
    return 0;
}
