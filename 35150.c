js_init() {
    /* This function creates the class and its definition, only once */
    if (!uzbl.js.initialized) {
        /* it would be pretty cool to make this dynamic */
        uzbl.js.classdef = kJSClassDefinitionEmpty;
        uzbl.js.classdef.staticFunctions = js_static_functions;

        uzbl.js.classref = JSClassCreate(&uzbl.js.classdef);
    }
}
