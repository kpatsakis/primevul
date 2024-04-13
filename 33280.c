is_read_comp_null (gnutls_session_t session)
{
  record_parameters_st *record_params;

  _gnutls_epoch_get (session, EPOCH_READ_CURRENT, &record_params);
  if (record_params->compression_algorithm == GNUTLS_COMP_NULL)
    return 0;

  return 1;
}
