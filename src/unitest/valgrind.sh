#valgrind --tool=memcheck --leak-check=full ./bin/test
valgrind --tool=memcheck --leak-check=full ./bin/test --gtest_filter=TransferTest.send_receive_message
#valgrind --tool=memcheck --leak-check=full ./bin/test --gtest_filter=transfer_send
