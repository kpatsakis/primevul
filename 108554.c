  void Compute(OpKernelContext* context) override {
    const Tensor& tensor_in = context->input(0);
    const Tensor& grad_in = context->input(1);
    const Tensor& argmax = context->input(2);

    PoolParameters params{context,
                          ksize_,
                          stride_,
                          padding_,
                          /*explicit_paddings=*/{},
                          FORMAT_NHWC,
                          tensor_in.shape()};
    if (!context->status().ok()) {
      return;
    }

    TensorShape out_shape({params.tensor_in_batch, params.tensor_in_rows,
                           params.tensor_in_cols, params.depth});
    Tensor* grad_out = nullptr;
    OP_REQUIRES_OK(context, context->forward_input_or_allocate_output(
                                {0}, 0, out_shape, &grad_out));

    LaunchMaxPoolingGradWithArgmax<Device, T>::launch(
        context, params, grad_in, argmax, grad_out, include_batch_in_index_);
  }