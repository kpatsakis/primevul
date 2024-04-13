m_canonicalize_jd(VALUE obj, union DateData *x)
{
    if (simple_dat_p(x)) {
	get_s_jd(x);
	canonicalize_s_jd(obj, x);
    }
    else {
	get_c_jd(x);
	canonicalize_c_jd(obj, x);
    }
}