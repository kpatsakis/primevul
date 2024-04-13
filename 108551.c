  void Compute(OpKernelContext* context) override {
    INDEX_TYPE first_dimension;
    OP_REQUIRES_OK(context, GetFirstDimensionSize(context, &first_dimension));
    vector<INDEX_TYPE> output_size;
    OP_REQUIRES_OK(context,
                   CalculateOutputSize(first_dimension, context, &output_size));
    vector<INDEX_TYPE> multiplier;
    multiplier.resize(ragged_rank_ + 1);

    multiplier[multiplier.size() - 1] = 1;
    for (int i = multiplier.size() - 2; i >= 0; --i) {
      multiplier[i] = multiplier[i + 1] * output_size[i + 1];
    }
    // Full size of the tensor.
    TensorShape output_shape;
    OP_REQUIRES_OK(context,
                   TensorShapeUtils::MakeShape(output_size, &output_shape));
    Tensor* output_tensor = nullptr;

    OP_REQUIRES_OK(context,
                   context->allocate_output(0, output_shape, &output_tensor));
    const INDEX_TYPE full_size = multiplier[0] * output_size[0];
    if (full_size > 0) {
      vector<INDEX_TYPE> output_index, new_output_index;
      int nvals = context->input(kValueInputIndex).shape().dim_size(0);
      output_index.reserve(nvals);
      new_output_index.reserve(nvals);

      CalculateFirstParentOutputIndex(first_dimension, multiplier[0],
                                      output_size[0], &output_index);
      for (int i = 1; i <= ragged_rank_; ++i) {
        OP_REQUIRES_OK(context, CalculateOutputIndex(
                                    context, i - 1, output_index, multiplier[i],
                                    output_size[i], &new_output_index));
        output_index.swap(new_output_index);
        new_output_index.clear();
      }

      SetOutput(context, ragged_rank_, output_index, output_tensor);
    }
  }