static jlong JNI_OfflinePageDownloadBridge_Init(
    JNIEnv* env,
    const JavaParamRef<jobject>& obj,
    const JavaParamRef<jobject>& j_profile) {
  content::BrowserContext* browser_context =
      ProfileAndroid::FromProfileAndroid(j_profile);

  OfflinePageModel* offline_page_model =
      OfflinePageModelFactory::GetForBrowserContext(browser_context);
  DCHECK(offline_page_model);

  DownloadUIAdapter* adapter =
      DownloadUIAdapter::FromOfflinePageModel(offline_page_model);

  if (!adapter) {
    RequestCoordinator* request_coordinator =
        RequestCoordinatorFactory::GetForBrowserContext(browser_context);
    DCHECK(request_coordinator);
    offline_items_collection::OfflineContentAggregator* aggregator =
        OfflineContentAggregatorFactory::GetForBrowserContext(browser_context);
    DCHECK(aggregator);
    adapter = new DownloadUIAdapter(
        aggregator, offline_page_model, request_coordinator,
        std::make_unique<DownloadUIAdapterDelegate>(offline_page_model));
    DownloadUIAdapter::AttachToOfflinePageModel(base::WrapUnique(adapter),
                                                offline_page_model);
  }

  return reinterpret_cast<jlong>(new OfflinePageDownloadBridge(env, obj));
}
