bool ExtensionService::IsInstalledApp(const GURL& url) {
  return !!GetInstalledApp(url);
}
