#valgrind --tool=memcheck --leak-check=full ./bin/test
valgrind --tool=memcheck --leak-check=full ./bin/test --gtest_filter=Transfer*
#valgrind --tool=memcheck --leak-check=full ./bin/test --gtest_filter=transfer_send
