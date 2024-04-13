void OneClickSigninHelper::OnStateChanged() {
  DCHECK(!original_continue_url_.is_empty());

  content::WebContents* contents = web_contents();
  Profile* profile =
      Profile::FromBrowserContext(contents->GetBrowserContext());
  ProfileSyncService* sync_service =
      ProfileSyncServiceFactory::GetForProfile(profile);

  if (signin::IsAutoCloseEnabledInURL(original_continue_url_)) {
    base::MessageLoop::current()->PostTask(
        FROM_HERE,
        base::Bind(&CloseTab, base::Unretained(contents)));
  } else {
    if (sync_service->FirstSetupInProgress())
      return;

    if (sync_service->sync_initialized() &&
        signin::GetSourceForPromoURL(original_continue_url_)
            != signin::SOURCE_SETTINGS) {
      contents->GetController().LoadURL(original_continue_url_,
                                        content::Referrer(),
                                        content::PAGE_TRANSITION_AUTO_TOPLEVEL,
                                        std::string());
    }
  }

  original_continue_url_ = GURL();
  sync_service->RemoveObserver(this);
}
