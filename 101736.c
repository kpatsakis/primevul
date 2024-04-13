void Document::ProcessBaseElement() {
  UseCounter::Count(*this, WebFeature::kBaseElement);

  const AtomicString* href = nullptr;
  const AtomicString* target = nullptr;
  for (HTMLBaseElement* base = Traversal<HTMLBaseElement>::FirstWithin(*this);
       base && (!href || !target);
       base = Traversal<HTMLBaseElement>::Next(*base)) {
    if (!href) {
      const AtomicString& value = base->FastGetAttribute(hrefAttr);
      if (!value.IsNull())
        href = &value;
    }
    if (!target) {
      const AtomicString& value = base->FastGetAttribute(targetAttr);
      if (!value.IsNull())
        target = &value;
    }
    if (GetContentSecurityPolicy()->IsActive()) {
      UseCounter::Count(*this,
                        WebFeature::kContentSecurityPolicyWithBaseElement);
    }
  }

  KURL base_element_url;
  if (href) {
    String stripped_href = StripLeadingAndTrailingHTMLSpaces(*href);
    if (!stripped_href.IsEmpty())
      base_element_url = KURL(FallbackBaseURL(), stripped_href);
  }

  if (!base_element_url.IsEmpty()) {
    if (base_element_url.ProtocolIsData()) {
      UseCounter::Count(*this, WebFeature::kBaseWithDataHref);
      AddConsoleMessage(ConsoleMessage::Create(
          kSecurityMessageSource, kErrorMessageLevel,
          "'data:' URLs may not be used as base URLs for a document."));
    }
    if (!GetSecurityOrigin()->CanRequest(base_element_url))
      UseCounter::Count(*this, WebFeature::kBaseWithCrossOriginHref);
  }

  if (base_element_url != base_element_url_ &&
      !base_element_url.ProtocolIsData() &&
      GetContentSecurityPolicy()->AllowBaseURI(base_element_url)) {
    base_element_url_ = base_element_url;
    UpdateBaseURL();
  }

  if (target) {
    if (target->Contains('\n') || target->Contains('\r'))
      UseCounter::Count(*this, WebFeature::kBaseWithNewlinesInTarget);
    if (target->Contains('<'))
      UseCounter::Count(*this, WebFeature::kBaseWithOpenBracketInTarget);
    base_target_ = *target;
  } else {
    base_target_ = g_null_atom;
  }
}
