static struct node* acquire_or_create_child_locked(
 struct fuse* fuse, struct node* parent,
 const char* name, const char* actual_name)
{
 struct node* child = lookup_child_by_name_locked(parent, name);
 if (child) {
        acquire_node_locked(child);
 } else {
        child = create_node_locked(fuse, parent, name, actual_name);
 }
 return child;
}
