bool TextCodecUTF8::HandlePartialSequence<LChar>(LChar*& destination,
                                                 const uint8_t*& source,
                                                 const uint8_t* end,
                                                 bool flush,
                                                 bool,
                                                 bool&) {
  DCHECK(partial_sequence_size_);
  do {
    if (IsASCII(partial_sequence_[0])) {
      *destination++ = partial_sequence_[0];
      ConsumePartialSequenceBytes(1);
      continue;
    }
    int count = NonASCIISequenceLength(partial_sequence_[0]);
    if (!count)
      return true;

    if (count > partial_sequence_size_) {
      if (count - partial_sequence_size_ > end - source) {
        if (!flush) {
          memcpy(partial_sequence_ + partial_sequence_size_, source,
                 end - source);
          partial_sequence_size_ += end - source;
          return false;
        }
        return true;
      }
      memcpy(partial_sequence_ + partial_sequence_size_, source,
             count - partial_sequence_size_);
      source += count - partial_sequence_size_;
      partial_sequence_size_ = count;
    }
    int character = DecodeNonASCIISequence(partial_sequence_, count);
    if (character & ~0xff)
      return true;

    partial_sequence_size_ -= count;
    *destination++ = static_cast<LChar>(character);
  } while (partial_sequence_size_);

  return false;
}
