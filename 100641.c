void OneClickSigninHelper::DidStopLoading(
    content::RenderViewHost* render_view_host) {
  content::WebContents* contents = web_contents();
  const GURL url = contents->GetLastCommittedURL();
  Profile* profile =
      Profile::FromBrowserContext(contents->GetBrowserContext());
  VLOG(1) << "OneClickSigninHelper::DidStopLoading: url=" << url.spec();

  if (!error_message_.empty() && auto_accept_ == AUTO_ACCEPT_EXPLICIT) {
    VLOG(1) << "OneClickSigninHelper::DidStopLoading: error=" << error_message_;
    RemoveSigninRedirectURLHistoryItem(contents);
    Browser* browser = chrome::FindBrowserWithWebContents(contents);

    RedirectToNtpOrAppsPage(web_contents(), source_);
    ShowSigninErrorBubble(browser, error_message_);
    CleanTransientState();
    return;
  }

  if (AreWeShowingSignin(url, source_, email_)) {
    if (!showing_signin_) {
      if (source_ == signin::SOURCE_UNKNOWN)
        LogOneClickHistogramValue(one_click_signin::HISTOGRAM_SHOWN);
      else
        LogHistogramValue(source_, one_click_signin::HISTOGRAM_SHOWN);
    }
    showing_signin_ = true;
  }

  GURL::Replacements replacements;
  replacements.ClearQuery();
  const bool continue_url_match = (
      continue_url_.is_valid() &&
      url.ReplaceComponents(replacements) ==
        continue_url_.ReplaceComponents(replacements));

  if (continue_url_match)
    RemoveSigninRedirectURLHistoryItem(contents);

  if (email_.empty()) {
    VLOG(1) << "OneClickSigninHelper::DidStopLoading: nothing to do";
    if (continue_url_match) {
      if (auto_accept_ == AUTO_ACCEPT_EXPLICIT)
        RedirectToSignin();
      std::string unused_value;
      if (net::GetValueForKeyInQuery(url, "ntp", &unused_value)) {
        signin::SetUserSkippedPromo(profile);
        RedirectToNtpOrAppsPage(web_contents(), source_);
      }
    } else {
      if (!IsValidGaiaSigninRedirectOrResponseURL(url) &&
          ++untrusted_navigations_since_signin_visit_ > kMaxNavigationsSince) {
        CleanTransientState();
      }
    }

    return;
  }

  if (!continue_url_match && IsValidGaiaSigninRedirectOrResponseURL(url))
    return;

  if (auto_accept_ == AUTO_ACCEPT_EXPLICIT) {
    DCHECK(source_ != signin::SOURCE_UNKNOWN);
    if (!continue_url_match) {
      VLOG(1) << "OneClickSigninHelper::DidStopLoading: invalid url='"
              << url.spec()
              << "' expected continue url=" << continue_url_;
      CleanTransientState();
      return;
    }

    signin::Source source = signin::GetSourceForPromoURL(url);
    if (source != source_) {
      source_ = source;
      switched_to_advanced_ = source == signin::SOURCE_SETTINGS;
    }
  }

  Browser* browser = chrome::FindBrowserWithWebContents(contents);

  VLOG(1) << "OneClickSigninHelper::DidStopLoading: signin is go."
          << " auto_accept=" << auto_accept_
          << " source=" << source_;

  switch (auto_accept_) {
    case AUTO_ACCEPT_NONE:
      if (showing_signin_)
        LogOneClickHistogramValue(one_click_signin::HISTOGRAM_DISMISSED);
      break;
    case AUTO_ACCEPT_ACCEPTED:
      LogOneClickHistogramValue(one_click_signin::HISTOGRAM_ACCEPTED);
      LogOneClickHistogramValue(one_click_signin::HISTOGRAM_WITH_DEFAULTS);
      SigninManager::DisableOneClickSignIn(profile);
      if (!do_not_start_sync_for_testing_) {
        StartSync(
            StartSyncArgs(profile, browser, auto_accept_,
                          session_index_, email_, password_,
                          NULL /* don't force to show sync setup in same tab */,
                          true /* confirmation_required */, source_,
                          CreateSyncStarterCallback()),
            OneClickSigninSyncStarter::SYNC_WITH_DEFAULT_SETTINGS);
      }
      break;
    case AUTO_ACCEPT_CONFIGURE:
      LogOneClickHistogramValue(one_click_signin::HISTOGRAM_ACCEPTED);
      LogOneClickHistogramValue(one_click_signin::HISTOGRAM_WITH_ADVANCED);
      SigninManager::DisableOneClickSignIn(profile);
      if (!do_not_start_sync_for_testing_) {
        StartSync(
            StartSyncArgs(profile, browser, auto_accept_,
                          session_index_, email_, password_,
                          NULL  /* don't force sync setup in same tab */,
                          true  /* confirmation_required */, source_,
                          CreateSyncStarterCallback()),
            OneClickSigninSyncStarter::CONFIGURE_SYNC_FIRST);
      }
      break;
    case AUTO_ACCEPT_EXPLICIT: {
      signin::Source original_source =
          signin::GetSourceForPromoURL(original_continue_url_);
      if (switched_to_advanced_) {
        LogHistogramValue(original_source,
                          one_click_signin::HISTOGRAM_WITH_ADVANCED);
        LogHistogramValue(original_source,
                          one_click_signin::HISTOGRAM_ACCEPTED);
      } else {
        LogHistogramValue(source_, one_click_signin::HISTOGRAM_ACCEPTED);
        LogHistogramValue(source_, one_click_signin::HISTOGRAM_WITH_DEFAULTS);
      }

      ProfileSyncService* sync_service =
          ProfileSyncServiceFactory::GetForProfile(profile);
      OneClickSigninSyncStarter::StartSyncMode start_mode =
          source_ == signin::SOURCE_SETTINGS ?
              (SigninGlobalError::GetForProfile(profile)->HasMenuItem() &&
               sync_service && sync_service->HasSyncSetupCompleted()) ?
                  OneClickSigninSyncStarter::SHOW_SETTINGS_WITHOUT_CONFIGURE :
                  OneClickSigninSyncStarter::CONFIGURE_SYNC_FIRST :
              OneClickSigninSyncStarter::SYNC_WITH_DEFAULT_SETTINGS;

      std::string last_email =
          profile->GetPrefs()->GetString(prefs::kGoogleServicesLastUsername);

      if (!last_email.empty() && !gaia::AreEmailsSame(last_email, email_)) {

        ConfirmEmailDialogDelegate::AskForConfirmation(
            contents,
            last_email,
            email_,
            base::Bind(
                &StartExplicitSync,
                StartSyncArgs(profile, browser, auto_accept_,
                              session_index_, email_, password_, contents,
                              false /* confirmation_required */, source_,
                              CreateSyncStarterCallback()),
                contents,
                start_mode));
      } else {
        if (!do_not_start_sync_for_testing_) {
          StartSync(
              StartSyncArgs(profile, browser, auto_accept_,
                            session_index_, email_, password_, contents,
                            untrusted_confirmation_required_, source_,
                            CreateSyncStarterCallback()),
              start_mode);
        }

        RedirectToNtpOrAppsPageIfNecessary(web_contents(), source_);
      }

      if (original_source == signin::SOURCE_SETTINGS ||
          (original_source == signin::SOURCE_WEBSTORE_INSTALL &&
           source_ == signin::SOURCE_SETTINGS)) {
        ProfileSyncService* sync_service =
            ProfileSyncServiceFactory::GetForProfile(profile);
        if (sync_service)
          sync_service->AddObserver(this);
      }
      break;
    }
    case AUTO_ACCEPT_REJECTED_FOR_PROFILE:
      AddEmailToOneClickRejectedList(profile, email_);
      LogOneClickHistogramValue(one_click_signin::HISTOGRAM_REJECTED);
      break;
    default:
      NOTREACHED() << "Invalid auto_accept=" << auto_accept_;
      break;
  }

  CleanTransientState();
}
