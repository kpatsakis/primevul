void AutoFillManager::ParseForms(
    const std::vector<webkit_glue::FormData>& forms) {
  for (std::vector<FormData>::const_iterator iter =
           forms.begin();
       iter != forms.end(); ++iter) {
    FormStructure* form_structure = new FormStructure(*iter);
    if (!form_structure->ShouldBeParsed())
      continue;

    DeterminePossibleFieldTypes(form_structure);
    form_structures_.push_back(form_structure);
  }

  if (!form_structures_.empty())
    download_manager_.StartQueryRequest(form_structures_);
}
