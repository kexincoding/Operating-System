#include"stdio.h"
#include"stdlib.h"
#include"assert.h"
#include"sys/types.h"
#include"sys/stat.h"
#include"fcntl.h"
#include"string.h"
#include"math.h"
#define N_DBLOCKS 10
#define N_IBLOCKS 4
typedef struct inode
{
    int next_inode;
    int protect;
    int nlink;
    int size;
    int uid;
    int gid;
    int ctime;
    int mtime;
    int atime;
    int dblocks[N_DBLOCKS];
    int iblocks[N_IBLOCKS];
    int i2block;
    int i3block;
} Inode;
typedef struct superblock
{
    int size;
    int inode_offset;
    int data_offset;
    int swap_offset;
    int free_inode;
    int free_iblock;
} Superblock;
int blocks;
char *buf;
void dfs(int *i_point, FILE *fin, int *blk, int layer, FILE *fout, Superblock* sp, int n_blocks);