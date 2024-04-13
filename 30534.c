_dbus_atomic_dec (DBusAtomic *atomic)
{
  return InterlockedDecrement (&atomic->value) + 1;
}
