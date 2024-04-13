void RenderWidgetHostImpl::SetInputTargetClient(
    viz::mojom::InputTargetClientPtr input_target_client) {
  input_target_client_ = std::move(input_target_client);
}
