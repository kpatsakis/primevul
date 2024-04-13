 void BasePrefsChange::DismissOnPrefChange(const std::string& pref_name) {
   DCHECK(!pref_observer_->IsObserved(pref_name));
   pref_observer_->AddPref(pref_name);
}
