#pragma once

#include <common.h>

typedef struct {
    s32 itemId;
    s32 weight;
} DropItem;

typedef struct {
    char * name;
    s32 tribeId;
    s32 dropItemChance;
    DropItem * items;
    s32 itemCnt;
} NPCData;

typedef struct {
    char * name;
    s32 itemId;
    s32 sellValue;
} ItemData;

extern char * shopTypes[];
extern NPCData npcData[];
extern ItemData itemDataTable[];
extern ItemData yoldTownShopOverrides[];