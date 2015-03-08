// utils/
#include "utils/Buffer_test.h"
#include "utils/string_test.h"
#include "utils/queue_test.h"

// core/
#include "core/Message_test.h"
#include "core/BasicChannel_test.h"
#include "core/AsynExec_test.h"

int main(int argc, char **argv) {  

    testing::InitGoogleTest(&argc, argv);  
    return RUN_ALL_TESTS();  
} 
