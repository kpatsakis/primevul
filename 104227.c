static inline long object_common1(UNSERIALIZE_PARAMETER, zend_class_entry *ce)
{
	long elements;
	
	elements = parse_iv2((*p) + 2, p);

	(*p) += 2;
	
	/* The internal class check here is a BC fix only, userspace classes implementing the
	Serializable interface have eventually an inconsistent behavior at this place when
	unserialized from a manipulated string. Additionaly the interal classes can possibly
	crash PHP so they're still disabled here. */
	if (ce->serialize == NULL || ce->unserialize == zend_user_unserialize || (ZEND_INTERNAL_CLASS != ce->type && ce->create_object == NULL)) {
		object_init_ex(*rval, ce);
	} else {
		/* If this class implements Serializable, it should not land here but in object_custom(). The passed string
		obviously doesn't descend from the regular serializer. */
		zend_error(E_WARNING, "Erroneous data format for unserializing '%s'", ce->name);
		return 0;
	}

	return elements;
}
