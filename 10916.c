set_sg(union DateData *x, double sg)
{
    if (simple_dat_p(x)) {
	get_s_jd(x);
	clear_civil(x);
	x->s.sg = (date_sg_t)sg;
    } else {
	get_c_jd(x);
	get_c_df(x);
	clear_civil(x);
	x->c.sg = (date_sg_t)sg;
    }
}