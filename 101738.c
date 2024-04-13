void Document::SetFeaturePolicy(const String& feature_policy_header) {
  if (!RuntimeEnabledFeatures::FeaturePolicyEnabled())
    return;

  if (!feature_policy_header.IsEmpty())
    UseCounter::Count(*this, WebFeature::kFeaturePolicyHeader);

  FeaturePolicy* parent_feature_policy = nullptr;
  ParsedFeaturePolicy container_policy;
  Vector<String> messages;
  const ParsedFeaturePolicy& parsed_header = ParseFeaturePolicyHeader(
      feature_policy_header, GetSecurityOrigin(), &messages);

  if (frame_) {
    if (!frame_->IsMainFrame()) {
      parent_feature_policy =
          frame_->Tree().Parent()->GetSecurityContext()->GetFeaturePolicy();
    }
    if (frame_->Owner())
      container_policy = frame_->Owner()->ContainerPolicy();
  }

  InitializeFeaturePolicy(parsed_header, container_policy,
                          parent_feature_policy);

  for (const auto& message : messages) {
    AddConsoleMessage(
        ConsoleMessage::Create(kOtherMessageSource, kErrorMessageLevel,
                               "Error with Feature-Policy header: " + message));
  }
  if (frame_ && !parsed_header.empty())
    frame_->Client()->DidSetFeaturePolicyHeader(parsed_header);
}
