static bool isDebuggable() {
 char debuggable[PROP_VALUE_MAX];
    property_get("ro.debuggable", debuggable, "0");
 if (strcmp(debuggable, "1") == 0) {
 return true;
 }
 return false;
}
