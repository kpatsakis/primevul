static bool check_caller_access_to_name(struct fuse* fuse,
 const struct fuse_in_header *hdr, const struct node* parent_node,
 const char* name, int mode) {
 /* Always block security-sensitive files at root */
 if (parent_node && parent_node->perm == PERM_ROOT) {
 if (!strcasecmp(name, "autorun.inf")
 || !strcasecmp(name, ".android_secure")
 || !strcasecmp(name, "android_secure")) {
 return false;
 }
 }

 /* Root always has access; access for any other UIDs should always
     * be controlled through packages.list. */
 if (hdr->uid == 0) {
 return true;
 }

 /* No extra permissions to enforce */
 return true;
}
