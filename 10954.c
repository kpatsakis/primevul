mk_inspect(union DateData *x, VALUE klass, VALUE to_s)
{
    return rb_enc_sprintf(rb_usascii_encoding(),
			  "#<%"PRIsVALUE": %"PRIsVALUE" "
			  "((%+"PRIsVALUE"j,%ds,%+"PRIsVALUE"n),%+ds,%.0fj)>",
			  klass, to_s,
			  m_real_jd(x), m_df(x), m_sf(x),
			  m_of(x), m_sg(x));
}