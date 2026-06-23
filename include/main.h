#pragma once

#include <common.h>
#include <stdarg.h>
#include <stdio.h>

typedef struct {
    FILE * outfile;
    char sInput[6][512];
    s32 enemyList[100];
    s32 enemyIdxBlacklist[100];
    s32 tribeBlacklist[32];
    s32 curIterationItemList[20];
    s32 curIterationItemCnt;
    s32 enemyListCnt;
    s32 enemyIdxBlacklistCnt;
    s32 tribeBlacklistCnt;
    s32 sellValueThreshold;
    s32 shopType;
    u32 iterationNum;
    u32 seedsFound;
    s32 seed;
    s32 highestSv;
    s32 highestSvSeed;
    u32 highestSvIteration;
} SFGlobals;

extern SFGlobals * gp;
