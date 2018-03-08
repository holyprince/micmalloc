/*
 * testvec.cpp
 *
 *  Created on: 2017年3月7日
 *      Author: wangzihao
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "immintrin.h"

/*#include <math.h>
#include <stddef.h>

#include "float.h"
#include<sys/time.h>

#include <offload.h>
#include <time.h>*/
//#define PRINT


typedef struct _Cilk_shared
{
    int m1;
    char *m2;
} nbwcs;

void __attribute__((target(mic))) set_length(nbwcs *s)
{
    s->m1 = strlen(s->m2);
}
_Cilk_shared nbwcs struct1;


char * _Cilk_shared reallocself(char *a, size_t newsize,size_t oldsize)
{
	return a;

}

_Cilk_shared void function_3()
{
	char *s;
	int i;
	s=(char *)malloc(sizeof(char)*10);
	for(i=0;i<10;i++)
		s[i]='m';
	s=reallocself(s,100,10);
/*
	printf("struct1.m2 %s \n",struct1.m2);
	int bdata;
	int *a=(int *)malloc(sizeof(int)*10);
	//abcd();

#pragma omp parallel for num_threads(220)
	for(int i=0;i<1000000;i++)
	{
		int x=1;
	}
		//for(int j=0;j<100000;j++)
*/


}
int main(void) {

	_Cilk_offload_to (0) function_3();
	return 0;
}
/*
int main()
{

/*	double *a=(double *)malloc(10*sizeof(double));
	for(int i=0;i<10;i++)
		a[i]=5;
	double *b=(double *)malloc(20*sizeof(double));
	MyMemMove2(b,a,10);
	for(int i=0;i<12;i++)
		printf("%f ",b[i]);*/


/*
int main() {


/*    char *string = "string";
    int str_len = strlen(string);
    char *stringp = (char *)malloc(str_len+1);
    int i=0;

    struct1.m1 = 0;
    struct1.m2 =(char *)	_Offload_shared_malloc(str_len+1);
     for(i=0;i<str_len;i++)
    	 struct1.m2[i]='a';
    //struct1.m2 = stringp;
    int m1;
    char *m2;

	float t1,t2;
	t1 = omp_get_wtime();



	t2 = omp_get_wtime();

	printf("time %f \n",(float)(t2-t1) *1e3);

   _Cilk_offload_to (0) function_3();
	int i;
	char *abc;
	abc=(char *)malloc(sizeof(char)*10);
	for(i=0;i<10;i++)
		abc[i]='a';
	abc=(char *)realloc(abc,40*sizeof(char));
	abc[30]='b';
	for(i=0;i<40;i++)
		printf("%c \n",abc[i]);
    /*
    #pragma offload target(mic)  inout(m2 : length(str_len+1))
    {
    	  //printf("%s on miC\n",struct1.m2);
        nbwcs struct2;
        //Reassemble the struct on the target
        struct2.m1 = m1;
        struct2.m2 = m2;

        // Use the non-bitwise copyable struct
        set_length(&struct2);

        // Disassemble it for copying back to CPU
        m1 = struct2.m1;
        m2 = struct2.m2;
    }
    // Reassemble struct received from target
    struct1.m1 = m1;
    struct1.m2 = m2;

    if (struct1.m1 == str_len)
        printf("PASS Sample11\n");
    else
        printf("*** FAIL Sample11\n");*/

/*
#define kbtree_t(name) kbtree_##name##_t
#define kb_init(name, s) kb_init_##name(s)
void MyMemMove1(char *dst,char *src,int count)
{
	while(count--)
		*dst++ = *src++;
}
#define type_t double
void MyMemMove2(void *dst,void *src,int count)
{
	while (count--)
	{
		*(type_t *)dst = *(type_t *)src;
		dst = (type_t *)dst + 1;
		src = (type_t *)src + 1;
	}
}

void * MyMemMove(void *dst,const void *src,int count)
{

	void *ret=dst;
	while (count--)
	{
		*(type_t *)dst = *(type_t *)src;
		dst = (type_t *)dst + 1;
		src = (type_t *)src + 1;
	}
	return ret;
}

typedef struct {
	int score; // best score
	int te, qe; // target end and query end
	int score2, te2; // second best score and ending position on the target
	int tb;
	int *qb; // target start and query start
} kswr_t;*/



