void ChromeContentBrowserClient::InitWebContextInterfaces() {
  frame_interfaces_ = std::make_unique<service_manager::BinderRegistry>();
  frame_interfaces_parameterized_ = std::make_unique<
      service_manager::BinderRegistryWithArgs<content::RenderFrameHost*>>();
  worker_interfaces_parameterized_ =
      std::make_unique<service_manager::BinderRegistryWithArgs<
          content::RenderProcessHost*, const url::Origin&>>();

  frame_interfaces_parameterized_->AddInterface(base::BindRepeating(
      &ChromeLanguageDetectionTabHelper::BindContentTranslateDriver));

  frame_interfaces_parameterized_->AddInterface(
      base::BindRepeating(&ChromePasswordManagerClient::BindCredentialManager));
  frame_interfaces_parameterized_->AddInterface(
      base::Bind(&InsecureSensitiveInputDriverFactory::BindDriver));

#if defined(OS_ANDROID)
  frame_interfaces_parameterized_->AddInterface(base::Bind(
      &ForwardToJavaFrameRegistry<blink::mojom::InstalledAppProvider>));
  frame_interfaces_parameterized_->AddInterface(
      base::Bind(&ForwardToJavaFrameRegistry<payments::mojom::PaymentRequest>));
  frame_interfaces_parameterized_->AddInterface(
      base::Bind(&ForwardToJavaFrameRegistry<blink::mojom::Authenticator>));
#else
  if (base::FeatureList::IsEnabled(features::kWebPayments)) {
    frame_interfaces_parameterized_->AddInterface(
        base::Bind(&payments::CreatePaymentRequest));
  }
#endif

#if defined(OS_ANDROID)
  frame_interfaces_parameterized_->AddInterface(base::Bind(
      &ForwardToJavaWebContentsRegistry<blink::mojom::ShareService>));
#elif defined(OS_LINUX) || defined(OS_WIN)
  frame_interfaces_->AddInterface(base::Bind(&ShareServiceImpl::Create));
#endif

#if defined(OS_CHROMEOS)
  frame_interfaces_->AddInterface(
      base::BindRepeating(&BadgeServiceImpl::Create));
#endif

  frame_interfaces_parameterized_->AddInterface(
      base::BindRepeating(&NavigationPredictor::Create));

#if defined(OS_ANDROID)
  frame_interfaces_parameterized_->AddInterface(
      base::BindRepeating(&offline_pages::OfflinePageAutoFetcher::Create),
      base::CreateSingleThreadTaskRunnerWithTraits({BrowserThread::UI}));
#endif
}
