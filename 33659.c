WhoIsThisWithName(struct rx_call *acall, struct ubik_trans *at, afs_int32 *aid,
		  char *aname)
{
    /* aid is set to the identity of the caller, if known, else ANONYMOUSID */
    /* returns -1 and sets aid to ANONYMOUSID on any failure */
    struct rx_connection *tconn;
    afs_int32 code;
    char tcell[MAXKTCREALMLEN];
    char name[MAXKTCNAMELEN];
    char inst[MAXKTCNAMELEN];
    int ilen;
    char vname[256];

    *aid = ANONYMOUSID;
    tconn = rx_ConnectionOf(acall);
    code = rx_SecurityClassOf(tconn);
    if (code == 0)
	return 0;
    else if (code == 1) {	/* vab class */
	goto done;		/* no longer supported */
    } else if (code == 2) {	/* kad class */

	int clen;

	if ((code = rxkad_GetServerInfo(acall->conn, NULL, 0 /*was &exp */ ,
					name, inst, tcell, NULL)))
	    goto done;


	strncpy(vname, name, sizeof(vname));
	if ((ilen = strlen(inst))) {
	    if (strlen(vname) + 1 + ilen >= sizeof(vname))
		goto done;
	    strcat(vname, ".");
	    strcat(vname, inst);
	}
	if ((clen = strlen(tcell))) {
	    int foreign = afs_is_foreign_ticket_name(name,inst,tcell,pr_realmName);

	    if (foreign) {
		if (strlen(vname) + 1 + clen >= sizeof(vname))
		    goto done;
		strcat(vname, "@");
		strcat(vname, tcell);
		lcstring(vname, vname, sizeof(vname));
		code = NameToID(at, vname, aid);
		strcpy(aname, vname);
		return 2;
	    }
	}

	if (strcmp(AUTH_SUPERUSER, vname) == 0)
	    *aid = SYSADMINID;	/* special case for the fileserver */
	else {
	    lcstring(vname, vname, sizeof(vname));
	    code = NameToID(at, vname, aid);
	}
    }
  done:
    if (code && !pr_noAuth)
	return -1;
    return 0;
}
