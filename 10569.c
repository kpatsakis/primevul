void CallInfo::print() {
  ResourceMark rm;
  const char* kindstr;
  switch (_call_kind) {
  case direct_call: kindstr = "direct";  break;
  case vtable_call: kindstr = "vtable";  break;
  case itable_call: kindstr = "itable";  break;
  default         : kindstr = "unknown"; break;
  }
  tty->print_cr("Call %s@%d %s", kindstr, _call_index,
                _resolved_method.is_null() ? "(none)" : _resolved_method->name_and_sig_as_C_string());
}