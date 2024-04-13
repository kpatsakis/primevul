static bool winbind_name_list_to_sid_string_list(struct pwb_context *ctx,
						 const char *user,
						 const char *name_list,
						 char *sid_list_buffer,
						 int sid_list_buffer_size)
{
	bool result = false;
	char *current_name = NULL;
	const char *search_location;
	const char *comma;

	if (sid_list_buffer_size > 0) {
		sid_list_buffer[0] = 0;
	}

	search_location = name_list;
	while ((comma = strstr(search_location, ",")) != NULL) {
		current_name = strndup(search_location,
				       comma - search_location);
		if (NULL == current_name) {
			goto out;
		}

		if (!winbind_name_to_sid_string(ctx, user,
						current_name,
						sid_list_buffer,
						sid_list_buffer_size)) {
			goto out;
		}

		SAFE_FREE(current_name);

		if (!safe_append_string(sid_list_buffer, ",",
					sid_list_buffer_size)) {
			goto out;
		}

		search_location = comma + 1;
	}

	if (!winbind_name_to_sid_string(ctx, user, search_location,
					sid_list_buffer,
					sid_list_buffer_size)) {
		goto out;
	}

	result = true;

out:
	SAFE_FREE(current_name);
	return result;
}