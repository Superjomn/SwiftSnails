#include <climits>
#include "../../utils/file.h"
#include "gtest/gtest.h"
using namespace std;
using namespace swift_snails;

TEST(file, scan_file_by_line) {
    FILE* file = fopen("./utils/1.txt", "r");
    std::atomic<int> line_no{0};
    std::mutex file_mut;

    std::function<void(const string &line)> task = [&line_no](const string &line) {
        LOG(INFO) << "get line\t" << line;
        line_no ++;

    };
    scan_file_by_line(
        file, 
        file_mut, 
        std::move(task)
        );

    fclose(file);

    ASSERT_EQ(line_no, 3);
}
