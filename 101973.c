void SVGElement::UpdateRelativeLengthsInformation(
    bool client_has_relative_lengths,
    SVGElement* client_element) {
  DCHECK(client_element);

  for (Node& current_node : NodeTraversal::InclusiveAncestorsOf(*this)) {
    if (!current_node.isConnected())
      return;
  }

  for (Node& current_node : NodeTraversal::InclusiveAncestorsOf(*this)) {
    if (!current_node.IsSVGElement())
      break;
    SVGElement& current_element = ToSVGElement(current_node);
#if DCHECK_IS_ON()
    DCHECK(!current_element.in_relative_length_clients_invalidation_);
#endif

    bool had_relative_lengths = current_element.HasRelativeLengths();
    if (client_has_relative_lengths)
      current_element.elements_with_relative_lengths_.insert(client_element);
    else
      current_element.elements_with_relative_lengths_.erase(client_element);

    if (had_relative_lengths == current_element.HasRelativeLengths())
      return;

    client_element = &current_element;
    client_has_relative_lengths = client_element->HasRelativeLengths();
  }

  if (auto* svg = ToSVGSVGElementOrNull(*client_element)) {
    SVGDocumentExtensions& svg_extensions = GetDocument().AccessSVGExtensions();
    if (client_element->HasRelativeLengths())
      svg_extensions.AddSVGRootWithRelativeLengthDescendents(svg);
    else
      svg_extensions.RemoveSVGRootWithRelativeLengthDescendents(svg);
  }
}
