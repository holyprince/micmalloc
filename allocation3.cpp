

#include<stdio.h>
#include<stdlib.h>

#define BASIC_NUM 10
#define MALLOC_TYPE void
#define BASIC_BLOCK 4096
typedef struct Tree {
	struct Tree *lefttree;
	struct Tree *righttree;
	long highdataaddress;
	long lowdataaddress;
	void *superblockmem;
	void *datablockmem;
	int dataleft;
	int dataright;
	int leftstart;
	int leftend;
	int rightstart;
	int rightend;
	int leftrestnum;
	int rightrestnum;
	int datablocknum;
} HeapTree;

typedef struct Superblock {
	int datastatus[BASIC_NUM];//basicblock num
	long address[BASIC_NUM];
	int basicblocksize;
	int restblocknum;
	//void *startaddr;
	long startaddrhigh; //equal the init data mem
	MALLOC_TYPE *mem;
	int heapnum;
	Superblock *next;
	Superblock *pre;
}Superblock;



void* aligned_malloc(size_t required_bytes, size_t alignment){
    void* p1;
    void** p2;
    int offset = alignment - 1 + sizeof(void*);
    if ((p1 = (void*)malloc(required_bytes + offset)) == NULL){
        return NULL;
    }
    p2 = (void**)(((size_t)(p1)+offset) & ~(alignment - 1));
    p2[-1] = p1;
    return p2;
}

void aligned_free(void *p2){
    void* p1 = ((void**)p2)[-1];
    free(p1);
}
int calcbasicblocknum(int size)
{
	if(size%BASIC_BLOCK!=0)
		return size/BASIC_BLOCK+1;
	else
		return size/BASIC_BLOCK;
}
void printtree(HeapTree *root)
{
	HeapTree *queue=(HeapTree* )malloc(sizeof(HeapTree)*8192);
	int head=0;
	int tail=0;
	queue[head++]=*root;
	while(head!=tail)
	{
		HeapTree temp= queue[tail++];
		printf("data node %d %d \n",temp.dataleft,temp.dataright);
		if(temp.lefttree!=NULL)
			queue[head++]=*(temp.lefttree);
		if(temp.righttree!=NULL)
			queue[head++]=*(temp.righttree);
	}

}
void* realdivide(void* left,void * right)
{


}

void malloctree(HeapTree * &root,int num,int left,int right)
{
	if(root==NULL)
	{
		root=(HeapTree *)malloc(sizeof(HeapTree)*1);
		int wholerestnum=(right-left)+1;

		if(num<=wholerestnum/2)
		{
			int midblocknum=(wholerestnum-1)/2+left;
			root->dataleft=midblocknum;
			root->dataright=midblocknum+num-1;
			root->datablocknum=num;

			root->leftrestnum=midblocknum-left;

			if(midblocknum-1<left)
			{
				root->leftstart=-1;
				root->leftend=-1;
			}
			else
			{
				root->leftstart=left;
				root->leftend=midblocknum-1;
			}

/*			if(midblocknum-1<left)
				root->leftend=left;
			else*/
			if(root->dataright+1>right)
			{
				root->rightstart=-1;
				root->rightend=-1;
			}
			else
			{
				root->rightstart=root->dataright+1;
				root->rightend=right;
			}

			root->rightrestnum=right-root->dataright;
/*			if(root->dataright+1>=right)
				root->rightstart=right;*/

			root->lefttree=NULL;
			root->righttree=NULL;

		}
		else if(num<=wholerestnum)
		{
			root->dataleft=left;
			root->dataright=left+num-1;
			root->datablocknum=num;

			root->leftrestnum=0;
			root->leftstart=-1;
			root->leftend=-1;

			root->rightrestnum=wholerestnum-num;
			if(root->dataright+1>right)
			{
				root->rightstart=-1;
				root->rightend=-1;
			}
			else
			{
				root->rightstart=root->dataright+1;
				root->rightend=right;
			}
/*			if(root->dataright+1>=right)
				root->rightstart=right;
			else*/

			root->lefttree=NULL;
			root->righttree=NULL;
		}
		else
		{
			printf("Do not have enough space to allocate\n");
		}
	}
	else
	{

		if(root->leftrestnum>root->rightrestnum)
		{
			if(root->leftrestnum<num)
			{
				printf("Do not have enough space to allocate\n");
				return ;
			}
			malloctree(root->lefttree,num,root->leftstart,root->leftend);
			root->leftrestnum=root->leftrestnum-num;
		}
		else
		{
			if(root->rightrestnum<num)
			{
				printf("Do not have enough space to allocate\n");
				return ;
			}
			malloctree(root->righttree,num,root->rightstart,root->rightend);
			root->rightrestnum=root->rightrestnum-num;
		}
	}
}
void print_fulltree(HeapTree* tree, int key, int direction)
{
    if(tree != NULL)
    {
        if(direction==0)    // tree是根节点
        {
        	printf("%2d  %2d is root\n", tree->dataleft, tree->dataright);
         printf("Rest : %d %d \n",tree->leftrestnum,tree->rightrestnum);
        }
        else                // tree是分支节点
        {
        	printf("%2d - %2d is %2d's %6s child \n", tree->dataleft, tree->dataright,key, direction==1?"right" : "left");
        	printf("Rest : %d %d \n",tree->leftrestnum,tree->rightrestnum);
         printf("Left : %d %d \n",tree->leftstart,tree->leftend);
         printf("Right : %d %d \n",tree->rightstart,tree->rightend);
        }

        print_fulltree(tree->lefttree, tree->dataleft, -1);
        print_fulltree(tree->righttree, tree->dataleft, 1);

    }
}
bool deletetree(HeapTree* &root,int key,HeapTree* pre,int blocknum)
{
	if(!root)
		return false;
	if(root->dataleft==key)
	{

		HeapTree *rawroot;
		HeapTree *dest;
		if(root->righttree==NULL&&root->lefttree==NULL)
		{
			rawroot=root;
			root=NULL;
			free(rawroot);
			return true;
		}
		if(root->righttree==NULL&&root->lefttree!=NULL)
		{
			rawroot=root;
			if(root->rightend==-1)
				root->lefttree->rightend=root->dataright;
			else
				root->lefttree->rightend=root->rightend;
			root->lefttree->rightrestnum=root->rightrestnum+root->datablocknum;
			root->lefttree->rightstart=root->lefttree->rightend-root->lefttree->rightrestnum+1;
			root=root->lefttree;
			free(rawroot);
			return true;
		}
		if(root->lefttree==NULL&&root->righttree!=NULL)
		{
			rawroot=root;
			if(root->leftstart==-1)
				root->righttree->leftstart=root->dataleft;//judge
			else
				root->righttree->leftstart=root->leftstart;

			root->righttree->leftrestnum=root->leftrestnum+root->datablocknum;
			root->righttree->leftend=root->righttree->leftstart+root->righttree->leftrestnum-1;//如果边界直接使用子树的边界可能存在不一致
			root=root->righttree;
			free(rawroot);
			return true;
		}

		if(root->lefttree!=NULL&&root->righttree!=NULL)
		{
			//find nextnode
			rawroot=root;
			dest=root->righttree;
			while(dest->lefttree!=NULL)
			{
				pre=dest;
				dest=dest->lefttree;
			}
			//root=s;
			if(dest==root->righttree) // houji shi youerzi
			{
            //delete 13
				dest->lefttree=root->lefttree; //
				dest->leftstart=root->leftstart;
				dest->leftrestnum=root->leftrestnum+blocknum;
				dest->leftend=root->dataright;//
//				直接拿上面两个做减不可以，对于长区间的情况不适用
//				如果是get dest->leftend，可能存在节点为叶子节点，左边leftend已经被占用的情况
				//dest->leftend=dest->leftstart+dest->leftrestnum-1;
				root=dest;
				free(rawroot);
			}
			else
			{
				if(dest->righttree!=NULL)// 目标节点的右子树存在，需要先把右子树接到目标的pre节点上
				{
					pre->lefttree=dest->righttree; //信息会在后续进行修改
				}
				else
				{
					pre->lefttree=NULL;
				}
				//change root node info !!;
				int adjustrestnum=dest->leftrestnum;
				dest->leftstart=root->leftstart;
				//dest->leftend=dest->leftend;
				dest->leftrestnum=dest->leftrestnum+blocknum+root->leftrestnum;
				if(dest->leftend==-1)
				{
					dest->leftend=root->dataright;
				}

				//dest->leftend=dest->leftstart+dest->leftrestnum-1;
				//dest->rightstart=dest->rightstart;
				dest->rightend=root->rightend;
				dest->rightrestnum=root->rightrestnum-dest->rightrestnum;
				if(dest->rightstart==-1)
				{
					dest->rightstart=dest->dataright+1; //如果是rootdata的右边界，会把这个节点的数据信息囊括进去
				}
				//dest->rightstart=dest->rightend-dest->rightrestnum+1;
				dest->lefttree=root->lefttree;
				dest->righttree=root->righttree;
				root=dest;
				//修正root右子树信息
				HeapTree *temp=dest->righttree;
				HeapTree *newdest=temp;
				temp->leftstart=dest->rightstart;
				temp->leftrestnum=temp->leftrestnum-adjustrestnum;
				//temp->leftend=temp->leftstart+temp->leftrestnum-1;
				//
				while(newdest->lefttree!=NULL)
				{
					pre=newdest;
					newdest=newdest->lefttree;
					if(pre->leftstart<=newdest->leftstart)//这个信息表明这些后继目前都是目标节点的父亲，需要做相应调整
					{
						newdest->leftstart=pre->leftstart;
						newdest->leftrestnum=newdest->leftrestnum-adjustrestnum;
					}
					//如果是儿子则可以忽略
				}

				free(rawroot);
			}
			//修正root左子树信息
			int adjustnum=blocknum;
			HeapTree *temp=dest->lefttree;
			HeapTree *newdest=temp;
			if(temp->rightstart==-1)
			{
				temp->rightstart=temp->dataright+1;
			}
			temp->rightend=root->leftend;
			temp->rightrestnum=temp->rightrestnum+adjustnum;
			//temp->rightstart=;
			//temp->rightstart=temp->rightend-temp->rightrestnum+1;

			while(newdest->righttree!=NULL)
			{
				pre=newdest;
				newdest=newdest->righttree;
				newdest->rightend=pre->rightend;
				newdest->rightrestnum=newdest->rightrestnum+adjustnum;
				if(newdest->rightstart==-1)
				{
					newdest->rightstart=newdest->dataright+1;
				}
				//newdest->rightstart=newdest->rightend-newdest->rightrestnum+1;
			}

			return true;
		}
	}
	else
	{
		if(key<root->dataleft)
		{
			if(deletetree(root->lefttree,key,pre,blocknum))
			{
				root->leftrestnum+=blocknum;
				return true;
			}
			else
				return false;
		}
		else
		{
			if(deletetree(root->righttree,key,pre,blocknum))
			{
				root->rightrestnum+=blocknum;
				return true;
			}
			else
				return false;
		}

	}
	return false;

}

int main()
{

	void *mem=(void *)aligned_malloc(1<<30,4096);
	int mallocnum=100;
	HeapTree *memorytree=NULL;


		//a[i]=3094;
	//int blocknum=calcbasicblocknum(3094);
	int blocknum=2;
	for(int i=0;i<7;i++)
		malloctree(memorytree,blocknum,0,15);
	//printtree(memorytree);
	/*test left node
	deletetree(memorytree,10,NULL,blocknum);
	deletetree(memorytree,9,NULL,blocknum);*/
    //*test right node
//	deletetree(memorytree,15,NULL,blocknum);
//	deletetree(memorytree,14,NULL,blocknum);
/*	malloctree(memorytree,blocknum,0,15);
	malloctree(memorytree,blocknum,0,15);*/
	//1,2,3
	//1. houji shi youerzi
/*	后继是右儿子，右儿子无右结点
   deletetree(memorytree,14,NULL,blocknum);
	malloctree(memorytree,blocknum,0,15);
	后继是右儿子，右儿子有右结点
	deletetree(memorytree,13,NULL,blocknum);
	malloctree(memorytree,blocknum,0,15);
	后继是右儿子的左节点，左节点无右儿子
	deletetree(memorytree,11,NULL,blocknum);
	malloctree(memorytree,blocknum,0,15);
	后继是右儿子的左节点，左节点有右儿子
	deletetree(memorytree,13,NULL,blocknum);
	malloctree(memorytree,blocknum,0,15);
	deletetree(memorytree,11,NULL,blocknum);
	malloctree(memorytree,blocknum,0,15);


		*/
	malloctree(memorytree,1,0,15);
	//malloctree(memorytree,1,0,15);
	print_fulltree(memorytree,0,0);
	aligned_free(mem);

}
