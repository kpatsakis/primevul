const std::string& CSPInfo::GetResourceContentSecurityPolicy(
    const Extension* extension,
    const std::string& relative_path) {
  return SandboxedPageInfo::IsSandboxedPage(extension, relative_path) ?
      SandboxedPageInfo::GetContentSecurityPolicy(extension) :
      GetContentSecurityPolicy(extension);
}
