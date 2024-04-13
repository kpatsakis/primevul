void Document::InitSecurityContext(const DocumentInit& initializer) {
  DCHECK(!GetSecurityOrigin());

  if (!initializer.HasSecurityContext()) {
    cookie_url_ = KURL(g_empty_string);
    SetSecurityOrigin(SecurityOrigin::CreateUniqueOpaque());
    InitContentSecurityPolicy();
    ApplyFeaturePolicy({});
    return;
  }

  SandboxFlags sandbox_flags = initializer.GetSandboxFlags();
  if (fetcher_->Archive()) {
    sandbox_flags |=
        kSandboxAll &
        ~(kSandboxPopups | kSandboxPropagatesToAuxiliaryBrowsingContexts);
  }
  EnforceSandboxFlags(sandbox_flags);
  SetInsecureRequestPolicy(initializer.GetInsecureRequestPolicy());
  if (initializer.InsecureNavigationsToUpgrade()) {
    for (auto to_upgrade : *initializer.InsecureNavigationsToUpgrade())
      AddInsecureNavigationUpgrade(to_upgrade);
  }

  ContentSecurityPolicy* last_origin_document_csp_ =
      frame_ ? frame_->Loader().GetLastOriginDocumentCSP() : nullptr;

  scoped_refptr<SecurityOrigin> document_origin;
  cookie_url_ = url_;
  if (initializer.OriginToCommit()) {
    document_origin = initializer.OriginToCommit();
  } else {
    if (Document* owner_document = initializer.OwnerDocument()) {
      document_origin = owner_document->GetMutableSecurityOrigin();
      cookie_url_ = owner_document->CookieURL();
      if (url_.IsEmpty())
        last_origin_document_csp_ = owner_document->GetContentSecurityPolicy();
    } else {
      document_origin = SecurityOrigin::CreateWithReferenceOrigin(
          url_, initializer.InitiatorOrigin().get());
    }
  }

  if (IsSandboxed(kSandboxOrigin)) {
    DCHECK(!initializer.ContextDocument());
    scoped_refptr<SecurityOrigin> sandboxed_origin =
        initializer.OriginToCommit() ? initializer.OriginToCommit()
                                     : document_origin->DeriveNewOpaqueOrigin();

    if (initializer.OwnerDocument()) {
      if (document_origin->IsPotentiallyTrustworthy())
        sandboxed_origin->SetOpaqueOriginIsPotentiallyTrustworthy(true);
      if (document_origin->CanLoadLocalResources())
        sandboxed_origin->GrantLoadLocalResources();
      if (url_.IsEmpty()) {
        last_origin_document_csp_ =
            initializer.OwnerDocument()->GetContentSecurityPolicy();
      }
    }
    cookie_url_ = url_;
    SetSecurityOrigin(std::move(sandboxed_origin));
  } else {
    SetSecurityOrigin(std::move(document_origin));
  }

  if (initializer.IsHostedInReservedIPRange()) {
    SetAddressSpace(GetSecurityOrigin()->IsLocalhost()
                        ? mojom::IPAddressSpace::kLocal
                        : mojom::IPAddressSpace::kPrivate);
  } else if (GetSecurityOrigin()->IsLocal()) {
    SetAddressSpace(mojom::IPAddressSpace::kLocal);
  } else {
    SetAddressSpace(mojom::IPAddressSpace::kPublic);
  }

  if (ImportsController()) {
    SetContentSecurityPolicy(
        ImportsController()->Master()->GetContentSecurityPolicy());
  } else {
    InitContentSecurityPolicy(nullptr, last_origin_document_csp_);
  }

  if (Settings* settings = initializer.GetSettings()) {
    if (!settings->GetWebSecurityEnabled()) {
      GetMutableSecurityOrigin()->GrantUniversalAccess();
    } else if (GetSecurityOrigin()->IsLocal()) {
      if (settings->GetAllowUniversalAccessFromFileURLs()) {
        GetMutableSecurityOrigin()->GrantUniversalAccess();
      } else if (!settings->GetAllowFileAccessFromFileURLs()) {
        GetMutableSecurityOrigin()->BlockLocalAccessFromLocalOrigin();
      }
    }
  }

  if (GetSecurityOrigin()->IsOpaque() &&
      SecurityOrigin::Create(url_)->IsPotentiallyTrustworthy())
    GetMutableSecurityOrigin()->SetOpaqueOriginIsPotentiallyTrustworthy(true);

  ParsedFeaturePolicy declared_policy = {};
  if (GetSandboxFlags() != kSandboxNone &&
      RuntimeEnabledFeatures::FeaturePolicyForSandboxEnabled()) {
    ApplySandboxFlagsToParsedFeaturePolicy(GetSandboxFlags(), declared_policy);
  }
  ApplyFeaturePolicy(declared_policy);

  InitSecureContextState();
}
