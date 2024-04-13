void RenderFrameImpl::didCreateDataSource(blink::WebLocalFrame* frame,
                                          blink::WebDataSource* datasource) {
  DCHECK(!frame_ || frame_ == frame);

  render_view_->didCreateDataSource(frame, datasource);

  scoped_ptr<ServiceWorkerNetworkProvider>
      network_provider(new ServiceWorkerNetworkProvider());
  ServiceWorkerNetworkProvider::AttachToDocumentState(
      DocumentState::FromDataSource(datasource),
      network_provider.Pass());
}
