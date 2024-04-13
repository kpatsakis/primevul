NGConstraintSpace NGColumnLayoutAlgorithm::CreateConstraintSpaceForColumns(
    const LogicalSize& column_size,
    bool separate_leading_margins) const {
  NGConstraintSpaceBuilder space_builder(
      ConstraintSpace(), Style().GetWritingMode(), /* is_new_fc */ true);
  space_builder.SetAvailableSize(column_size);
  space_builder.SetPercentageResolutionSize(column_size);

  if (NGBaseline::ShouldPropagateBaselines(Node()))
    space_builder.AddBaselineRequests(ConstraintSpace().BaselineRequests());

  LayoutUnit column_block_size =
      std::max(column_size.block_size, LayoutUnit(1));

  space_builder.SetFragmentationType(kFragmentColumn);
  space_builder.SetFragmentainerBlockSize(column_block_size);
  space_builder.SetFragmentainerSpaceAtBfcStart(column_block_size);
  space_builder.SetIsAnonymous(true);
  space_builder.SetSeparateLeadingFragmentainerMargins(
      separate_leading_margins);

  return space_builder.ToConstraintSpace();
}
