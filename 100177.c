print_t_info(netdissect_options *ndo,
             int info)
{
	int ntype = info & 3;
	switch (ntype) {
	case 0: ND_PRINT((ndo, "reserved-ntype? ")); break;
	case TI_L2ROUT: ND_PRINT((ndo, "l2rout ")); break;
	case TI_L1ROUT: ND_PRINT((ndo, "l1rout ")); break;
	case TI_ENDNODE: ND_PRINT((ndo, "endnode ")); break;
	}
	if (info & TI_VERIF)
	    ND_PRINT((ndo, "verif "));
	if (info & TI_BLOCK)
	    ND_PRINT((ndo, "blo "));
}
