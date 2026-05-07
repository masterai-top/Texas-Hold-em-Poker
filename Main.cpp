#include <cstdlib>
#include <iostream>
#include <time.h>
#include <string>
#include "ShareMem.hpp"

using namespace std;

static void share_memory_test() {
    ShareMem sm;
    //
    sm.Init();
    sm.Create(STAGE_FLOP);
    sm.Create(STAGE_TURN);
    sm.Create(STAGE_RIVER);
    //
    sm.TestConfig();
    //
    COUT << "------------- verify -------------" << ENDL;
    sm.TestCard("2s5s", true);
    sm.TestCard("2s5s6s8sTs", true);
    sm.TestCard("2s3s6s9sTs", true);
    sm.TestCard("2s5s6s8sTsKs", true);
    sm.TestCard("2s5s6s8sTsKsAs", true);
    sm.TestCard("2s3s5s8sTsKsAs", true);
    sm.TestCard("2s3s5s8sQsKsAs", true);
}

int main(int argc, char **argv) {
    share_memory_test();
    return 0;
}
