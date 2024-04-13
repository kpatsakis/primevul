void BaseAudioContext::PerformCleanupOnMainThread() {
  DCHECK(IsMainThread());
  GraphAutoLocker locker(this);

  if (is_resolving_resume_promises_) {
    for (auto& resolver : resume_resolvers_) {
      if (context_state_ == kClosed) {
        resolver->Reject(DOMException::Create(
            DOMExceptionCode::kInvalidStateError,
            "Cannot resume a context that has been closed"));
      } else {
        SetContextState(kRunning);
        resolver->Resolve();
      }
    }
    resume_resolvers_.clear();
    is_resolving_resume_promises_ = false;
  }

  if (active_source_nodes_.size()) {
    for (AudioNode* node : active_source_nodes_) {
      if (node->Handler().GetNodeType() ==
          AudioHandler::kNodeTypeAudioBufferSource) {
        AudioBufferSourceNode* source_node =
            static_cast<AudioBufferSourceNode*>(node);
        source_node->GetAudioBufferSourceHandler().HandleStoppableSourceNode();
      }
    }

    Vector<AudioHandler*> finished_handlers;
    {
      MutexLocker lock(finished_source_handlers_mutex_);
      finished_source_handlers_.swap(finished_handlers);
    }
    wtf_size_t remove_count = 0;
    Vector<bool> removables;
    removables.resize(active_source_nodes_.size());
    for (AudioHandler* handler : finished_handlers) {
      for (wtf_size_t i = 0; i < active_source_nodes_.size(); ++i) {
        if (handler == &active_source_nodes_[i]->Handler()) {
          handler->BreakConnectionWithLock();
          removables[i] = true;
          remove_count++;
          break;
        }
      }
    }

    if (remove_count > 0) {
      HeapVector<Member<AudioNode>> actives;
      DCHECK_GE(active_source_nodes_.size(), remove_count);
      wtf_size_t initial_capacity =
          std::min(active_source_nodes_.size() - remove_count,
                   active_source_nodes_.size());
      actives.ReserveInitialCapacity(initial_capacity);
      for (wtf_size_t i = 0; i < removables.size(); ++i) {
        if (!removables[i])
          actives.push_back(active_source_nodes_[i]);
      }
      active_source_nodes_.swap(actives);
    }
  }

  has_posted_cleanup_task_ = false;
}
