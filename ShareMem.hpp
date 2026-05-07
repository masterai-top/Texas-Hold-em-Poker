#ifndef _SHARE_MEM_H_
#define _SHARE_MEM_H_

#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <assert.h>
#include <pwd.h>
#include <stdio.h>
#include <errno.h>
#include <cstring>
#include <pthread.h>
#include <fcntl.h>
#include <filesystem>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <string>
#include <cstddef>
#include <NumCpp.hpp>
#include <fstream>
#include "HandIndex.hpp"

using namespace std;

//@决定是否使用新聚类训练
#define HAND_NEW_CLUS 1

//
#define SHM_FLOP_PATH  "/dev/shm/flop.shm"
#define SHM_TURN_PATH  "/dev/shm/turn.shm"
#define SHM_RIVER_PATH "/dev/shm/river.shm"
//
#ifndef HAND_NEW_CLUS
#define FILE_FLOP_PATH  "/data/numpy_data/txt/flop.txt"
#define FILE_TURN_PATH  "/data/numpy_data/txt/turn.txt"
#define FILE_RIVER_PATH "/data/numpy_data/txt/river.txt"
#else
#define FILE_FLOP_PATH  "/data/numpy_data/csv/flop.csv"
#define FILE_TURN_PATH  "/data/numpy_data/csv/turn.csv"
#define FILE_RIVER_PATH "/data/numpy_data/csv/river.csv"
#endif

//
#define COUT std::cout
#define ENDL std::endl
#define CERR std::cerr
//
typedef enum eStage {
    STAGE_FLOP  = 0,
    STAGE_TURN  = 1,
    STAGE_RIVER = 2,
} Stage;
//
class ShareMem {
public:
    //
    ShareMem(bool isNewHandClus = true);
    //
    ~ShareMem();
    //
    void Create(int flag);
    //
    void Destroy(int flag);
    //
    void CreateFile(char *fileName);
    //
    int GetValue(int64_t index, int flag);
    //
    hand_index_t GetIndex(std::string cardString);
    //
    hand_index_t GetIndex2(std::string cardString);
    //
    void Init();
    //
    void Final();
    //
    void TestConfig();
    //
    int TestCard(std::string cardString, bool isOutput = false);

private:
    //
    int FindRankCard(char chr);
    //
    int FindSuitCard(char chr);

private:
    //
    int64_t preFlopMaxIndex;
    //
    int64_t flopMaxIndex;
    //
    int64_t turnMaxIndex;
    //
    int64_t riverMaxIndex;

private:
    //
    uint8_t *shmAddr;
    //
    uint8_t *flopShmAddr;
    //
    uint8_t *turnShmAddr;
    //
    uint8_t *riverShmAddr;

private:
    //
    hand_indexer_t preFlopIndexer;
    //
    hand_indexer_t flopIndexer;
    //
    hand_indexer_t turnIndexer;
    //
    hand_indexer_t riverIndexer;

private:
    //
    bool newHandClus;

};

#endif
