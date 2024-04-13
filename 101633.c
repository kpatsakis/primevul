  static void CreateLoaderAndStart(
      RenderFrameHost* frame,
      int route_id,
      int request_id,
      const network::ResourceRequest& resource_request) {
    network::mojom::URLLoaderPtr loader;
    network::TestURLLoaderClient client;
    CreateLoaderAndStart(frame, mojo::MakeRequest(&loader), route_id,
                         request_id, resource_request,
                         client.CreateInterfacePtr().PassInterface());
  }
