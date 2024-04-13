_dbus_get_is_errno_eagain_or_ewouldblock (void)
{
  return errno == EAGAIN || errno == EWOULDBLOCK;
}
