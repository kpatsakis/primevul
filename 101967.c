static bool HasLoadListener(Element* element) {
  if (element->HasEventListeners(event_type_names::kLoad))
    return true;

  for (element = element->ParentOrShadowHostElement(); element;
       element = element->ParentOrShadowHostElement()) {
    EventListenerVector* entry =
        element->GetEventListeners(event_type_names::kLoad);
    if (!entry)
      continue;
    for (wtf_size_t i = 0; i < entry->size(); ++i) {
      if (entry->at(i).Capture())
        return true;
    }
  }

  return false;
}
