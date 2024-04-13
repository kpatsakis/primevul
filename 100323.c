void MenuGtk::OnMenuItemActivated(GtkMenuItem* menuitem, MenuGtk* menu) {
  if (block_activation_)
    return;

  if (gtk_menu_item_get_submenu(menuitem))
    return;

  const MenuCreateMaterial* data =
      reinterpret_cast<const MenuCreateMaterial*>(
          g_object_get_data(G_OBJECT(menuitem), "menu-data"));

  int id;
  if (data) {
    id = data->id;
  } else {
    id = reinterpret_cast<intptr_t>(g_object_get_data(G_OBJECT(menuitem),
                                                      "menu-id"));
  }

  if (menu->delegate_->IsCommandEnabled(id))
    menu->delegate_->ExecuteCommand(id);
}
