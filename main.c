#include <bean_burrito.h>
#include <common.h>
#include <main.h>
#include <math.h>
#include <npcdata.h>
#include <spm_system.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_MAX 1024

SFGlobals sfGlobals;
SFGlobals * gp = &sfGlobals;

void trim(char * input) {
    for (s32 i = 0; i < strlen(input); i += 1) {
        if (input[i] == ',')
            input[i + 1] = '\n';
    }
    return;
}

bool strChkZero(char * input) {
    if (strcmp(input, "0\n") == 0)
        return true;
    return false;
}

void takeInput(char * input, char * save) {
    fgets(input, INPUT_MAX, stdin);
    if (save != NULL && strChkZero(input) == false) {
        strcpy(save, input);
        // printf("Debug: save = %s", save);
    }
    return;
}

s32 npcNameToTribeId(char * name) {
    for (s32 i = 0; npcData[i].name != NULL; i += 1) {
        if (strcmp(name, npcData[i].name) == 0)
            return npcData[i].tribeId;
    }
    assertf(false, "\"%s\" failed to return a valid tribe ID", name);
    return -1;
}

void createTribeList(char * input, s32 * list, s32 * counter) {
    trim(input);
    char * curNpc = strtok(input, ",\n");
    for (s32 i = 0; curNpc != NULL; i += 1) {
        s32 tribeId = atoi(curNpc);
        if (tribeId == 0 && strcmp(curNpc, "0") != 0)
            tribeId = npcNameToTribeId(curNpc);
        list[i] = tribeId;
        *counter += 1;
        // printf("%d\n", tribeId);
        curNpc = strtok(NULL, ",\n");
    }
    return;
}

void createGenericList(char * input, s32 * list, s32 * counter) {
    trim(input);
    char * curIn = strtok(input, ",\n");
    for (s32 i = 0; curIn != NULL; i += 1) {
        s32 result = atoi(curIn);
        list[i] = result;
        *counter += 1;
        curIn = strtok(NULL, ",\n");
    }
    return;
}

bool procMainMenu(char * mInput) {
    printf("\nEnter a number to view information about a setting and modify it. Enter 0 to execute the seed finder.\n");
    printf("[1] Enemy List (Length: %d)\n", gp->enemyListCnt);
    printf("[2] Tribe Blacklist (Length: %d)\n", gp->tribeBlacklistCnt);
    printf("[3] Index Blacklist (Length: %d)\n", gp->enemyIdxBlacklistCnt);
    printf("[4] Sell Value Report Threshold (Value: %d)\n", gp->sellValueThreshold);
    printf("[5] Shop Type (Currently: %s)\n", shopTypes[gp->shopType - 1]);
    printf("[6] Starting RNG Seed (Currently: %d)\n", gp->seed);
    printf("[0] Execute Seed Finder\n\nInput: ");
    takeInput(mInput, NULL);
    s32 opId = atoi(mInput);
    assertf(opId >= 0 && opId <= 6, "%d is not a valid operation ID", opId);
    memset(mInput, 0, INPUT_MAX);
    switch (opId) {
    case 0:
        return true;
        break;
    case 1:
        printf("\nEnemy List: A list of enemies in the exact order in which they spawn in your desired map.\n");
        printf("Enter a list of NPC names or Tribe IDs. Enemy names are case-sensitive. Separate everything with a comma.\nEnter 0 to exit without changes.\n\nInput: ");
        takeInput(mInput, gp->sInput[0]);
        if (strChkZero(mInput) == true)
            break;
        memset(gp->enemyList, 0, sizeof(gp->enemyList));
        gp->enemyListCnt = 0;
        createTribeList(mInput, gp->enemyList, &gp->enemyListCnt);
        return false;
        break;
    case 2:
        printf("\nTribe Blacklist: A list of tribes to ignore from sell value calculations.\n");
        printf("Enter a list of NPC names or Tribe IDs. Enemy names are case-sensitive. Separate everything with a comma.\nEnter 0 to exit without changes.\n\nInput: ");
        takeInput(mInput, gp->sInput[1]);
        if (strChkZero(mInput) == true)
            break;
        memset(gp->tribeBlacklist, 0, sizeof(gp->tribeBlacklist));
        gp->tribeBlacklistCnt = 0;
        createTribeList(mInput, gp->tribeBlacklist, &gp->tribeBlacklistCnt);
        return false;
        break;
    case 3:
        printf("\nIndex Blacklist: A list of enemy indices in your enemy list to ignore from sell value calculations.\n");
        printf("Enter a list of indices. 1-indexed. Separate everything with a comma.\nEnter 0 to exit without changes.\n\nInput: ");
        takeInput(mInput, gp->sInput[2]);
        if (strChkZero(mInput) == true)
            break;
        memset(gp->enemyIdxBlacklist, 0, sizeof(gp->enemyIdxBlacklist));
        gp->enemyIdxBlacklistCnt = 0;
        createGenericList(mInput, gp->enemyIdxBlacklist, &gp->enemyIdxBlacklistCnt);
        return false;
        break;
    case 4:
        printf("\nSell Value Report Threshold: Desired sell value for a seed to reach before being printed to console.\n");
        printf("Enter any number above 0. I would recommend something relatively high.\nEnter 0 to exit without changes. Enter -1 to ONLY report seeds with highest sell value.\n\nInput: ");
        takeInput(mInput, gp->sInput[3]);
        if (strChkZero(mInput) == true)
            break;
        gp->sellValueThreshold = atoi(mInput);
        return false;
        break;
    case 5:
        printf("\nShop Type: Shop at which all items found in a seed will be sold.\n");
        printf("Enter a number 1-3. (1 is Flipside/Flopside, 2 is Yold Town, 3 is Downtown of Crag)\nEnter 0 to exit without changes.\n\nInput: ");
        takeInput(mInput, gp->sInput[4]);
        if (strChkZero(mInput) == true)
            break;
        gp->shopType = atoi(mInput);
        assertf(gp->shopType >= 1 && gp->shopType <= 3, "%d is not a valid shop type ID", gp->shopType);
        return false;
        break;
    case 6:
        printf("\nStarting RNG Seed: Program ends once seed wraps back around to 0. Starts here.\n");
        printf("Enter a number.\n\nInput: ");
        takeInput(mInput, gp->sInput[5]);
        gp->seed = atoi(mInput);
        return false;
        break;
    }
    return false;
}

s32 main() {
    char outfileName[64];
    char input[INPUT_MAX];
    gp->sellValueThreshold = 50;
    gp->shopType = 1;
    sprintf(gp->sInput[1], "Null\n"); // Tribe Blacklist
    sprintf(gp->sInput[2], "Null\n"); // Index Blacklist
    sprintf(gp->sInput[3], "50\n"); // SV Thresh
    sprintf(gp->sInput[4], "Flipside/Flopside\n"); // Shop Type
    sprintf(gp->sInput[3], "0\n"); // Starting RNG Seed
    printf("Enter a list of NPC names or Tribe IDs in the exact order in which they spawn.\nEnemy names are case-sensitive. Separate everything with a comma.\n\nInput: ");
    takeInput(input, gp->sInput[0]);
    createTribeList(input, gp->enemyList, &gp->enemyListCnt);
    bool exec = false;
    do {
        memset(input, 0, INPUT_MAX);
        exec = procMainMenu(input);
    } while (!exec);
    printf("Enter a output filename. I'll automatically include \".txt\" for you.\n\nInput: ");
    takeInput(input, NULL);
    strcpy(outfileName, strtok(input, "\n"));
    sprintf(outfileName, "%s.txt", input);
    gp->outfile = fopen(outfileName, "w");
    fprintf(gp->outfile, "== Bean Burrito: SPM Seed Finder by Yme ==\nEnemy List: %sTribe Blacklist: %sIndex Blacklist: %sSell Value Threshold: %sShop Type: %s\n== SEEDS ==\n\n", gp->sInput[0], gp->sInput[1], gp->sInput[2], gp->sInput[3], gp->sInput[4], gp->sInput[5]);
    fclose(gp->outfile);
    gp->outfile = fopen(outfileName, "a");
    fflush(stdout);
    do {
        exec = beanBurrito();
    } while (!exec);
    fprintf(gp->outfile, "\n== FOUND %u SEEDS ==", gp->seedsFound);
    fclose(gp->outfile);
    printf("\n\nSeed finder is finished! %u total matching seeds found.", gp->seedsFound);
    return 0;
}
