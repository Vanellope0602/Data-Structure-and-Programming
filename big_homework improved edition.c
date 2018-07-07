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
    struct suf *tail;    //���tail Ӧ�ñ���ָ�����һ����׺ 

}LNode, *LinkList;
unsigned int BKDRHash(char *str);
 //ÿһ��ǰ׺���� ��һ��ָ���׺��βָ�뼴��,������һ��40��������һ�¹�ϣ���βָ��


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
    	list = (LinkList)malloc(sizeof(LNode)); //��������ֻ��ָ�룬���ڳ�ʼ�������������ÿһ��ָ�붼ָ��һ���½��Ŀռ� 
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
        //hash_list[hash_value] ������ָ���һ��Ԫ�ص�ָ�� ,��һ��Ԫ����Ӧ���洢���� 
        if( hash_list[hash_value]->link == NULL )  /*a new combined prefix added*/
		{
			q = hash_list[hash_value];  //q �����Ǹ�list һ���������ͷָ�� 
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
            p = hash_list[hash_value]->link;// ָ���һ�� prefix�Ǹ� 
            /*judge this is a new prefix or an old one ????*/
            while( strcmp(p->prefix,combine) != 0  && p->link != NULL )
            {
                p = p->link;
            }
            if( strcmp(p->prefix,combine) != 0 && (p->link == NULL ))  /*�Ѿ��������hashֵǰ׺�����һ�� ��Ȼû���ҵ�a new prefix that conflicts with hash value*/
            {   //hash conflict
                 /*�½�һ��LNode ��ǰ׺��㡣put in prefix and its first suffix*/
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
            else /*means this is a repeatedly appeared prefixֻ���½�һ����׺��㣬���ڶ�Ӧǰ׺���ڶ��׺�����һ��*/
            {
                /*No need to judge if its suffix has occured before 
                just add it like a new suffix after "next" pointer ,remember the number of suffix +1 �ظ�ǰ׺�ˣ���Ҫ��������������һ���µĺ�׺
				��tail��������׺Ȼ���޸�βָ��*/
				p->num_of_suf++;
		
				p0 = p->tail;  //����˵��ʱ p0->next == NULL 
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
    
    strcpy(tmp3, "(end)");      /*give it corresponding new suffix������һ�����ظ�whileѭ����������ݣ�Ŀ���ǰ����һ��ǰ׺�����ϣ����end���ŵ���׺����ȥ*/
    strcpy(combine, tmp1);
    	len1 = strlen(combine);
    	combine[len1] = ' ';
    	combine[len1+1] = '\0';
    	strcat(combine ,tmp2);
    	
        
        hash_value = BKDRHash(combine);
        
        //hash_list[hash_value] ������ָ���һ��Ԫ�ص�ָ�� ,��һ��Ԫ����Ӧ���洢���� 
        if( hash_list[hash_value]->link == NULL )  /*a new combined prefix added*/
		{
			q = hash_list[hash_value];  //q �����Ǹ�list һ���������ͷָ�� 
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
			
            p = hash_list[hash_value]->link;// ָ���һ�� prefix�Ǹ� 
            /*judge this is a new prefix or an old one ????*/
            while( strcmp(p->prefix,combine) != 0  && p->link != NULL )
            {
                p = p->link;
                
            }
            if( strcmp(p->prefix,combine) != 0 && (p->link == NULL ))  /*�Ѿ��������hashֵǰ׺�����һ�� ��Ȼû���ҵ�a new prefix that conflicts with hash value*/
            {   
                /*�½�һ��LNode ��ǰ׺��㡣put in prefix and its first suffix*/
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
            else /*means this is a repeatedly appeared prefixֻ���½�һ����׺��㣬���ڶ�Ӧǰ׺���ڶ��׺�����һ��*/
            {
                /*No need to judge if its suffix has occured before 
                just add it like a new suffix after "next" pointer ,remember the number of suffix +1 �ظ�ǰ׺�ˣ���Ҫ��������������һ���µĺ�׺*/
				p->num_of_suf++;
		
				p0 = p->tail;  //����˵��ʱ p0->next == NULL 
                pt = (LinkSuffix)malloc(sizeof(suffix));
                
                strcpy(pt->word, tmp3);
                p0->next = pt;
                pt->next = NULL;
                
                p->tail = pt; 
            } 
			
		}

        memset(combine, 0, sizeof(combine));

/*NOW  ���������й�ϣ��ÿ��ǰ׺��Ӧ�ĺ�׺ ������Ҫ����ı���*/	
	out = fopen("markov.txt", "w");
	
    char obj[200] , nextword[200];//obj�൱�������combine�� houzhui ���Ǻ�׺ 
    int n;
    
    strcpy(obj, top1);
    len1 = strlen(obj);
    obj[len1] = ' ';
    obj[len1+1] = '\0';
    strcat(obj,top2);
    hash_value = BKDRHash(obj);
	p = hash_list[hash_value]->link;   /*hash value is a head pointerֱ�Ӽ���ָ�����ǰ׺�������д�Ľṹ*/
	
	fprintf(out, "%s ", obj);
	while(1)
	{
		i = 0;
	    p = hash_list[hash_value]->link;   /*hash value is a head pointerֱ�Ӽ���ָ�����ǰ׺�������д�Ľṹ*/ 
		while( strcmp(p->prefix, obj) != 0  && p->link != NULL) /*�ҵ���Ӧ��ǰ׺, ����top2 �� houzhui �ҵ���һ����׺��top1 = top2, top2 = houzhui ,�����µ�houzhui*/
		{
			p = p->link;
		}//�ҵ�ǰ׺ 
		pt = p->suffix.next; //ָ���һ����׺ ���������׺�ڵ�Ӧ���ǲ��涫���� 
		//�ҵ���Ӧǰ׺��ָ�룬�����п����ж����׺ �������׺���� ��ǰ׺���ܺ�׺���� ��������������� Ҫ�����ٴ�
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
			p = hash_list[hash_value];    /*�������������nextword������һ��ǰ׺ �ٽ�����һ��ѭ�� ���*/
		}
    	
	}  
	fclose(in);
	fclose(out);
    return 0; 
	
}

unsigned int BKDRHash(char *str) /*��ÿ���ַ���������һ�����ֱ�� ��˵��һ�����ٳ�ͻ���㷨*/ 
{
    unsigned int seed = 131; 
    unsigned int hash = 0;
    while (*str)
    {
        hash = hash * seed + (*str++);
    }
    return (hash & 0x7FFFFFFF) % M ;
}

