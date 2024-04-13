void RenderProcessHostImpl::RegisterMojoInterfaces() {
  auto registry = std::make_unique<service_manager::BinderRegistry>();

  channel_->AddAssociatedInterfaceForIOThread(
      base::Bind(&IndexedDBDispatcherHost::AddBinding,
                 base::Unretained(indexed_db_factory_.get())));

  AddUIThreadInterface(
      registry.get(), base::Bind(&ForwardRequest<device::mojom::BatteryMonitor>,
                                 device::mojom::kServiceName));

  AddUIThreadInterface(
      registry.get(),
      base::Bind(&RenderProcessHostImpl::CreateOffscreenCanvasProvider,
                 base::Unretained(this)));

  AddUIThreadInterface(registry.get(),
                       base::Bind(&RenderProcessHostImpl::BindFrameSinkProvider,
                                  base::Unretained(this)));

  AddUIThreadInterface(
      registry.get(),
      base::Bind(&RenderProcessHostImpl::BindCompositingModeReporter,
                 base::Unretained(this)));

  AddUIThreadInterface(
      registry.get(),
      base::Bind(&RenderProcessHostImpl::BindSharedBitmapAllocationNotifier,
                 base::Unretained(this)));

  AddUIThreadInterface(
      registry.get(),
      base::Bind(&BackgroundSyncContext::CreateService,
                 base::Unretained(
                     storage_partition_impl_->GetBackgroundSyncContext())));
  AddUIThreadInterface(
      registry.get(),
      base::Bind(&RenderProcessHostImpl::CreateStoragePartitionService,
                 base::Unretained(this)));
  AddUIThreadInterface(
      registry.get(),
      base::Bind(&BroadcastChannelProvider::Connect,
                 base::Unretained(
                     storage_partition_impl_->GetBroadcastChannelProvider())));
  if (base::FeatureList::IsEnabled(features::kMemoryCoordinator)) {
    AddUIThreadInterface(
        registry.get(), base::Bind(&CreateMemoryCoordinatorHandle, GetID()));
  }
  if (resource_coordinator::IsResourceCoordinatorEnabled()) {
    AddUIThreadInterface(
        registry.get(),
        base::Bind(&CreateProcessResourceCoordinator, base::Unretained(this)));
  }

  BrowserContext* browser_context = GetBrowserContext();
  scoped_refptr<ChromeBlobStorageContext> blob_storage_context =
      ChromeBlobStorageContext::GetFor(browser_context);

  AddUIThreadInterface(
      registry.get(),
      base::Bind(&ClipboardHostImpl::Create, std::move(blob_storage_context)));

  media::VideoDecodePerfHistory* video_perf_history =
      GetBrowserContext()->GetVideoDecodePerfHistory();
  AddUIThreadInterface(
      registry.get(),
      base::BindRepeating(&media::VideoDecodePerfHistory::BindRequest,
                          base::Unretained(video_perf_history)));

  registry->AddInterface(
      base::Bind(&MimeRegistryImpl::Create),
      base::CreateSequencedTaskRunnerWithTraits(
          {base::MayBlock(), base::TaskShutdownBehavior::SKIP_ON_SHUTDOWN,
           base::TaskPriority::USER_BLOCKING}));
#if BUILDFLAG(USE_MINIKIN_HYPHENATION)
  registry->AddInterface(base::Bind(&hyphenation::HyphenationImpl::Create),
                         hyphenation::HyphenationImpl::GetTaskRunner());
#endif

  registry->AddInterface(base::Bind(&device::GamepadHapticsManager::Create));

  registry->AddInterface(base::Bind(&device::GamepadMonitor::Create));

  registry->AddInterface(
      base::Bind(&PushMessagingManager::BindRequest,
                 base::Unretained(push_messaging_manager_.get())));

  if (gpu_client_) {
    registry->AddInterface(
        base::Bind(&GpuClient::Add, base::Unretained(gpu_client_.get())));
  }

  registry->AddInterface(
      base::Bind(
          &WebDatabaseHostImpl::Create, GetID(),
          base::WrapRefCounted(storage_partition_impl_->GetDatabaseTracker())),
      storage_partition_impl_->GetDatabaseTracker()->task_runner());

  MediaStreamManager* media_stream_manager =
      BrowserMainLoop::GetInstance()->media_stream_manager();

  registry->AddInterface(
      base::Bind(&VideoCaptureHost::Create, GetID(), media_stream_manager));

  registry->AddInterface(
      base::Bind(&FileUtilitiesHostImpl::Create, GetID()),
      base::CreateSequencedTaskRunnerWithTraits(
          {base::MayBlock(), base::TaskPriority::USER_VISIBLE}));

#if BUILDFLAG(ENABLE_WEBRTC)
  registry->AddInterface(
      base::Bind(&RenderProcessHostImpl::CreateMediaStreamDispatcherHost,
                 base::Unretained(this), media_stream_manager));
#endif

  registry->AddInterface(
      base::Bind(&metrics::CreateSingleSampleMetricsProvider));

  registry->AddInterface(base::Bind(
      QuotaDispatcherHost::Create, GetID(),
      base::RetainedRef(storage_partition_impl_->GetQuotaManager()),
      base::WrapRefCounted(
          GetContentClient()->browser()->CreateQuotaPermissionContext())));

  registry->AddInterface(
      base::Bind(&CreateReportingServiceProxy, storage_partition_impl_));

  registry->AddInterface(base::BindRepeating(
      &AppCacheDispatcherHost::Create,
      base::Unretained(storage_partition_impl_->GetAppCacheService()),
      GetID()));

  AddUIThreadInterface(registry.get(), base::Bind(&FieldTrialRecorder::Create));

  associated_interfaces_.reset(new AssociatedInterfaceRegistryImpl());
  GetContentClient()->browser()->ExposeInterfacesToRenderer(
      registry.get(), associated_interfaces_.get(), this);
  blink::AssociatedInterfaceRegistry* associated_registry =
      associated_interfaces_.get();
  associated_registry->AddInterface(base::Bind(
      &RenderProcessHostImpl::BindRouteProvider, base::Unretained(this)));
  associated_registry->AddInterface(base::Bind(
      &RenderProcessHostImpl::CreateRendererHost, base::Unretained(this)));

  if (base::FeatureList::IsEnabled(features::kNetworkService)) {
    AddUIThreadInterface(
        registry.get(),
        base::Bind(&RenderProcessHostImpl::CreateURLLoaderFactory,
                   base::Unretained(this)));
  }

  registry->AddInterface(
      base::BindRepeating(&BlobRegistryWrapper::Bind,
                          storage_partition_impl_->GetBlobRegistry(), GetID()));

#if BUILDFLAG(ENABLE_LIBRARY_CDMS)
  registry->AddInterface(base::BindRepeating(&KeySystemSupportImpl::Create));
#endif  // BUILDFLAG(ENABLE_LIBRARY_CDMS)

  ServiceManagerConnection* service_manager_connection =
      BrowserContext::GetServiceManagerConnectionFor(browser_context_);
  std::unique_ptr<ConnectionFilterImpl> connection_filter(
      new ConnectionFilterImpl(child_connection_->child_identity(),
                               std::move(registry)));
  connection_filter_controller_ = connection_filter->controller();
  connection_filter_id_ = service_manager_connection->AddConnectionFilter(
      std::move(connection_filter));
}
