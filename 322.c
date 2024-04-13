void AddVersionKeyWorkItems(HKEY root,
                            BrowserDistribution* dist,
                            const Version& new_version,
                            WorkItemList* list) {
  std::wstring version_key(dist->GetVersionKey());
  list->AddCreateRegKeyWorkItem(root, version_key);

  std::wstring product_name(dist->GetAppShortCutName());
  list->AddSetRegValueWorkItem(root, version_key, google_update::kRegNameField,
                               product_name, true);  // overwrite name also
  list->AddSetRegValueWorkItem(root, version_key,
                               google_update::kRegOopcrashesField,
                               static_cast<DWORD>(1),
                               false);  // set during first install
  list->AddSetRegValueWorkItem(root, version_key,
                               google_update::kRegVersionField,
                               UTF8ToWide(new_version.GetString()),
                               true);  // overwrite version
}
