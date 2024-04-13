RenderWidgetHostImpl::RenderWidgetHostImpl(RenderWidgetHostDelegate* delegate,
                                           RenderProcessHost* process,
                                           int32_t routing_id,
                                           bool hidden)
    : renderer_initialized_(false),
      destroyed_(false),
      delegate_(delegate),
      owner_delegate_(nullptr),
      process_(process),
      routing_id_(routing_id),
      is_loading_(false),
      is_hidden_(hidden),
      repaint_ack_pending_(false),
      resize_ack_pending_(false),
      color_profile_out_of_date_(false),
      auto_resize_enabled_(false),
      waiting_for_screen_rects_ack_(false),
      needs_repainting_on_restore_(false),
      is_unresponsive_(false),
      in_flight_event_count_(0),
      in_get_backing_store_(false),
      ignore_input_events_(false),
      text_direction_updated_(false),
      text_direction_(blink::WebTextDirectionLeftToRight),
      text_direction_canceled_(false),
      suppress_next_char_events_(false),
      pending_mouse_lock_request_(false),
      allow_privileged_mouse_lock_(false),
      has_touch_handler_(false),
      is_in_gesture_scroll_(false),
      received_paint_after_load_(false),
      next_browser_snapshot_id_(1),
      owned_by_render_frame_host_(false),
      is_focused_(false),
      scale_input_to_viewport_(IsUseZoomForDSFEnabled()),
      hung_renderer_delay_(
          base::TimeDelta::FromMilliseconds(kHungRendererDelayMs)),
      new_content_rendering_delay_(
          base::TimeDelta::FromMilliseconds(kNewContentRenderingDelayMs)),
      mouse_wheel_coalesce_timer_(new base::ElapsedTimer()),
      weak_factory_(this) {
  CHECK(delegate_);
  CHECK_NE(MSG_ROUTING_NONE, routing_id_);

  std::pair<RoutingIDWidgetMap::iterator, bool> result =
      g_routing_id_widget_map.Get().insert(std::make_pair(
          RenderWidgetHostID(process->GetID(), routing_id_), this));
  CHECK(result.second) << "Inserting a duplicate item!";
  process_->AddRoute(routing_id_, this);

  if (!hidden)
    process_->WidgetRestored();

  latency_tracker_.Initialize(routing_id_, GetProcess()->GetID());

  input_router_.reset(new InputRouterImpl(
      process_, this, this, routing_id_, GetInputRouterConfigForPlatform()));

  touch_emulator_.reset();

  if (!base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableHangMonitor)) {
    hang_monitor_timeout_.reset(new TimeoutMonitor(
        base::Bind(&RenderWidgetHostImpl::RendererIsUnresponsive,
                   weak_factory_.GetWeakPtr())));
  }

  new_content_rendering_timeout_.reset(new TimeoutMonitor(
      base::Bind(&RenderWidgetHostImpl::ClearDisplayedGraphics,
                 weak_factory_.GetWeakPtr())));

  delegate_->RenderWidgetCreated(this);
}
