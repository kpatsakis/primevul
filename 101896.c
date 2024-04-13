ScriptProcessorNode* ScriptProcessorNode::Create(
    BaseAudioContext& context,
    size_t buffer_size,
    unsigned number_of_input_channels,
    unsigned number_of_output_channels,
    ExceptionState& exception_state) {
  DCHECK(IsMainThread());

  if (context.IsContextClosed()) {
    context.ThrowExceptionForClosedState(exception_state);
    return nullptr;
  }

  if (number_of_input_channels == 0 && number_of_output_channels == 0) {
    exception_state.ThrowDOMException(
        kIndexSizeError,
        "number of input channels and output channels cannot both be zero.");
    return nullptr;
  }

  if (number_of_input_channels > BaseAudioContext::MaxNumberOfChannels()) {
    exception_state.ThrowDOMException(
        kIndexSizeError,
        "number of input channels (" +
            String::Number(number_of_input_channels) + ") exceeds maximum (" +
            String::Number(BaseAudioContext::MaxNumberOfChannels()) + ").");
    return nullptr;
  }

  if (number_of_output_channels > BaseAudioContext::MaxNumberOfChannels()) {
    exception_state.ThrowDOMException(
        kIndexSizeError,
        "number of output channels (" +
            String::Number(number_of_output_channels) + ") exceeds maximum (" +
            String::Number(BaseAudioContext::MaxNumberOfChannels()) + ").");
    return nullptr;
  }

  switch (buffer_size) {
    case 0:
      buffer_size =
          context.HasRealtimeConstraint()
              ? ChooseBufferSize(context.destination()->CallbackBufferSize())
              : 256;
      break;
    case 256:
    case 512:
    case 1024:
    case 2048:
    case 4096:
    case 8192:
    case 16384:
      break;
    default:
      exception_state.ThrowDOMException(
          kIndexSizeError,
          "buffer size (" + String::Number(buffer_size) +
              ") must be 0 or a power of two between 256 and 16384.");
      return nullptr;
  }

  ScriptProcessorNode* node = new ScriptProcessorNode(
      context, context.sampleRate(), buffer_size, number_of_input_channels,
      number_of_output_channels);

  if (!node)
    return nullptr;

  context.NotifySourceNodeStartedProcessing(node);

  return node;
}
