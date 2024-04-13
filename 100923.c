void RenderViewImpl::Initialize(RenderViewImplParams* params) {
#if defined(ENABLE_PLUGINS)
  pepper_helper_.reset(new PepperPluginDelegateImpl(this));
#else
  pepper_helper_.reset(new RenderViewPepperHelper());
#endif
  set_throttle_input_events(params->renderer_prefs.throttle_input_events);
  routing_id_ = params->routing_id;
  surface_id_ = params->surface_id;
  if (params->opener_id != MSG_ROUTING_NONE && params->is_renderer_created)
    opener_id_ = params->opener_id;

  DCHECK_GE(next_page_id_, 0);

#if defined(ENABLE_NOTIFICATIONS)
  notification_provider_ = new NotificationProvider(this);
#else
  notification_provider_ = NULL;
#endif

  webwidget_ = WebView::create(this);
  webwidget_mouse_lock_target_.reset(new WebWidgetLockTarget(webwidget_));

  const CommandLine& command_line = *CommandLine::ForCurrentProcess();

#if defined(OS_ANDROID)
  content::DeviceTelephonyInfo device_info;

  const std::string region_code =
      command_line.HasSwitch(switches::kNetworkCountryIso)
          ? command_line.GetSwitchValueASCII(switches::kNetworkCountryIso)
          : device_info.GetNetworkCountryIso();
  content_detectors_.push_back(linked_ptr<ContentDetector>(
      new AddressDetector()));
  content_detectors_.push_back(linked_ptr<ContentDetector>(
      new PhoneNumberDetector(region_code)));
  content_detectors_.push_back(linked_ptr<ContentDetector>(
      new EmailDetector()));
#endif

  if (params->counter) {
    shared_popup_counter_ = params->counter;
    if (!params->swapped_out)
      shared_popup_counter_->data++;
    decrement_shared_popup_at_destruction_ = true;
  } else {
    shared_popup_counter_ = new SharedRenderViewCounter(0);
    decrement_shared_popup_at_destruction_ = false;
  }

  RenderThread::Get()->AddRoute(routing_id_, this);
  AddRef();

  if (opener_id_ == MSG_ROUTING_NONE) {
    did_show_ = true;
    CompleteInit();
  }

  g_view_map.Get().insert(std::make_pair(webview(), this));
  g_routing_id_view_map.Get().insert(std::make_pair(routing_id_, this));
  webview()->setDeviceScaleFactor(device_scale_factor_);
  webview()->settings()->setAcceleratedCompositingForFixedPositionEnabled(
      ShouldUseFixedPositionCompositing(device_scale_factor_));

  webkit_preferences_.Apply(webview());
  webview()->initializeMainFrame(this);

  if (command_line.HasSwitch(switches::kEnableTouchDragDrop))
    webview()->settings()->setTouchDragDropEnabled(true);

  if (!params->frame_name.empty())
    webview()->mainFrame()->setName(params->frame_name);
  webview()->settings()->setMinimumTimerInterval(
      is_hidden() ? webkit_glue::kBackgroundTabTimerInterval :
          webkit_glue::kForegroundTabTimerInterval);

  OnSetRendererPrefs(params->renderer_prefs);

#if defined(ENABLE_WEBRTC)
  if (!media_stream_dispatcher_)
    media_stream_dispatcher_ = new MediaStreamDispatcher(this);
#endif

  new MHTMLGenerator(this);
#if defined(OS_MACOSX)
  new TextInputClientObserver(this);
#endif  // defined(OS_MACOSX)

#if defined(OS_ANDROID)
  media_player_manager_.reset(
      new webkit_media::WebMediaPlayerManagerAndroid());
#endif

  devtools_agent_ = new DevToolsAgent(this);
  mouse_lock_dispatcher_ = new RenderViewMouseLockDispatcher(this);
  favicon_helper_ = new FaviconHelper(this);

  OnSetAccessibilityMode(params->accessibility_mode);

  new IdleUserDetector(this);

  if (command_line.HasSwitch(switches::kDomAutomationController))
    enabled_bindings_ |= BINDINGS_POLICY_DOM_AUTOMATION;

  ProcessViewLayoutFlags(command_line);

  GetContentClient()->renderer()->RenderViewCreated(this);

  if (params->opener_id != MSG_ROUTING_NONE && !params->is_renderer_created) {
    RenderViewImpl* opener_view = FromRoutingID(params->opener_id);
    if (opener_view)
      webview()->mainFrame()->setOpener(opener_view->webview()->mainFrame());
  }

  if (is_swapped_out_)
    NavigateToSwappedOutURL(webview()->mainFrame());
}
