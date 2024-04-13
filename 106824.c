tsquerysend(PG_FUNCTION_ARGS)
{
	TSQuery		query = PG_GETARG_TSQUERY(0);
	StringInfoData buf;
	int			i;
	QueryItem  *item = GETQUERY(query);

	pq_begintypsend(&buf);

	pq_sendint(&buf, query->size, sizeof(uint32));
	for (i = 0; i < query->size; i++)
	{
		pq_sendint(&buf, item->type, sizeof(item->type));

		switch (item->type)
		{
			case QI_VAL:
				pq_sendint(&buf, item->qoperand.weight, sizeof(uint8));
				pq_sendint(&buf, item->qoperand.prefix, sizeof(uint8));
				pq_sendstring(&buf, GETOPERAND(query) + item->qoperand.distance);
				break;
			case QI_OPR:
				pq_sendint(&buf, item->qoperator.oper, sizeof(item->qoperator.oper));
				break;
			default:
				elog(ERROR, "unrecognized tsquery node type: %d", item->type);
		}
		item++;
	}

	PG_FREE_IF_COPY(query, 0);

	PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}
