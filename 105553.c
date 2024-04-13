luks_get_uid_from_dm_name (const char *dm_name,
                           uid_t *out_uid)
{
  int n;
  gboolean ret;
  uid_t uid;
  char *endp;

  ret = FALSE;

  if (!g_str_has_prefix (dm_name, "udisks-luks-uuid"))
    goto out;

  /* determine who unlocked the device */
  for (n = strlen (dm_name) - 1; n >= 0; n--)
    {
      if (dm_name[n] == '-')
        break;
    }
  if (strncmp (dm_name + n, "-uid", 4) != 0)
    goto out;

  uid = strtol (dm_name + n + 4, &endp, 10);
  if (endp == NULL || *endp != '\0')
    goto out;

  if (out_uid != NULL)
    *out_uid = uid;

  ret = TRUE;
 out:
  return ret;
}
