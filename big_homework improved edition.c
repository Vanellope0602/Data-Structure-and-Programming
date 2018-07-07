#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#define M 450000
double seed = 997;
double rrand()
{
    double lambda = 3125.0;
    double m = 34359738337.0;
    double r;
    seed = fmod(lambda*seed, m);
    r = seed / m;
    return r;
}
struct suf{
	char word[100];
	struct suf *next;
};
typedef struct suf suffix, *LinkSuffix;
typedef struct node{
    char prefix[200];
    int num_of_suf;
    struct suf suffix;
    struct node *link;
    struct suf *tail;    //这个tail 应该保持指向最后一个后缀 

}LNode, *LinkList;
unsigned int BKDRHash(char *str);
 //每一个前缀里面 有一个指向后缀的尾指针即可,下面用一个40万大数组存一下哈希表的尾指针


int main()
{
    FILE *in , *out;
    char tmp1[200], tmp2[200], tmp3[200];
    char combine[200];  /*two words leads to a combination , use this combination to create hash value*/
    int i = 0, loop = 0, len1;
    LinkList hash_list[M];  /*huge array to store pointer*/
    LinkList p,q, list;
    LinkSuffix pt , p0;

    unsigned int hash_value;
    for(loop = 0; loop< M; loop++)
    {
    	list = (LinkList)malloc(sizeof(LNode)); //数组里面只存指针，现在初始化让数组里面的每一个指针都指向一个新建的空间 
    	list->link = NULL;
        list->suffix.next = NULL;
        list->tail = NULL;
		hash_list[loop] = list;
	}

    if( (in = fopen("article.txt","r")) == NULL )  printf("No \n");
    fscanf(in, "%s %s", tmp1, tmp2);
    char top1[100], top2[100];
    strcpy( top1, tmp1 );
    strcpy( top2, tmp2 );
    
    strcpy(combine, tmp1);
    len1 = strlen(combine);
    combine[len1] = ' ';
    combine[len1+1] = '\0';
    strcat(combine ,tmp2);
    

    hash_value = BKDRHash(combine);
    
    p = hash_list[hash_value];  
    while( (fscanf(in, "%s", tmp3) )>0 )
    {
    	
        strcpy(combine, tmp1);
    	len1 = strlen(combine);
    	combine[len1] = ' ';
    	combine[len1+1] = '\0';
    	strcat(combine ,tmp2);
    	
        hash_value = BKDRHash(combine); 
        //hash_list[hash_value] 本身是指向第一个元素的指针 ,第一个元素理应不存储东西 
        if( hash_list[hash_value]->link == NULL )  /*a new combined prefix added*/
		{
			q = hash_list[hash_value];  //q 就是那个list 一个数组里的头指针 
			p = (LinkList)malloc(sizeof(LNode));
			p->num_of_suf = 1;
		    strcpy(p->prefix, combine);
		    
		    pt = (LinkSuffix)malloc(sizeof(suffix)); // establish a suffix node
		    strcpy(pt->word, tmp3);
		    p->suffix.next = pt;
		    pt->next = NULL;
		    
		    p->tail = pt;
		    
		    q->link = p;
		    p->link = NULL;
		}
		else if( hash_list[hash_value]->link != NULL )   /*maybe hash conflict or a repeatedly appeared prefix*/
		{
            p = hash_list[hash_value]->link;// 指向第一个 prefix那个 
            /*judge this is a new prefix or an old one ????*/
            while( strcmp(p->prefix,combine) != 0  && p->link != NULL )
            {
                p = p->link;
            }
            if( strcmp(p->prefix,combine) != 0 && (p->link == NULL ))  /*已经到了这个hash值前缀的最后一个 依然没有找到a new prefix that conflicts with hash value*/
            {   //hash conflict
                 /*新建一个LNode 新前缀结点。put in prefix and its first suffix*/
                q = (LinkList)malloc(sizeof(LNode));
                q->num_of_suf = 1;
                strcpy(q->prefix, combine);
                
                pt = (LinkSuffix)malloc(sizeof(suffix));
		    	strcpy(pt->word, tmp3);
		    	q->suffix.next = pt;
		    	pt->next = NULL;
		    	
		    	q->tail = pt;
		    	
		    	p->link = q;
		    	q->link = NULL;
                
            }
            else /*means this is a repeatedly appeared prefix只用新建一个后缀结点，插在对应前缀的众多后缀中最后一个*/
            {
                /*No need to judge if its suffix has occured before 
                just add it like a new suffix after "next" pointer ,remember the number of suffix +1 重复前缀了！需要在这里正常插入一个新的后缀
				在tail后面插入后缀然后修改尾指针*/
				p->num_of_suf++;
		
				p0 = p->tail;  //按理说此时 p0->next == NULL 
                pt = (LinkSuffix)malloc(sizeof(suffix));
                
                strcpy(pt->word, tmp3);
                p0->next = pt;
                pt->next = NULL;
                
                p->tail = pt; 
                
            } 
			
		}

        memset(combine, 0, sizeof(combine));
        strcpy(tmp1, tmp2);
        strcpy(tmp2, tmp3);
    }/*now only EOF left*/
    
    strcpy(tmp3, "(end)");      /*give it corresponding new suffix下面这一段是重复while循环里面的内容，目的是把最后一个前缀加入哈希表，（end）放到后缀里面去*/
    strcpy(combine, tmp1);
    	len1 = strlen(combine);
    	combine[len1] = ' ';
    	combine[len1+1] = '\0';
    	strcat(combine ,tmp2);
    	
        
        hash_value = BKDRHash(combine);
        
        //hash_list[hash_value] 本身是指向第一个元素的指针 ,第一个元素理应不存储东西 
        if( hash_list[hash_value]->link == NULL )  /*a new combined prefix added*/
		{
			q = hash_list[hash_value];  //q 就是那个list 一个数组里的头指针 
			p = (LinkList)malloc(sizeof(LNode));
			p->num_of_suf = 1;
		    strcpy(p->prefix, combine);
		    
		    pt = (LinkSuffix)malloc(sizeof(suffix));
		    strcpy(pt->word, tmp3);
		    p->suffix.next = pt;
		    pt->next = NULL;
		    
		    p->tail = pt;
		    
		    q->link = p;
		    p->link = NULL;
		    
		}
		else if( hash_list[hash_value]->link != NULL )   /*maybe hash conflict or a repeatedly appeared prefix*/
		{
			
            p = hash_list[hash_value]->link;// 指向第一个 prefix那个 
            /*judge this is a new prefix or an old one ????*/
            while( strcmp(p->prefix,combine) != 0  && p->link != NULL )
            {
                p = p->link;
                
            }
            if( strcmp(p->prefix,combine) != 0 && (p->link == NULL ))  /*已经到了这个hash值前缀的最后一个 依然没有找到a new prefix that conflicts with hash value*/
            {   
                /*新建一个LNode 新前缀结点。put in prefix and its first suffix*/
                q = (LinkList)malloc(sizeof(LNode));
                q->num_of_suf = 1;
                strcpy(q->prefix, combine);
                
                pt = (LinkSuffix)malloc(sizeof(suffix));
		    	strcpy(pt->word, tmp3);
		    	q->suffix.next = pt;
		    	pt->next = NULL;
		    		q->tail = pt;
		    	p->link = q;
		    	q->link = NULL;
                
            }
            else /*means this is a repeatedly appeared prefix只用新建一个后缀结点，插在对应前缀的众多后缀中最后一个*/
            {
                /*No need to judge if its suffix has occured before 
                just add it like a new suffix after "next" pointer ,remember the number of suffix +1 重复前缀了！需要在这里正常插入一个新的后缀*/
				p->num_of_suf++;
		
				p0 = p->tail;  //按理说此时 p0->next == NULL 
                pt = (LinkSuffix)malloc(sizeof(suffix));
                
                strcpy(pt->word, tmp3);
                p0->next = pt;
                pt->next = NULL;
                
                p->tail = pt; 
            } 
			
		}

        memset(combine, 0, sizeof(combine));

/*NOW  建立了所有哈希表即每个前缀对应的后缀 接下来要输出文本了*/	
	out = fopen("markov.txt", "w");
	
    char obj[200] , nextword[200];//obj相当于上面的combine， houzhui 就是后缀 
    int n;
    
    strcpy(obj, top1);
    len1 = strlen(obj);
    obj[len1] = ' ';
    obj[len1+1] = '\0';
    strcat(obj,top2);
    hash_value = BKDRHash(obj);
	p = hash_list[hash_value]->link;   /*hash value is a head pointer直接计算指向这个前缀的数组中存的结构*/
	
	fprintf(out, "%s ", obj);
	while(1)
	{
		i = 0;
	    p = hash_list[hash_value]->link;   /*hash value is a head pointer直接计算指向这个前缀的数组中存的结构*/ 
		while( strcmp(p->prefix, obj) != 0  && p->link != NULL) /*找到对应的前缀, 根据top2 和 houzhui 找到下一个后缀，top1 = top2, top2 = houzhui ,再找新的houzhui*/
		{
			p = p->link;
		}//找到前缀 
		pt = p->suffix.next; //指向第一个后缀 但是这个后缀节点应该是不存东西的 
		//找到对应前缀的指针，它还有可能有多个后缀 而这个后缀是由 该前缀的总后缀数量 和随机函数决定的 要跳多少次
		if( p->num_of_suf == 1 ) {
			
			if(strcmp( pt->word, "(end)") != 0)
			fprintf(out, "%s ", pt->word);
			
			strcpy(nextword, pt->word);
		}
		else{
			n = (int)(rrand() *(p->num_of_suf));
			
			for(i = 0; i<n; i++)
			{
				pt = pt->next;
			}
			
			if(strcmp( pt->word, "(end)") != 0)
			fprintf(out,"%s ", pt->word);
			
			strcpy(nextword, pt->word);

		}
		if(strcmp(nextword, "(end)") == 0) break; 
		else{
			strcpy(top1, top2);
			strcpy(top2, nextword);
			
			strcpy(obj, top1);
    		len1 = strlen(obj);
    		obj[len1] = ' ';
    		obj[len1+1] = '\0';
    		strcat(obj,top2);
    		hash_value = BKDRHash(obj);
			p = hash_list[hash_value];    /*根据随机产生的nextword制作下一个前缀 再进入下一个循环 如果*/
		}
    	
	}  
	fclose(in);
	fclose(out);
    return 0; 
	
}

unsigned int BKDRHash(char *str) /*给每个字符串都赋予一个数字编号 据说是一个很少冲突的算法*/ 
{
    unsigned int seed = 131; 
    unsigned int hash = 0;
    while (*str)
    {
        hash = hash * seed + (*str++);
    }
    return (hash & 0x7FFFFFFF) % M ;
}

