device_constructor (GType type,
                    guint n_construct_properties,
                    GObjectConstructParam *construct_properties)
{
  Device *device;
  DeviceClass *klass;

  klass = DEVICE_CLASS (g_type_class_peek (TYPE_DEVICE));

  device = DEVICE (G_OBJECT_CLASS (device_parent_class)->constructor (type,
                                                                      n_construct_properties,
                                                                      construct_properties));
  return G_OBJECT (device);
}
