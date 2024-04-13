void ChildProcessSecurityPolicyImpl::AddWorker(int child_id,
                                               int main_render_process_id) {
  base::AutoLock lock(lock_);
  AddChild(child_id);
  worker_map_[child_id] = main_render_process_id;
}
