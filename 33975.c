nsPluginInstance::getDocumentProp(const std::string& propname) const
{
    std::string rv;

    if (!HasScripting()) {
        LOG_ONCE( gnash::log_debug("Browser doesn't support scripting") );
        return rv;
    }

    NPObject* windowobj;
    NPError err = NPN_GetValue(_instance, NPNVWindowNPObject, &windowobj);
    if (err != NPERR_NO_ERROR || !windowobj) {
        return rv;
    }

    boost::shared_ptr<NPObject> window_obj(windowobj, NPN_ReleaseObject);
  
    NPIdentifier doc_id = NPN_GetStringIdentifier("document");

    NPVariant docvar;
    if(! NPN_GetProperty(_instance, windowobj, doc_id, &docvar) ) {
        return rv;
    }

    boost::shared_ptr<NPVariant> doc_var(&docvar, NPN_ReleaseVariantValue);

    if (!NPVARIANT_IS_OBJECT(docvar)) {
        return rv;
    }

    NPObject* doc_obj = NPVARIANT_TO_OBJECT(docvar);
    
    NPIdentifier prop_id = NPN_GetStringIdentifier(propname.c_str());

    NPVariant propvar;
    if (!NPN_GetProperty(_instance, doc_obj, prop_id, &propvar)) {
        return rv;
    }

    boost::shared_ptr<NPVariant> prop_var(&propvar, NPN_ReleaseVariantValue);

    if (!NPVARIANT_IS_STRING(propvar)) {
        return rv;
    }

    const NPString& prop_str = NPVARIANT_TO_STRING(propvar);

    rv = NPStringToString(prop_str);
    return rv;
}
