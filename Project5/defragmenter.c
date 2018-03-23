
#include"defragmenter.h"
void dfs(int *i_point, FILE *fin, int *blk, int layer, FILE *fout, Superblock* sp, int n_blocks)
{
    //读完一个inode的所有块就返回
    if(*blk==n_blocks)return;
    //读出当前所指的块
    assert(fseek(fin,1024+(*i_point+sp->data_offset)*sp->size,SEEK_SET)==0);
    assert(fread(buf,sp->size,1,fin)==1);
    int i;
    //当读到的为数据块时,写到fout中
    if(layer==0){
        assert(fseek(fout,1024+blocks*sp->size,SEEK_SET)==0);
        assert(fwrite(buf,sp->size,1,fout)==1);
        *i_point=blocks-sp->data_offset;
        blocks++;
        (*blk)++;
        return;
    }
    //读到的为索引
    if(layer>0){
    assert(fseek(fout,1024+blocks*sp->size,SEEK_SET)==0);
    *i_point=blocks-sp->data_offset;
    blocks++;

    int * p_array=(int *)malloc(sp->size);
    memcpy(p_array,buf,sp->size);
    for(i=0;i<sp->size/sizeof(int);i++)
    {
        dfs(&p_array[i], fin, blk, layer-1, fout, sp, n_blocks);
        if(*blk==n_blocks){i++;break;}
    }
    assert(fseek(fout,1024+(blocks-(i+1))*sp->size,SEEK_SET)==0);    
    assert(fwrite(p_array,sp->size,1,fout)==1);
    free(p_array);
    }
}
int main(int argc,char *argv[ ])
{
    FILE * f;
    FILE * df;
    char * buffer;
    //open disk file
    f = fopen(argv[1], "r");
    assert(f);
    //create defrag_file
    const char *str = "-defrag";
    const size_t len = strlen(argv[1])+strlen(str);
    char n_str[len+1];
    strcpy(n_str,argv[1]);
    strcat(n_str,str);
    df = fopen(n_str,"w");
    assert(df);

    //write boot block and read superblock
    Superblock sp;
    buffer = malloc(512);
    assert(fseek(f,0,SEEK_SET)==0);
    assert(fread(buffer,512,1,f)==1);
    assert(fwrite(buffer,512,1,df)==1);

    assert(fseek(f,512,SEEK_SET)==0);
    assert(fread(buffer,512,1,f)==1);
    memcpy(&sp,buffer,sizeof(Superblock));

    buf=malloc(sp.size);
    //insert blocks between inode and superblocks
    assert(fseek(f,1024,SEEK_SET)==0);
    if(sp.inode_offset>0){
        free(buffer);
        buffer=malloc(sp.inode_offset*sp.size);
        assert(fread(buffer,sp.inode_offset*sp.size,1,f)==1);
        assert(fwrite(buffer,sp.inode_offset*sp.size,1,df)==1);
    }
    assert(fseek(f,1024+sp.inode_offset*sp.size,SEEK_SET)==0);
    //read inodes array
    size_t size_inode=(sp.data_offset-sp.inode_offset)*sp.size;
    Inode * inodes;
    inodes=malloc(size_inode);
    assert(fread(inodes,size_inode,1,f)==1);

    int N_INODES=size_inode/sizeof(Inode);
    int i,j,k,n_blocks,blk=0;
    blocks=sp.data_offset;
    //deal with each inode
    for(i=0;i<N_INODES;i++)
    {   
        blk=0;
        if(inodes[i].nlink==0)
            continue;
        if(inodes[i].nlink>0){
            n_blocks=(int)ceil((float)inodes[i].size/sp.size);

        //direct blocks
        for(j=0;j<10;j++){
            assert(fseek(f,1024+(inodes[i].dblocks[j]+sp.data_offset)*sp.size,SEEK_SET)==0);
            assert(fread(buf,sp.size,1,f)==1);
            assert(fseek(df,1024+blocks*sp.size,SEEK_SET)==0);
            assert(fwrite(buf,sp.size,1,df)==1);
            inodes[i].dblocks[j]=blocks-sp.data_offset;
            blocks++;
            if(++blk == n_blocks)break;
            }
        if(blk == n_blocks)continue;
        //indirect blocks
        if(blk < n_blocks){
            for(k=0;k<4;k++){
                dfs(&(inodes[i].iblocks[k]), f, &blk, 1, df, &sp, n_blocks);
                if(blk==n_blocks)break;
                }
            }
        if(blk == n_blocks)continue;
        //i2block
        if(blk < n_blocks){
                dfs(&(inodes[i].i2block), f, &blk, 2, df, &sp, n_blocks);
            }
        if(blk == n_blocks)continue;
        //i3block
        if(blk < n_blocks){
                dfs(&(inodes[i].i3block), f, &blk, 3, df, &sp, n_blocks);
            }
        }
    }
    //write inodes to defrag-file
    assert(fseek(df,1024+sp.inode_offset*sp.size,SEEK_SET)==0);
    assert(fwrite(inodes,size_inode,1,df)==1);
    
    //deal with the free_iblock list
    int cnt=0;
    assert(fseek(f,1024,SEEK_SET)==0);
    while(fread(buf,sp.size,1,f)){
        cnt++;
    }
    sp.free_iblock=blocks-sp.data_offset;
    int next_free;
    for(i=blocks;i<cnt-1;i++){
        next_free=i+1-sp.data_offset;
        memcpy(buf,&next_free,sizeof(int));
        assert(fseek(df,1024+i*sp.size,SEEK_SET)==0);
        assert(fwrite(buf,sp.size,1,df)==1);
    }
    next_free=-1;
    memcpy(buf,&next_free,sizeof(int));
    assert(fseek(df,1024+i*sp.size,SEEK_SET)==0);
    assert(fwrite(buf,sp.size,1,df)==1);
    //write superblock to defrag-file
    memcpy(buf,&sp,sizeof(Superblock));
    assert(fseek(df,512,SEEK_SET)==0);
    assert(fwrite(buf,512,1,df)==1);
    
    //free all allocated memory and close files
    free(inodes);
    free(buffer);
    free(buf);
    fclose(f);
    fclose(df);
    return 0;
}
