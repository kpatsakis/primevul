void AudioNode::disconnect(AudioParam* destination_param,
                           unsigned output_index,
                           ExceptionState& exception_state) {
  DCHECK(IsMainThread());
  BaseAudioContext::GraphAutoLocker locker(context());

  if (output_index >= Handler().NumberOfOutputs()) {
    exception_state.ThrowDOMException(
        kIndexSizeError,
        ExceptionMessages::IndexOutsideRange(
            "output index", output_index, 0u,
            ExceptionMessages::kInclusiveBound, numberOfOutputs() - 1,
            ExceptionMessages::kInclusiveBound));
    return;
  }

  if (!DisconnectFromOutputIfConnected(output_index, *destination_param)) {
    exception_state.ThrowDOMException(
        kInvalidAccessError,
        "specified destination AudioParam and node output (" +
            String::Number(output_index) + ") are not connected.");
    return;
  }
}
