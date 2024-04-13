_dbus_atomic_inc (DBusAtomic *atomic)
{
  return InterlockedIncrement (&atomic->value) - 1;
}
