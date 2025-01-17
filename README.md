-------------------Important notice-----------------------------------------
This software was created with a specific database structure.
If you do not wish to use this structure, you will have to change the queries in the program yourself.
It is possible that not all columns are referenced or used.

It contains four tables with columns:

-Devices
---Device_ID (Int)
---Device_Name (String)
---Serial_Tage (String)
---Device_Model_ID (Foreign key "Device_Model_ID" from Device_Models, Int)
---Currently_Issued_To (Foreign key "Employee_ID" from Employees, Int)
---In_Service_Date (String)

-Device Models
---Device_Model_ID (Int)
---Device_Model_Name (String)
---Device_Model_Cost (String)
---Device_Model_Type (float)

-Employees
---Employee_ID (Int)
---Employee_Name (String)
---Employee_Title (String)
---Location_ID (Foreign key "Location_ID" from Locations, Int)
---Employee_Email (String, *Not a key, but used like one in the code, so it must be unique)
---Employee_Phone_Number (String)
---Employee_Extension (Int)

-Locations
---Location ID (Int)
---Location_Name (String)
---Location_Address (String)
---Location_Phone (String)

-----------------------How to use-------------------------------------------
Other than the executable itself, the program requires a config file (config.cfg). The file consists of two lines,
each one is an exact connection string (meaning no 'string =', just the connection string).

The first line is for whatever test server you have, and the second is for a production server. If you don't know
your connection string it can be found reasonably easily online. Currently, the program has only been tested with 
OBDC 17 and using Windows auth. It may take some tweaking to get right.
