include(Testing)

add_executable(smrtptrs_test
unique_ptr_test.cpp
shared_ptr_test.cpp
weak_ptr_test.cpp
)

AddTests(smrtptrs_test)

