void ZeroSuggestProvider::Run(const GURL& suggest_url) {
  if (OmniboxFieldTrial::InZeroSuggestMostVisitedFieldTrial()) {
    most_visited_urls_.clear();
    scoped_refptr<history::TopSites> ts = client()->GetTopSites();
    if (ts) {
      waiting_for_most_visited_urls_request_ = true;
      ts->GetMostVisitedURLs(
          base::Bind(&ZeroSuggestProvider::OnMostVisitedUrlsAvailable,
                     weak_ptr_factory_.GetWeakPtr()), false);
    }
  } else {
    net::NetworkTrafficAnnotationTag traffic_annotation =
        net::DefineNetworkTrafficAnnotation("omnibox_zerosuggest", R"(
        semantics {
          sender: "Omnibox"
          description:
            "When the user focuses the omnibox, Chrome can provide search or "
            "navigation suggestions from the default search provider in the "
            "omnibox dropdown, based on the current page URL.\n"
            "This is limited to users whose default search engine is Google, "
            "as no other search engines currently support this kind of "
            "suggestion."
          trigger: "The omnibox receives focus."
          data: "The URL of the current page."
          destination: GOOGLE_OWNED_SERVICE
        }
        policy {
          cookies_allowed: true
          cookies_store: "user"
          setting:
            "Users can control this feature via the 'Use a prediction service "
            "to help complete searches and URLs typed in the address bar' "
            "settings under 'Privacy'. The feature is enabled by default."
          chrome_policy {
            SearchSuggestEnabled {
                policy_options {mode: MANDATORY}
                SearchSuggestEnabled: false
            }
          }
        })");
    const int kFetcherID = 1;
    fetcher_ =
        net::URLFetcher::Create(kFetcherID, suggest_url, net::URLFetcher::GET,
                                this, traffic_annotation);
    data_use_measurement::DataUseUserData::AttachToFetcher(
        fetcher_.get(), data_use_measurement::DataUseUserData::OMNIBOX);
    fetcher_->SetRequestContext(client()->GetRequestContext());
    fetcher_->SetLoadFlags(net::LOAD_DO_NOT_SAVE_COOKIES);
    net::HttpRequestHeaders headers;
    bool is_signed_in = false;
    variations::AppendVariationHeaders(fetcher_->GetOriginalURL(),
                                       client()->IsOffTheRecord(), false,
                                       is_signed_in, &headers);
    fetcher_->SetExtraRequestHeaders(headers.ToString());
    fetcher_->Start();
    LogOmniboxZeroSuggestRequest(ZERO_SUGGEST_REQUEST_SENT);
  }
}
