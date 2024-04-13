circle_copy(CIRCLE *circle)
{
	CIRCLE	   *result;

	if (!PointerIsValid(circle))
		return NULL;

	result = (CIRCLE *) palloc(sizeof(CIRCLE));
	memcpy((char *) result, (char *) circle, sizeof(CIRCLE));
	return result;
}
