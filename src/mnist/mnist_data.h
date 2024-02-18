#pragma once

struct LABELSET
{
    int sig;
    int count;
    unsigned char *data;
};

struct IMAGESET
{
    int sig;
    int count;      //number of images
    int pRows;      //width in pixels of each image
    int pCols;      //height in pixels of each image
    unsigned char *data;
};

int loadDatabase(LABELSET *labset, const char *path);
int loadDatabase(IMAGESET *imgset, const char *path);