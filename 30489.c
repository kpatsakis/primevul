_dbus_atomic_get (DBusAtomic *atomic)
{
#if DBUS_USE_SYNC
  __sync_synchronize ();
  return atomic->value;
#else
  dbus_int32_t res;

  _DBUS_LOCK (atomic);
  res = atomic->value;
  _DBUS_UNLOCK (atomic);
  return res;
#endif
}
