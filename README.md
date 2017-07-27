# pds_version2a
This version of pds (personal data store) focuses on efficient retrieval of data from a binary file by making use of indexing.
The file named pds_index.dat is used to store the <key,value> pair. The key is the contact_id of the customer and the value is the offset of the record in the pds_demo.dat file.
If we want to retrieve a particular record we first look for it in the index file. If the record is found we retrieve the offset of that record.
Using that offset we go into the data file and retrieve the original record that was needed.
