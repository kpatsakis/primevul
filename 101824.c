  void OnCallback(mojom::PdfCompositor::Status status,
                  mojo::ScopedSharedBufferHandle handle) {
    if (status == mojom::PdfCompositor::Status::SUCCESS)
      CallbackOnSuccess(handle.get());
    else
      CallbackOnError(status);
    run_loop_->Quit();
  }
