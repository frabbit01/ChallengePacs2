# ChallengePacs2
Second challenge of the 2024 Pacs Course 

WARNINGS: 
- Upon insertion of a new element the code has the default option to increase the number of nonzero elements. 
If a zero element is inserted, the function "void set_nnz(unsigned n)", present in both the template class Matrix and its specialization, must be called, so that the private member of the class that stores the number of non-zero elements will be updated correctly.

-The utility Chrono is included in the main file. To allow the code to work properly, please include the correct path where it is stored in the Makefile. It might be different from the one that is already included, depending on where the folder is stored.