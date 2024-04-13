bool SetSanitizationInfo(crash_reporter::CrashReporterClient* client,
                         SanitizationInformation* info) {
  const char* const* whitelist = nullptr;
  void* target_module = nullptr;
  bool sanitize_stacks = false;
  client->GetSanitizationInformation(&whitelist, &target_module,
                                     &sanitize_stacks);
  info->annotations_whitelist_address = FromPointerCast<VMAddress>(whitelist);
  info->target_module_address = FromPointerCast<VMAddress>(target_module);
  info->sanitize_stacks = sanitize_stacks;
  return whitelist != nullptr || target_module != nullptr || sanitize_stacks;
}
