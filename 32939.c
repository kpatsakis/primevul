account_type_from_pwent (struct passwd *pwent)
{
        struct group *grp;
        gid_t wheel;
        gid_t *groups;
        gint ngroups;
        gint i;

        if (pwent->pw_uid == 0) {
                g_debug ("user is root so account type is administrator");
                return ACCOUNT_TYPE_ADMINISTRATOR;
        }

        grp = getgrnam ("wheel");
        if (grp == NULL) {
                g_debug ("wheel group not found");
                return ACCOUNT_TYPE_STANDARD;
        }
        wheel = grp->gr_gid;

        ngroups = get_user_groups (pwent->pw_name, pwent->pw_gid, &groups);

        for (i = 0; i < ngroups; i++) {
                if (groups[i] == wheel) {
                        g_free (groups);
                        return ACCOUNT_TYPE_ADMINISTRATOR;
                }
        }

        g_free (groups);

        return ACCOUNT_TYPE_STANDARD;
}
