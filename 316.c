void AddProductSpecificWorkItems(const InstallationState& original_state,
                                 const InstallerState& installer_state,
                                 const FilePath& setup_path,
                                 const Version& new_version,
                                 WorkItemList* list) {
  const Products& products = installer_state.products();
  for (size_t i = 0; i < products.size(); ++i) {
    const Product& p = *products[i];
    if (p.is_chrome_frame()) {
      AddChromeFrameWorkItems(original_state, installer_state, setup_path,
                              new_version, p, list);
    }
  }
}
