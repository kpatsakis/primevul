void AudioNode::disconnect(AudioNode* destination,
                           ExceptionState& exception_state) {
  DCHECK(IsMainThread());
  BaseAudioContext::GraphAutoLocker locker(context());

  unsigned number_of_disconnections = 0;

  for (unsigned output_index = 0; output_index < numberOfOutputs();
       ++output_index) {
    for (unsigned input_index = 0;
         input_index < destination->Handler().NumberOfInputs(); ++input_index) {
      if (DisconnectFromOutputIfConnected(output_index, *destination,
                                          input_index))
        number_of_disconnections++;
    }
  }

  if (number_of_disconnections == 0) {
    exception_state.ThrowDOMException(
        kInvalidAccessError, "the given destination is not connected.");
    return;
  }
}
