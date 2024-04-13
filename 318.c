void AddRegisterComDllWorkItemsForPackage(const InstallerState& installer_state,
                                          const Version* old_version,
                                          const Version& new_version,
                                          WorkItemList* work_item_list) {
  std::vector<FilePath> com_dll_list;
  installer_state.AddComDllList(&com_dll_list);

  if (!com_dll_list.empty()) {
    if (old_version) {
      FilePath old_dll_path(installer_state.target_path().AppendASCII(
          old_version->GetString()));

      installer::AddRegisterComDllWorkItems(old_dll_path,
                                            com_dll_list,
                                            installer_state.system_install(),
                                            false,  // Unregister
                                            true,   // May fail
                                            work_item_list);
    }

    FilePath dll_path(installer_state.target_path().AppendASCII(
        new_version.GetString()));
    installer::AddRegisterComDllWorkItems(dll_path,
                                          com_dll_list,
                                          installer_state.system_install(),
                                          true,   // Register
                                          false,  // Must succeed.
                                          work_item_list);
  }
}
