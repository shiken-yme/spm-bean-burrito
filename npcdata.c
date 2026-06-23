#include <common.h>
#include <spm_system.h>
#include <stdio.h>
#include <stdlib.h>
#include <npcdata.h>

ItemData itemDataTable[] = {
    {"Dried Shroom", 83, 1},
    {"Cake Mix", 105, 13},
    {"Big Egg", 118, 13},
    {"Honey Jar", 116, 10},
    {"Shroom Shake", 80, 15},
    {"Catch Card", 87, 10},
    {"Peachy Peach", 100, 13},
    {"Keel Mango", 101, 10},
    {"Fresh Veggie", 114, 10},
    {"Horsetail", 107, 9},
    {"Fire Burst", 65, 10},
    {"Power Steak", 117, 18},
    {"Hot Sauce", 109, 10},
    {"Smelly Herb", 115, 8},
    {"Ice Storm", 66, 20},
    {NULL, -1, -1}
};

ItemData yoldTownShopOverrides[] = {
    {"Fire Burst", 65, 0x4},
    {"Ice Storm", 66, 0x1E},
    {"Thunder Rage", 67, 0x64},
    {"Shooting Star", 68, 0x96},
    {"POW Block", 69, 0x1E},
    {"Shroom Shake", 80, 0x6},
    {"Long-Last Shake", 85, 0x8},
    {"Life Shroom", 84, 0x19},
    {"Sleepy Sheep", 78, 0xD},
    {"Shell Shock", 70, 0x4},
    {"Mighty Tonic", 75, 0x19},
    {"Courage Shell", 74, 0xD},
    {"Volt Shroom", 76, 0xA},
    {"Gold Bar", 71, 0x78},
    {"Gold Bar x3", 72, 0x177},
    {NULL, -1, -1}
};

DropItem goombaDropItems[] = {
    {83, 100},
    {105, 100},
    {118, 100},
    {116, 100},
    {80, 200},
    {87, 50}
};

DropItem paragoombaDropItems[] = {
    {100, 100},
    {101, 100},
    {114, 100},
    {107, 100},
    {80, 200},
    {87, 50}
};

DropItem squigletDropItems[] = {
    {100, 100},
    {80, 100},
    {114, 100},
    {107, 100},
    {65, 400},
    {87, 50}
};

DropItem squigDropItems[] = {
    {117, 100},
    {80, 100},
    {109, 100},
    {115, 100},
    {66, 400},
    {87, 50}
};

DropItem piranhaPlantDropItems[] = {
    {100, 100},
    {101, 100},
    {114, 100},
    {107, 100},
    {87, 40}
};

NPCData npcData[] = {
    {"Goomba", 0, 4, goombaDropItems, 6},
    {"Paragoomba", 7, 3, paragoombaDropItems, 6},
    {"Squiglet", 125, 3, squigletDropItems, 6},
    {"Squig", 126, 3, squigDropItems, 6},
    {"Piranha Plant", 77, 10, piranhaPlantDropItems, 5},
    {NULL, 0, 0, NULL, 0}
};

char * shopTypes[3] = {"Flipside/Flopside", "Yold Town", "Downtown of Crag"};