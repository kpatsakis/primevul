  void Compute(OpKernelContext *ctx) override {
    const Tensor *a_indices_t, *a_values_t, *a_shape_t, *b_indices_t,
        *b_values_t, *b_shape_t;
    OP_REQUIRES_OK(ctx, ctx->input("a_indices", &a_indices_t));
    OP_REQUIRES_OK(ctx, ctx->input("a_values", &a_values_t));
    OP_REQUIRES_OK(ctx, ctx->input("a_shape", &a_shape_t));
    OP_REQUIRES_OK(ctx, ctx->input("b_indices", &b_indices_t));
    OP_REQUIRES_OK(ctx, ctx->input("b_values", &b_values_t));
    OP_REQUIRES_OK(ctx, ctx->input("b_shape", &b_shape_t));

    // Validations.
    OP_REQUIRES(
        ctx,
        TensorShapeUtils::IsMatrix(a_indices_t->shape()) &&
            TensorShapeUtils::IsMatrix(b_indices_t->shape()),
        errors::InvalidArgument("Inputs a_indices and b_indices should be "
                                "matrices but received shapes: ",
                                a_indices_t->shape().DebugString(), ", ",
                                b_indices_t->shape().DebugString()));
    OP_REQUIRES(ctx,
                TensorShapeUtils::IsVector(a_values_t->shape()) &&
                    TensorShapeUtils::IsVector(b_values_t->shape()),
                errors::InvalidArgument(
                    "Inputs a_values and b_values should be vectors "
                    "but received shapes: ",
                    a_values_t->shape().DebugString(), " and ",
                    b_values_t->shape().DebugString()));

    const int64 a_nnz = a_indices_t->dim_size(0);
    const int64 b_nnz = b_indices_t->dim_size(0);
    const auto a_values = a_values_t->vec<T>();
    const auto b_values = b_values_t->vec<T>();

    OP_REQUIRES(
        ctx, a_values.size() == a_nnz && b_values.size() == b_nnz,
        errors::InvalidArgument("Expected ", a_nnz, " and ", b_nnz,
                                " non-empty input values, got ",
                                a_values.size(), " and ", b_values.size()));

    OP_REQUIRES(ctx,
                TensorShapeUtils::IsVector(a_shape_t->shape()) &&
                    TensorShapeUtils::IsVector(b_shape_t->shape()),
                errors::InvalidArgument(
                    "Input shapes should be a vector but received shapes ",
                    a_shape_t->shape().DebugString(), " and ",
                    b_shape_t->shape().DebugString()));
    OP_REQUIRES(ctx, a_shape_t->IsSameSize(*b_shape_t),
                errors::InvalidArgument(
                    "Operands do not have the same ranks; got shapes: ",
                    a_shape_t->SummarizeValue(10), " and ",
                    b_shape_t->SummarizeValue(10)));
    const auto a_shape = a_shape_t->flat<int64>();
    const auto b_shape = b_shape_t->flat<int64>();
    for (int i = 0; i < a_shape_t->NumElements(); ++i) {
      OP_REQUIRES(ctx, a_shape(i) == b_shape(i),
                  errors::InvalidArgument("Operands' shapes do not match: got ",
                                          a_shape(i), " and ", b_shape(i),
                                          " for dimension ", i));
    }

    const int num_dims = a_indices_t->dim_size(1);
    const auto a_indices_mat = a_indices_t->matrix<int64>();
    const auto b_indices_mat = b_indices_t->matrix<int64>();
    std::vector<T> a_augmented_values, b_augmented_values;
    std::vector<std::pair<bool, int64>> entries_to_copy;  // from_a?, idx
    UnionSparseIndicesAndValues(a_indices_mat, a_values, a_nnz, b_indices_mat,
                                b_values, b_nnz, num_dims, &a_augmented_values,
                                &b_augmented_values, &entries_to_copy);

    // Allocates and fills output tensors.
    const int64 sum_nnz = a_augmented_values.size();
    Tensor *output_indices_t, *output_values_t;
    OP_REQUIRES_OK(ctx,
                   ctx->allocate_output(0, TensorShape({sum_nnz, num_dims}),
                                        &output_indices_t));
    OP_REQUIRES_OK(
        ctx, ctx->allocate_output(1, TensorShape({sum_nnz}), &output_values_t));
    auto output_indices_mat = output_indices_t->matrix<int64>();

    for (int64 i = 0; i < sum_nnz; ++i) {
      const bool from_a = entries_to_copy[i].first;
      const int64 idx = entries_to_copy[i].second;
      output_indices_mat.chip<0>(i) =
          from_a ? a_indices_mat.chip<0>(idx) : b_indices_mat.chip<0>(idx);
    }

    // Performs the functor operation using Eigen.
    //
    // Note that the two stack-allocated std::vector's may not be aligned. Using
    // allocate_temp() would've given us aligned storage, but we do not know
    // their sizes in advance, so we couldn't use allocate_temp() anyway.
    //
    // TODO(zongheng): measure if it's worthwhile to somehow force alignment.
    using UnalignedTensorMap =
        Eigen::TensorMap<Eigen::Tensor<const T, 1, Eigen::RowMajor>,
                         Eigen::Unaligned>;
    auto a_augmented_values_t =
        UnalignedTensorMap(a_augmented_values.data(), sum_nnz);
    auto b_augmented_values_t =
        UnalignedTensorMap(b_augmented_values.data(), sum_nnz);
    output_values_t->flat<T>().device(ctx->eigen_device<Device>()) =
        a_augmented_values_t.binaryExpr(b_augmented_values_t,
                                        typename Functor::func());
  }