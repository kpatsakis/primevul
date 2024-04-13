account_type_from_pwent (struct passwd *pwent)
{
        struct group *grp;
        gint i;

        if (pwent->pw_uid == 0) {
                g_debug ("user is root so account type is administrator");
                return ACCOUNT_TYPE_ADMINISTRATOR;
        }

        grp = getgrnam (ADMIN_GROUP);
        if (grp == NULL) {
                g_debug (ADMIN_GROUP " group not found");
                return ACCOUNT_TYPE_STANDARD;
        }

        for (i = 0; grp->gr_mem[i] != NULL; i++) {
                if (g_strcmp0 (grp->gr_mem[i], pwent->pw_name) == 0) {
                        return ACCOUNT_TYPE_ADMINISTRATOR;
                }
        }

        return ACCOUNT_TYPE_STANDARD;
}
