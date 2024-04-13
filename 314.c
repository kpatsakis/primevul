void AddGoogleUpdateWorkItems(const InstallationState& original_state,
                              const InstallerState& installer_state,
                              WorkItemList* install_list) {
  if (installer_state.operation() != InstallerState::MULTI_INSTALL &&
      installer_state.operation() != InstallerState::MULTI_UPDATE) {
    VLOG(1) << "AddGoogleUpdateWorkItems noop: " << installer_state.operation();
    return;
  }

  if (installer_state.state_type() != BrowserDistribution::CHROME_BINARIES) {
    std::wstring multi_key(
        installer_state.multi_package_binaries_distribution()->GetStateKey());
    const ProductState* chrome_product_state =
        original_state.GetNonVersionedProductState(
            installer_state.system_install(),
            BrowserDistribution::CHROME_BROWSER);

    const std::wstring& brand(chrome_product_state->brand());
    if (!brand.empty()) {
      install_list->AddCreateRegKeyWorkItem(installer_state.root_key(),
                                            multi_key);
      install_list->AddSetRegValueWorkItem(installer_state.root_key(),
                                           multi_key,
                                           google_update::kRegBrandField,
                                           brand,
                                           false);
    }
  }

  AddUsageStatsWorkItems(original_state, installer_state, install_list);

}
