int TypedUrlModelAssociator::MergeUrls(
    const sync_pb::TypedUrlSpecifics& node,
    const history::URLRow& url,
    history::VisitVector* visits,
    history::URLRow* new_url,
    std::vector<history::VisitInfo>* new_visits) {
  DCHECK(new_url);
  DCHECK(!node.url().compare(url.url().spec()));
  DCHECK(!node.url().compare(new_url->url().spec()));
  DCHECK(visits->size());
  DCHECK_EQ(node.visits_size(), node.visit_transitions_size());

  if (node.visits_size() == 0)
    return DIFF_UPDATE_NODE;

  string16 node_title(UTF8ToUTF16(node.title()));
  base::Time node_last_visit = base::Time::FromInternalValue(
      node.visits(node.visits_size() - 1));

  int different = DIFF_NONE;

  if ((node_title.compare(url.title()) != 0) ||
      (node.hidden() != url.hidden())) {
    if (node_last_visit >= url.last_visit()) {
      new_url->set_title(node_title);
      new_url->set_hidden(node.hidden());
      different |= DIFF_LOCAL_ROW_CHANGED;

      if (new_url->title().compare(url.title()) != 0) {
        different |= DIFF_LOCAL_TITLE_CHANGED;
      }
    } else {
      new_url->set_title(url.title());
      new_url->set_hidden(url.hidden());
      different |= DIFF_UPDATE_NODE;
    }
  } else {
    new_url->set_title(url.title());
    new_url->set_hidden(url.hidden());
  }

  size_t node_num_visits = node.visits_size();
  size_t history_num_visits = visits->size();
  size_t node_visit_index = 0;
  size_t history_visit_index = 0;
  while (node_visit_index < node_num_visits ||
         history_visit_index < history_num_visits) {
    base::Time node_time, history_time;
    if (node_visit_index < node_num_visits)
      node_time = base::Time::FromInternalValue(node.visits(node_visit_index));
    if (history_visit_index < history_num_visits)
      history_time = (*visits)[history_visit_index].visit_time;
    if (node_visit_index >= node_num_visits ||
        (history_visit_index < history_num_visits &&
         node_time > history_time)) {
      different |= DIFF_UPDATE_NODE;
      ++history_visit_index;
    } else if (history_visit_index >= history_num_visits ||
               node_time < history_time) {
      different |= DIFF_LOCAL_VISITS_ADDED;
      new_visits->push_back(history::VisitInfo(
          node_time, node.visit_transitions(node_visit_index)));
      ++node_visit_index;
    } else {
      ++node_visit_index;
      ++history_visit_index;
    }
  }

  if (different & DIFF_LOCAL_VISITS_ADDED) {
    history::VisitVector::iterator visit_ix = visits->begin();
    for (std::vector<history::VisitInfo>::iterator new_visit =
             new_visits->begin();
         new_visit != new_visits->end(); ++new_visit) {
      while (visit_ix != visits->end() &&
             new_visit->first > visit_ix->visit_time) {
        ++visit_ix;
      }
      visit_ix = visits->insert(visit_ix,
                                history::VisitRow(url.id(), new_visit->first,
                                                  0, new_visit->second, 0));
      ++visit_ix;
    }
  }

  new_url->set_last_visit(visits->back().visit_time);
  return different;
}
