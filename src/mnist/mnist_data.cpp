#include <cstring>
#include <fstream>

#include "mnist_data.h"

int labSig = 0x00000801;
int imgSig = 0x00000803;

inline int File_ReadLE32(FILE *stream)
{
	unsigned char bytes[4];

	fread(bytes, 4, 1, stream);

	return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
}

int loadDatabase(LABELSET *labset, const char *path)
{
    FILE *fp = fopen(path, "rb");
    if(fp == NULL)
        return -1;

    labset->sig = File_ReadLE32(fp);
    //fread(&labset->sig, 4, 1, fp);
    if(labset->sig != labSig) //check to see if it's a label file
        return -2;

    labset->count = File_ReadLE32(fp);
    labset->data = new unsigned char[labset->count];
    fread(labset->data, 1, labset->count, fp);

    return 0;
}

int loadDatabase(IMAGESET *imgset, const char *path)
{
    FILE *fp = fopen(path, "rb");
    if(fp == NULL)
        return -1;

    imgset->sig = File_ReadLE32(fp);
    //fread(&imgset->sig, 4, 1, fp);
    if(imgset->sig != imgSig) //check to see if it's a label file
        return -2;

    imgset->count = File_ReadLE32(fp);
    imgset->pRows = File_ReadLE32(fp);
    imgset->pCols = File_ReadLE32(fp);

    imgset->data = new unsigned char[imgset->count*imgset->pRows*imgset->pCols];
    fread(imgset->data, 1, imgset->count*imgset->pRows*imgset->pCols, fp);

    return 0;
}