MenuGtk::MenuGtk(MenuGtk::Delegate* delegate,
                 const MenuCreateMaterial* menu_data,
                 GtkAccelGroup* accel_group)
    : delegate_(delegate),
      dummy_accel_group_(gtk_accel_group_new()),
      menu_(gtk_menu_new()),
      factory_(this) {
  ConnectSignalHandlers();
  BuildMenuIn(menu_.get(), menu_data, accel_group);
}
