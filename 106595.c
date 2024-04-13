box_copy(BOX *box)
{
	BOX		   *result = (BOX *) palloc(sizeof(BOX));

	memcpy((char *) result, (char *) box, sizeof(BOX));

	return result;
}
