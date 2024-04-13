PropertyBag* ExtensionService::GetPropertyBag(const Extension* extension) {
  return &extension_runtime_data_[extension->id()].property_bag;
}
