  PermissionMessages GetMessages(const APIPermissionSet& permissions,
                                 Manifest::Type type) {
    return message_provider_->GetPermissionMessages(
        message_provider_->GetAllPermissionIDs(
            PermissionSet(permissions, ManifestPermissionSet(), URLPatternSet(),
                          URLPatternSet()),
            type));
  }
