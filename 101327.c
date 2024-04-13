BookmarkShortcutDisposition GetBookmarkShortcutDisposition(Profile* profile) {
#if defined(ENABLE_EXTENSIONS)
  extensions::CommandService* command_service =
      extensions::CommandService::Get(profile);

  extensions::ExtensionRegistry* registry =
      extensions::ExtensionRegistry::Get(profile);
  if (!registry)
    return BOOKMARK_SHORTCUT_DISPOSITION_UNCHANGED;

  const extensions::ExtensionSet& extension_set =
      registry->enabled_extensions();

  bool removed = false;
  for (extensions::ExtensionSet::const_iterator i = extension_set.begin();
       i != extension_set.end();
       ++i) {
    if (command_service->RequestsBookmarkShortcutOverride(i->get()))
      return BOOKMARK_SHORTCUT_DISPOSITION_OVERRIDE_REQUESTED;

    if (!removed &&
        extensions::CommandService::RemovesBookmarkShortcut(i->get())) {
      removed = true;
    }
  }

  if (removed)
    return BOOKMARK_SHORTCUT_DISPOSITION_REMOVED;
#endif
  return BOOKMARK_SHORTCUT_DISPOSITION_UNCHANGED;
}
