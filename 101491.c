bool StartupBrowserCreator::ProcessCmdLineImpl(
    const base::CommandLine& command_line,
    const base::FilePath& cur_dir,
    bool process_startup,
    Profile* last_used_profile,
    const Profiles& last_opened_profiles) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  TRACE_EVENT0("startup", "StartupBrowserCreator::ProcessCmdLineImpl");

  DCHECK(last_used_profile);
  if (process_startup &&
      command_line.HasSwitch(switches::kDisablePromptOnRepost)) {
    content::NavigationController::DisablePromptOnRepost();
  }

  bool silent_launch = false;
  bool can_use_last_profile =
      (CanOpenProfileOnStartup(last_used_profile) &&
       !IncognitoModePrefs::ShouldLaunchIncognito(
           command_line, last_used_profile->GetPrefs()));

#if BUILDFLAG(ENABLE_PRINT_PREVIEW)
  if (command_line.HasSwitch(switches::kCloudPrintFile) &&
      can_use_last_profile &&
      print_dialog_cloud::CreatePrintDialogFromCommandLine(last_used_profile,
                                                           command_line)) {
    silent_launch = true;
  }
#endif  // BUILDFLAG(ENABLE_PRINT_PREVIEW)

  if (command_line.HasSwitch(switches::kValidateCrx)) {
    if (!process_startup) {
      LOG(ERROR) << "chrome is already running; you must close all running "
                 << "instances before running with the --"
                 << switches::kValidateCrx << " flag";
      return false;
    }
    extensions::StartupHelper helper;
    std::string message;
    std::string error;
    if (helper.ValidateCrx(command_line, &error))
      message = std::string("ValidateCrx Success");
    else
      message = std::string("ValidateCrx Failure: ") + error;
    printf("%s\n", message.c_str());
    return false;
  }

#if defined(OS_CHROMEOS)

  if (command_line.HasSwitch(chromeos::switches::kLoginManager))
    silent_launch = true;

  if (chrome::IsRunningInAppMode() &&
      command_line.HasSwitch(switches::kAppId)) {
    chromeos::LaunchAppOrDie(
        last_used_profile,
        command_line.GetSwitchValueASCII(switches::kAppId));

    silent_launch = true;
  }

  if (chromeos::DemoAppLauncher::IsDemoAppSession(
          cryptohome::Identification::FromString(
              command_line.GetSwitchValueASCII(chromeos::switches::kLoginUser))
              .GetAccountId())) {
    chrome::AttemptUserExit();
    return false;
  }
#endif  // OS_CHROMEOS

#if defined(TOOLKIT_VIEWS) && defined(USE_X11)
  ui::TouchFactory::SetTouchDeviceListFromCommandLine();
#endif

#if defined(OS_MACOSX)
  if (web_app::MaybeRebuildShortcut(command_line))
    return true;
#endif

  if (!process_startup &&
      command_line.HasSwitch(switches::kDumpBrowserHistograms)) {
    base::FilePath output_file(
        command_line.GetSwitchValuePath(switches::kDumpBrowserHistograms));
    if (!output_file.empty()) {
      base::PostTask(FROM_HERE,
                     {base::ThreadPool(), base::MayBlock(),
                      base::TaskPriority::BEST_EFFORT,
                      base::TaskShutdownBehavior::BLOCK_SHUTDOWN},
                     base::BindOnce(&DumpBrowserHistograms, output_file));
    }
    silent_launch = true;
  }

#if !defined(OS_CHROMEOS)
  if (!process_startup &&
      base::FeatureList::IsEnabled(features::kOnConnectNative) &&
      command_line.HasSwitch(switches::kNativeMessagingConnectHost) &&
      command_line.HasSwitch(switches::kNativeMessagingConnectExtension)) {
    silent_launch = true;
    extensions::LaunchNativeMessageHostFromNativeApp(
        command_line.GetSwitchValueASCII(
            switches::kNativeMessagingConnectExtension),
        command_line.GetSwitchValueASCII(switches::kNativeMessagingConnectHost),
        command_line.GetSwitchValueASCII(switches::kNativeMessagingConnectId),
        last_used_profile);
  }
#endif

  if (!process_startup && command_line.HasSwitch(switches::kNoStartupWindow))
    silent_launch = true;

  if (silent_launch) {
    if (process_startup)
      startup_metric_utils::SetNonBrowserUIDisplayed();
    return true;
  }

  if (command_line.HasSwitch(extensions::switches::kLoadApps) &&
      can_use_last_profile) {
    if (!ProcessLoadApps(command_line, cur_dir, last_used_profile))
      return false;

    if (chrome::GetBrowserCount(last_used_profile) != 0)
      return true;
  }

  if (command_line.HasSwitch(apps::kLoadAndLaunchApp) && can_use_last_profile) {
    base::CommandLine::StringType path =
        command_line.GetSwitchValueNative(apps::kLoadAndLaunchApp);

    if (!apps::AppLoadService::Get(last_used_profile)->LoadAndLaunch(
            base::FilePath(path), command_line, cur_dir)) {
      return false;
    }

    if (chrome::GetBrowserCount(last_used_profile) != 0)
      return true;
  }

#if defined(OS_WIN)
  if (command_line.HasSwitch(switches::kWinJumplistAction)) {
    jumplist::LogJumplistActionFromSwitchValue(
        command_line.GetSwitchValueASCII(switches::kWinJumplistAction));
    last_used_profile->SetUserData(
        chrome::kJumpListIconDirname,
        base::WrapUnique(new base::SupportsUserData::Data()));
  }
#endif  // defined(OS_WIN)

  return LaunchBrowserForLastProfiles(command_line, cur_dir, process_startup,
                                      last_used_profile, last_opened_profiles);
}
