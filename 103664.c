static int _rollback_transactions(zval *el)
{
	PGconn *link;
	PGresult *res;
	int orig;
	zend_resource *rsrc = Z_RES_P(el);

	if (rsrc->type != le_plink) 
		return 0;

	link = (PGconn *) rsrc->ptr;

	if (PQ_SETNONBLOCKING(link, 0)) {
		php_error_docref("ref.pgsql", E_NOTICE, "Cannot set connection to blocking mode");
		return -1;
	}
	
	while ((res = PQgetResult(link))) {
		PQclear(res);
	}
#if HAVE_PGTRANSACTIONSTATUS && HAVE_PQPROTOCOLVERSION
	if ((PQprotocolVersion(link) >= 3 && PQtransactionStatus(link) != PQTRANS_IDLE) || PQprotocolVersion(link) < 3)
#endif
	{
		orig = PGG(ignore_notices);
		PGG(ignore_notices) = 1;
#if HAVE_PGTRANSACTIONSTATUS && HAVE_PQPROTOCOLVERSION
		res = PQexec(link,"ROLLBACK;");
#else
		res = PQexec(link,"BEGIN;");
		PQclear(res);
		res = PQexec(link,"ROLLBACK;");
#endif
		PQclear(res);
		PGG(ignore_notices) = orig;
	}

	return 0;
}
