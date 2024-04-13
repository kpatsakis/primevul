poly_center(PG_FUNCTION_ARGS)
{
	POLYGON    *poly = PG_GETARG_POLYGON_P(0);
	Datum		result;
	CIRCLE	   *circle;

	circle = DatumGetCircleP(DirectFunctionCall1(poly_circle,
												 PolygonPGetDatum(poly)));
	result = DirectFunctionCall1(circle_center,
								 CirclePGetDatum(circle));

	PG_RETURN_DATUM(result);
}
