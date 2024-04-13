identity_is_root_user (PolkitIdentity *user)
{
  if (!POLKIT_IS_UNIX_USER (user))
    return FALSE;
  return polkit_unix_user_get_uid (POLKIT_UNIX_USER (user)) == 0;
}
