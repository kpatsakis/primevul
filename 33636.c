CreateOK(struct ubik_trans *ut, afs_int32 cid, afs_int32 oid, afs_int32 flag,
	 int admin)
{
    if (restricted && !admin)
	return 0;

    if (flag & PRFOREIGN) {
	/* Foreign users are recognized by the '@' sign and
	 * not by the PRFOREIGN flag.
	 */
	return 0;
    } else if (flag & PRGRP) {
	/* Allow anonymous group creation only if owner specified
	 * and running noAuth.
	 */
	if (cid == ANONYMOUSID) {
	    if ((oid == 0) || !pr_noAuth)
		return 0;
	}
    } else {			/* creating a user */
	if (!admin && !pr_noAuth)
	    return 0;
    }
    return 1;			/* OK! */
}
