void AddUsageStatsWorkItems(const InstallationState& original_state,
                            const InstallerState& installer_state,
                            WorkItemList* install_list) {
  DCHECK(installer_state.operation() == InstallerState::MULTI_INSTALL ||
         installer_state.operation() == InstallerState::MULTI_UPDATE);

  HKEY root_key = installer_state.root_key();
  bool value_found = false;
  DWORD usagestats = 0;
  const Products& products = installer_state.products();

  for (Products::const_iterator scan = products.begin(), end = products.end();
       !value_found && scan != end; ++scan) {
    BrowserDistribution* dist = (*scan)->distribution();
    const ProductState* product_state =
        original_state.GetNonVersionedProductState(
            installer_state.system_install(), dist->GetType());
    value_found = product_state->GetUsageStats(&usagestats);
  }

  if (value_found) {
    std::wstring state_key(
        installer_state.multi_package_binaries_distribution()->GetStateKey());
    install_list->AddCreateRegKeyWorkItem(root_key, state_key);
    install_list->AddSetRegValueWorkItem(root_key, state_key,
                                         google_update::kRegUsageStatsField,
                                         usagestats, false);

    for (Products::const_iterator scan = products.begin(), end = products.end();
         scan != end; ++scan) {
      BrowserDistribution* dist = (*scan)->distribution();
      if (installer_state.system_install()) {
        install_list->AddDeleteRegValueWorkItem(
            root_key, dist->GetStateMediumKey(),
            google_update::kRegUsageStatsField);
        install_list->AddDeleteRegValueWorkItem(
            HKEY_CURRENT_USER, dist->GetStateKey(),
            google_update::kRegUsageStatsField);
      }
      install_list->AddDeleteRegValueWorkItem(root_key, dist->GetStateKey(),
          google_update::kRegUsageStatsField);
    }
  }
}
