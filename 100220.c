        virtual ~PendingCharactersCallback() 
        {
            xmlFree(s);
        }
