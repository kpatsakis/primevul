PPVarFromNPObject::~PPVarFromNPObject() {
  Var::PluginReleasePPVar(var_);
}
