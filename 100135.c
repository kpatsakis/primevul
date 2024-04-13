static int cx24116_tune(struct dvb_frontend *fe, bool re_tune,
	unsigned int mode_flags, unsigned int *delay, fe_status_t *status)
{
	/*
	 * It is safe to discard "params" here, as the DVB core will sync
	 * fe->dtv_property_cache with fepriv->parameters_in, where the
	 * DVBv3 params are stored. The only practical usage for it indicate
	 * that re-tuning is needed, e. g. (fepriv->state & FESTATE_RETUNE) is
	 * true.
	 */

	*delay = HZ / 5;
	if (re_tune) {
		int ret = cx24116_set_frontend(fe);
		if (ret)
			return ret;
	}
	return cx24116_read_status(fe, status);
}
