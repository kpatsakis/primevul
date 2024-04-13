m_proleptic_julian_p(union DateData *x)
{
    double sg;

    sg = m_sg(x);
    if (isinf(sg) && sg > 0)
	return 1;
    return 0;
}