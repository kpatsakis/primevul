        virtual ~PendingCDATABlockCallback() 
        {
            xmlFree(s);
        }
