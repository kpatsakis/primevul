        virtual ~PendingErrorCallback() 
        {
            free (message);
        }
