# ChallengePacs2
Second challenge of the 2024 Pacs Course 

WARNINGS: 
-If the main is altered to test with other matrices, please, update the number of nonzero elements manually with the public method void set_nnz(unsigned n), otherwise the code might behave unexpectedly after elements insersion.

-Everything is implemented inside the algebra namespace, so, if the user intends to create a new main file, the command "using namespace algebra" is heavily suggested.

CONTENTS:

- Matrix.hpp
This file contains the declarations of the members for the Matrix template class and its specialization for column ordered matrices, accompanied by a new comparator struct called CompareKeysCSC.
To distinguish between row ordered matrices and column ordered ones I defined an enumerated class StorageOrder, which allows the user to build a matrix as Matrix<T,StorageOrder::Rows> or Matrix<T,StorageOrder::Columns>. The default case considers row ordered matrices, if not specified otherwise.

-Matrix_impl.hpp
This file contains the implementation for all the members and friends of the Matrix template class.

-Matrix_csc_impl
This file contains the implementation for all the members and friends of the template class specialization Matrix<T,StorageOrder::Columns>.

-mmio.hpp:
Matrix market I/O library from the math.nist.gov website

-mmio.cpp:
Definitions for the mmio library.

-main.cpp:
File that tests the implementation. It also includes the utility chrono that allows the user to time the implementation and print the elapsed time. In this case it was used to time the matrix vector product in the uncompressed case and compressed case.
There is also an example of the implementation applied to complex numbers.