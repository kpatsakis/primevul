_dbus_atomic_get (DBusAtomic *atomic)
{
  /* this is what GLib does, hopefully it's right... */
  MemoryBarrier ();
  return atomic->value;
}
