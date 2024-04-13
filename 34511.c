polkit_backend_interactive_authority_check_authorization_finish (PolkitBackendAuthority  *authority,
                                                                 GAsyncResult            *res,
                                                                 GError                 **error)
{
  GSimpleAsyncResult *simple;
  PolkitAuthorizationResult *result;

  simple = G_SIMPLE_ASYNC_RESULT (res);

  g_warn_if_fail (g_simple_async_result_get_source_tag (simple) == polkit_backend_interactive_authority_check_authorization);

  result = NULL;

  if (g_simple_async_result_propagate_error (simple, error))
    goto out;

  result = g_object_ref (g_simple_async_result_get_op_res_gpointer (simple));

 out:
  return result;
}
