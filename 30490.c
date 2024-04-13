_dbus_atomic_inc (DBusAtomic *atomic)
{
#if DBUS_USE_SYNC
  return __sync_add_and_fetch(&atomic->value, 1)-1;
#else
  dbus_int32_t res;
  _DBUS_LOCK (atomic);
  res = atomic->value;
  atomic->value += 1;
  _DBUS_UNLOCK (atomic);
  return res;
#endif
}
