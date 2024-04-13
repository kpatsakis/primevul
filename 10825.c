m_cwday(union DateData *x)
{
    int w;

    w = m_wday(x);
    if (w == 0)
	w = 7;
    return w;
}