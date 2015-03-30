// utils/
#include "utils/Buffer_test.h"
#include "utils/string_test.h"
#include "utils/queue_test.h"
#include "utils/hashmap_test.h"
#include "utils/RWLock_test.h"
#include "utils/Barrier_test.h"
#include "utils/ConfigParser_test.h"
#include "utils/shell_test.h"
#include "utils/file_test.h"

// core/
#include "core/common_test.h"
#include "core/Message_test.h"
#include "core/BasicChannel_test.h"
#include "core/AsynExec_test.h"
#include "core/system/master/terminate_test.h"

// third
#include "third/eigen_test.h"

// transfer
#include "core/transfer/Route_test.h"
#include "core/transfer/transfer_test.h"

// parameter
#include "core/parameter/sparsetable_test.h"
#include "core/parameter/sparse_access_method_test.h"

// hashfrag
#include "core/parameter/hashfrag_test.h"

int main(int argc, char **argv) {  

    testing::InitGoogleTest(&argc, argv);  
    return RUN_ALL_TESTS();  
} 
