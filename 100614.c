bool SyncManager::SyncInternal::Init(
    const FilePath& database_location,
    const WeakHandle<JsEventHandler>& event_handler,
    const std::string& sync_server_and_path,
    int port,
    bool use_ssl,
    HttpPostProviderFactory* post_factory,
    ModelSafeWorkerRegistrar* model_safe_worker_registrar,
    ChangeDelegate* change_delegate,
    const std::string& user_agent,
    const SyncCredentials& credentials,
    sync_notifier::SyncNotifier* sync_notifier,
    const std::string& restored_key_for_bootstrapping,
    bool setup_for_test_mode,
    UnrecoverableErrorHandler* unrecoverable_error_handler) {
  CHECK(!initialized_);

  DCHECK(thread_checker_.CalledOnValidThread());

  DVLOG(1) << "Starting SyncInternal initialization.";

  weak_handle_this_ = MakeWeakHandle(weak_ptr_factory_.GetWeakPtr());

  registrar_ = model_safe_worker_registrar;
  change_delegate_ = change_delegate;
  setup_for_test_mode_ = setup_for_test_mode;

  sync_notifier_.reset(sync_notifier);

  AddObserver(&js_sync_manager_observer_);
  SetJsEventHandler(event_handler);

  AddObserver(&debug_info_event_listener_);

  share_.dir_manager.reset(new DirectoryManager(database_location));

  connection_manager_.reset(new SyncAPIServerConnectionManager(
      sync_server_and_path, port, use_ssl, user_agent, post_factory));

  net::NetworkChangeNotifier::AddIPAddressObserver(this);
  observing_ip_address_changes_ = true;

  connection_manager()->AddListener(this);

  unrecoverable_error_handler_ = unrecoverable_error_handler;

  if (!setup_for_test_mode_) {
    DVLOG(1) << "Sync is bringing up SyncSessionContext.";
    std::vector<SyncEngineEventListener*> listeners;
    listeners.push_back(&allstatus_);
    listeners.push_back(this);
    SyncSessionContext* context = new SyncSessionContext(
        connection_manager_.get(),
        dir_manager(),
        model_safe_worker_registrar,
        listeners,
        &debug_info_event_listener_);
    context->set_account_name(credentials.email);
    scheduler_.reset(new SyncScheduler(name_, context, new Syncer()));
  }

  bool signed_in = SignIn(credentials);

  if (signed_in) {
    if (scheduler()) {
      scheduler()->Start(
          browser_sync::SyncScheduler::CONFIGURATION_MODE, base::Closure());
    }

    initialized_ = true;

    ReadTransaction trans(FROM_HERE, GetUserShare());
    trans.GetCryptographer()->Bootstrap(restored_key_for_bootstrapping);
    trans.GetCryptographer()->AddObserver(this);
  }

  FOR_EACH_OBSERVER(SyncManager::Observer, observers_,
                    OnInitializationComplete(
                        MakeWeakHandle(weak_ptr_factory_.GetWeakPtr()),
                        signed_in));

  if (!signed_in && !setup_for_test_mode_)
    return false;

  sync_notifier_->AddObserver(this);

  if (CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kSyncThrowUnrecoverableError)) {
    ReadTransaction trans(FROM_HERE, GetUserShare());
    trans.GetWrappedTrans()->OnUnrecoverableError(FROM_HERE,
        "Simulating unrecoverable error for testing purpose.");
  }

  return signed_in;
}
