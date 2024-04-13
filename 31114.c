decode_num (uintmax_t *num, char const *arg, uintmax_t maxval)
{
  uintmax_t u;
  char *arg_lim;

  if (!ISDIGIT (*arg))
    return false;

  errno = 0;
  u = strtoumax (arg, &arg_lim, 10);

  if (! (u <= maxval && errno != ERANGE) || *arg_lim)
    return false;

  *num = u;
  return true;
}
