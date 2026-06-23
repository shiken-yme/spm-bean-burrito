#include <bean_burrito.h>
#include <common.h>
#include <main.h>
#include <npcdata.h>
#include <spm_system.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool npcChkBlacklisted(s32 tribeId, s32 index) {
    for (s32 i = 0; i < gp->tribeBlacklistCnt; i += 1) {
        if (tribeId == gp->tribeBlacklist[i])
            return true;
    }
    for (s32 i = 0; i < gp->enemyIdxBlacklistCnt; i += 1) {
        if (index == (gp->enemyIdxBlacklist[i] - 1)) // adjust for 1-index
            return true;
    }
    return false;
}

s32 npcGetDropItem(NPCData * data, s32 index) {
    if (data->dropItemChance == 0)
        return 0;
    if (irand(99) >= data->dropItemChance)
        return 0;
    if (data->items == NULL)
        return 0;
    // Calculate total weight and determine item
    s32 totalWt = 0;
    for (s32 i = 0; i < data->itemCnt; i += 1)
        totalWt += data->items[i].weight;
    s32 rolledWt = irand(totalWt - 1);
    // Blacklist check should happen AFTER second irand call
    if (npcChkBlacklisted(data->tribeId, index) == true) {
        return 0;
    } else {
        s32 itemIdx;
        for (itemIdx = 0; itemIdx < data->itemCnt; itemIdx += 1) {
            rolledWt -= data->items[itemIdx].weight;
            if (rolledWt < 0)
                break;
        }
        /*s32 item = data->items[itemIdx].itemId;
        s32 sv = 0;
        if (gp->shopType == 2) {
            for (s32 i = 0; yoldTownShopOverrides[i].itemId != -1; i += 1) {
                if (yoldTownShopOverrides[i].itemId == item) {
                    sv = yoldTownShopOverrides[i].sellValue;
                    gp->curIterationItemList[gp->curIterationItemCnt++] = yoldTownShopOverrides[i].name;
                }
            }
        }
        // Todo: support for Crag shop
        if (sv == 0) {
            for (s32 i = 0; itemDataTable[i].itemId != -1; i += 1) {
                if (itemDataTable[i].itemId == item) {
                    sv = itemDataTable[i].sellValue;
                    gp->curIterationItemList[gp->curIterationItemCnt++] = itemDataTable[i].name;
                }
            }
        }
        return sv;*/
        return data->items[itemIdx].itemId;
    }
}

NPCData * npcGetData(s32 tribeId) {
    for (s32 i = 0; npcData[i].name != NULL; i += 1) {
        if (tribeId == npcData[i].tribeId)
            return &npcData[i];
    }
    assertf(false, "NPC data ptr for tribe %d not found", tribeId);
    return NULL;
}

s32 itemGetSellValue(s32 item) {
    s32 sv = 0;
    if (gp->shopType == 2) {
        for (s32 i = 0; yoldTownShopOverrides[i].itemId != -1; i += 1) {
            if (yoldTownShopOverrides[i].itemId == item) {
                sv = yoldTownShopOverrides[i].sellValue;
            }
        }
    }
    // Todo: support for Crag shop
    if (sv == 0) {
        for (s32 i = 0; itemDataTable[i].itemId != -1; i += 1) {
            if (itemDataTable[i].itemId == item) {
                sv = itemDataTable[i].sellValue;
            }
        }
    }
    return sv;
}

s32 getLowestSv(s32 * list, s32 listSize, s32 * index) {
    s32 lowestSv = list[0];
    *index = 0;
    for (s32 i = 0; i < listSize; i += 1) {
        if (list[i] < lowestSv) {
            lowestSv = list[i];
            *index = i;
        }
    }
    return lowestSv;
}

s32 mapCalcSellValue() {
    s32 sellValue = 0, tribeId = 0, svList[10];
    for (s32 i = 0; i < gp->enemyListCnt; i += 1) {
        tribeId = gp->enemyList[i];
        s32 itemId = npcGetDropItem(npcGetData(tribeId), i);
        if (itemId != 0) {
            gp->curIterationItemList[gp->curIterationItemCnt++] = itemId;
            // printf("Iteration %d: item index %d is ID %d\n", gp->iterationNum, gp->curIterationItemCnt, gp->curIterationItemList[gp->curIterationItemCnt]);
        }
    }
    for (s32 i = 0; i < gp->curIterationItemCnt; i += 1) {
        if (i >= 10) {
            s32 lowestSvIdx;
            s32 lowestSv = getLowestSv(svList, 10, &lowestSvIdx);
            if (lowestSv > itemGetSellValue(gp->curIterationItemList[i])) {
                gp->curIterationItemList[lowestSvIdx] = gp->curIterationItemList[i];
                svList[lowestSvIdx] = itemGetSellValue(gp->curIterationItemList[i]);
            }
            gp->curIterationItemList[i] = 0;
        } else {
            svList[i] = itemGetSellValue(gp->curIterationItemList[i]);
        }
    }
    if (gp->curIterationItemCnt > 10)
        gp->curIterationItemCnt = 10;
    for (s32 i = 0; i < gp->curIterationItemCnt; i += 1) {
        sellValue += svList[i];
    }
    return sellValue;
}

char * itemGetName(s32 itemId) {
    for (s32 i = 0; itemDataTable[i].name != NULL; i += 1) {
        if (itemId == itemDataTable[i].itemId)
            return itemDataTable[i].name;
    }
    assertf(false, "Item name for ID %d not found", itemId);
    return NULL;
}

bool beanBurrito() {
    gp->seed += 1;
    gp->iterationNum += 1;
    gp->curIterationItemCnt = 0;
    RANDOM_SEED = gp->seed;
    s32 sellValue = mapCalcSellValue();
    if (gp->sellValueThreshold == -1) {
        if (sellValue >= gp->highestSv) {
            gp->seedsFound += 1;
            gp->highestSv = sellValue;
            gp->highestSvIteration = gp->iterationNum;
            gp->highestSvSeed = gp->seed;
            fprintf(gp->outfile, "Seed 0x%X has sell value of %d coins. Items (%d): ", gp->highestSvSeed, gp->highestSv, gp->curIterationItemCnt);
            for (s32 i = 0; i < gp->curIterationItemCnt; i += 1) {
                if (i != (gp->curIterationItemCnt - 1))
                    fprintf(gp->outfile, "%s, ", itemGetName(gp->curIterationItemList[i]));
                else
                    fprintf(gp->outfile, "%s\n", itemGetName(gp->curIterationItemList[i]));
            }
        }
        if ((gp->iterationNum % 10000000U) == 0) {
            printf("\r%u seeds processed... Seed 0x%X has highest sell value of %d coins.", ((u32)(gp->iterationNum / 10000000U) * 10000000U), gp->highestSvSeed, gp->highestSv);
            fflush(stdout);
        }
    } else {
        if (sellValue >= gp->sellValueThreshold) {
            gp->seedsFound += 1;
            fprintf(gp->outfile, "Seed 0x%X has sell value of %d coins. Items (%d): ", gp->seed, sellValue, gp->curIterationItemCnt);
            for (s32 i = 0; i < gp->curIterationItemCnt; i += 1) {
                if (i != (gp->curIterationItemCnt - 1))
                    fprintf(gp->outfile, "%s, ", itemGetName(gp->curIterationItemList[i]));
                else
                    fprintf(gp->outfile, "%s\n", itemGetName(gp->curIterationItemList[i]));
            }
        }
        if ((gp->iterationNum % 10000000U) == 0) {
            printf("\r%u seeds processed...", ((u32)(gp->iterationNum / 10000000U) * 10000000U));
            fflush(stdout);
        }
    }
    if (gp->seed == 0)
        return true;
    return false;
}
