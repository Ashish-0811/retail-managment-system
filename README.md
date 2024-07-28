Retail Management System

This Retail Management System is a C++ console application designed to manage retail store inventory and sales efficiently. The system includes features for adding, updating, deleting, and displaying items, as well as recording and managing sales. It also features a secure login mechanism for accessing administrative functions.



Features

Secure Login System:

Admin login with a predefined username and password.
Secure password input (hidden characters).


Inventory Management:

Add New Items: Add new items to the inventory with details such as item code, name, company name, quantity, price, and expiration date.
Display Items: View a list of all items in the inventory with their details.
Update Items: Modify the details of existing items.
Delete Items: Remove items from the inventory.


Sales Management:

Record Sales: Record a new sale by entering the sale ID, item code, quantity sold, and sold price.
Display Sales: View a list of all sales recorded with their details.
Check Specific Sale: Retrieve details of a specific sale by entering the sale ID.
Update Sales: Modify the details of an existing sale.
Delete Sales: Remove a sale record.


Validations
Input Validation: Ensures that user inputs for item codes, quantities, prices, and other numerical values are valid.
Existence Checks: Verifies the existence of items before recording sales to prevent inconsistencies.


File Handling
Data Persistence: All items and sales data are stored in text files (item.txt and sales.txt), ensuring data persistence between sessions.
Temporary Files: Uses temporary files to safely update or delete records without corrupting the main files.
