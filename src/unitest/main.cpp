// utils/
#include "utils/Buffer_test.h"
#include "utils/string_test.h"
#include "utils/queue_test.h"
#include "utils/hashmap_test.h"
#include "utils/RWLock_test.h"

// core/
#include "core/common_test.h"
#include "core/Message_test.h"
#include "core/BasicChannel_test.h"
#include "core/AsynExec_test.h"
#include "core/transfer/Route_test.h"
#include "core/transfer/transfer_test.h"
//#include "core/transfer/Sender_test.h"
//#include "core/transfer/Receiver_test.h"
//#include "core/transfer/Transfer_test.h"
// a sender send a message to a receiver
//#include "core/transfer/test.h"

int main(int argc, char **argv) {  

    testing::InitGoogleTest(&argc, argv);  
    return RUN_ALL_TESTS();  
} 
