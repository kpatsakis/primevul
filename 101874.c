inline void QuicStreamSequencerBuffer::UpdateGapList(
    std::list<Gap>::iterator gap_with_new_data_written,
    QuicStreamOffset start_offset,
    size_t bytes_written) {
  if (gap_with_new_data_written->begin_offset == start_offset &&
      gap_with_new_data_written->end_offset > start_offset + bytes_written) {
    gap_with_new_data_written->begin_offset = start_offset + bytes_written;
  } else if (gap_with_new_data_written->begin_offset < start_offset &&
             gap_with_new_data_written->end_offset ==
                 start_offset + bytes_written) {
    gap_with_new_data_written->end_offset = start_offset;
  } else if (gap_with_new_data_written->begin_offset < start_offset &&
             gap_with_new_data_written->end_offset >
                 start_offset + bytes_written) {
    auto current = gap_with_new_data_written++;
    QuicStreamOffset current_end = current->end_offset;
    current->end_offset = start_offset;
    gaps_.insert(gap_with_new_data_written,
                 Gap(start_offset + bytes_written, current_end));
  } else if (gap_with_new_data_written->begin_offset == start_offset &&
             gap_with_new_data_written->end_offset ==
                 start_offset + bytes_written) {
    gaps_.erase(gap_with_new_data_written);
  }
}
