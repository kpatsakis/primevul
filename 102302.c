 virtual int handleResponse(WifiEvent& reply) {

        ALOGV("In GetFeatureSetCommand::handleResponse");

 if (reply.get_cmd() != NL80211_CMD_VENDOR) {
            ALOGD("Ignoring reply with cmd = %d", reply.get_cmd());
 return NL_SKIP;
 }

 int id = reply.get_vendor_id();
 int subcmd = reply.get_vendor_subcmd();

        nlattr *vendor_data = reply.get_attribute(NL80211_ATTR_VENDOR_DATA);
 int len = reply.get_vendor_data_len();

        ALOGV("Id = %0x, subcmd = %d, len = %d", id, subcmd, len);
 if (vendor_data == NULL || len == 0) {
            ALOGE("no vendor data in GetFeatureSetCommand response; ignoring it");
 return NL_SKIP;
 }
 if(feature_type == FEATURE_SET) {
 void *data = reply.get_vendor_data();
 if(!fset) {
                ALOGE("Buffers pointers not set");
 return NL_SKIP;
 }
            memcpy(fset, data, min(len, (int) sizeof(*fset)));
 } else {
 int num_features_set = 0;
 int i = 0;

 if(!feature_matrix || !fm_size) {
                ALOGE("Buffers pointers not set");
 return NL_SKIP;
 }

 for (nl_iterator it(vendor_data); it.has_next(); it.next()) {
 if (it.get_type() == ANDR_WIFI_ATTRIBUTE_NUM_FEATURE_SET) {
                    num_features_set = it.get_u32();
                    ALOGV("Got feature list with %d concurrent sets", num_features_set);
 if(set_size_max && (num_features_set > set_size_max))
                        num_features_set = set_size_max;
 *fm_size = num_features_set;
 } else if ((it.get_type() == ANDR_WIFI_ATTRIBUTE_FEATURE_SET) &&
                             i < num_features_set) {
                    feature_matrix[i] = it.get_u32();
                    i++;
 } else {
                    ALOGW("Ignoring invalid attribute type = %d, size = %d",
                            it.get_type(), it.get_len());
 }
 }

 }
 return NL_OK;
 }
