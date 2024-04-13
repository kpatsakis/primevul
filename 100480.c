void ChromotingInstance::DidChangeView(const pp::View& view) {
  DCHECK(plugin_task_runner_->BelongsToCurrentThread());

  view_->SetView(view);

  mouse_input_filter_.set_input_size(view_->get_view_size_dips());
}
