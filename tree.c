#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <errno.h>
#include <malloc.h>
#include <math.h>

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define INFEASIBLE -1

#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10

typedef int Status ;
typedef char ElemType  ;

typedef struct BiTNode{
        ElemType data;
        struct BiTNode *lchild,*rchild;
}BiTNode,*BiTree;

typedef BiTree SElemType;
typedef struct{
        SElemType *base ;
        SElemType *top ;
        int stacksize;
}SqStack;

typedef BiTree QElemType;
typedef struct QNode
{
	QElemType data;
	struct QNode *next ;
}QNode ;
typedef struct{
    QNode *head;
    QNode *rear;
}Queue;

Status InitBiTree(BiTree *T);
Status DestroyBiTree(BiTree *T);
Status CreateBiTree(BiTree *T);
Status outElem(ElemType  e);
Status PreOrderTraverse(BiTree T,Status (*visit)(ElemType ));
Status InOrderTraverse(BiTree T,Status (*visit)(ElemType ));
Status PostOrderTraverse(BiTree T,Status (*visit)(ElemType ));
Status LevelOrderTraverse(BiTree T,Status (*visit)(ElemType));
Status F_PreOrderTraverse(BiTree T,Status (*visit)(ElemType ));
Status F_InOrderTraverse(BiTree T,Status (*visit)(ElemType ));
Status F_PostOrderTraverse(BiTree T,Status (*visit)(ElemType ));

Status InitStack(SqStack *S);
Status DestroyStack(SqStack *S);
Status ClearStack(SqStack *S);
Status StackEmpty(SqStack *S);
int  StackLength(SqStack *S);
Status GetTop(SqStack *S,SElemType *e);
Status Push(SqStack *S,SElemType e);
Status Pop(SqStack *S,SElemType *e);

Status InitQueue(Queue *Q);
Status DestroyQueue(Queue *Q);
Status EnQueue(Queue *Q,QElemType t);
Status QueueEmpty(Queue *Q);
Status DeQueue(Queue *Q,QElemType *t);

int main()
{
    BiTree root;
    CreateBiTree(&root);
    LevelOrderTraverse(root,outElem);   printf("\n");
    
    PreOrderTraverse(root,outElem);     printf("\n");
    
    F_PreOrderTraverse(root,outElem);   printf("\n");
    
    InOrderTraverse(root,outElem);      printf("\n");
    
    F_InOrderTraverse(root,outElem);    printf("\n");
    
    PostOrderTraverse(root,outElem);    printf("\n");
    
    F_PostOrderTraverse(root,outElem);  printf("\n");
    
    
    system("pause");
    return 0;
}
Status CreateBiTree(BiTree *T)
{
       BiTree p;
       char  ch;
       scanf("%c",&ch);
       getchar();
       if(ch=='0') *T=NULL; 
       else{
            p=(BiTNode *)malloc(sizeof(BiTNode));
            if(!p) return ERROR;
            p->data=ch;
            *T=p;
            CreateBiTree(&(p->lchild));
            CreateBiTree(&(p->rchild));
       }
       return OK;
}
Status outElem(ElemType  e)
{
	printf("%c",e);
	return OK ;
}

Status PreOrderTraverse(BiTree T,Status (*visit)(ElemType ))
{
       if(T){
             if(visit(T->data))
              if(PreOrderTraverse(T->lchild,outElem))
               if(PreOrderTraverse(T->rchild,outElem)) return OK;
              return ERROR;
       }else return OK;
}
Status InOrderTraverse(BiTree T,Status (*visit)(ElemType ))
{
       if(T){
             if(InOrderTraverse(T->lchild,outElem))
              if(visit(T->data))
               if(InOrderTraverse(T->rchild,outElem)) return OK;
              return ERROR;
       }else return OK;
}
Status PostOrderTraverse(BiTree T,Status (*visit)(ElemType ))
{
       if(T){
             if(PostOrderTraverse(T->lchild,outElem))
              if(PostOrderTraverse(T->rchild,outElem)) 
               if(visit(T->data)) return OK;
              return ERROR;
       }else return OK;
}
Status LevelOrderTraverse(BiTree T,Status (*visit)(ElemType))
{
       Queue Q;
       QElemType P;
       InitQueue(&Q);
       EnQueue(&Q,T);
       while(!QueueEmpty(&Q))
       {
             DeQueue(&Q,&P);
             visit(P->data);
             if(P->lchild)
               EnQueue(&Q,P->lchild);
             if(P->rchild)
               EnQueue(&Q,P->rchild);
       }
       DestroyQueue(&Q);
}
Status F_PreOrderTraverse(BiTree T,Status (*visit)(ElemType ))
{
        SElemType p;
        SqStack S;
        InitStack(&S);
        Push(&S,T);
        while(!StackEmpty(&S))
        {
           while(GetTop(&S,&p)&&p) 
           {
            if(!visit(p->data)) return ERROR;
            Push(&S,p->lchild);
           }
          Pop(&S,&p);
          if(!StackEmpty(&S))
           {
             Pop(&S,&p);
             Push(&S,p->rchild);
           }
        }
        DestroyStack(&S);
}

Status F_InOrderTraverse(BiTree T,Status (*visit)(ElemType ))
{
        SElemType p;
        SqStack S;
        InitStack(&S);
        Push(&S,T);
        while(!StackEmpty(&S))
        {
          while(GetTop(&S,&p)&&p) Push(&S,p->lchild);
          Pop(&S,&p);
          if(!StackEmpty(&S))
           {
             Pop(&S,&p);
             if(!visit(p->data)) return ERROR;
             Push(&S,p->rchild);
           }
        }
        DestroyStack(&S);
}
/*Status F_PostOrderTraverse(BiTree T,Status (*visit)(ElemType ))
{
        SElemType p;
        SqStack S;
        int flag=0;
        InitStack(&S);
        Push(&S,T);
        while(!StackEmpty(&S))
        {
          while(GetTop(&S,&p)&&p->lchild)   {Push(&S,p->lchild);flag=0;}
           
          if(!StackEmpty(&S))
           {
             GetTop(&S,&p);
             if(!p->rchild && !p->lchild)
             {
               if(!visit(p->data)) return ERROR;
               Pop(&S,&p);
               GetTop(&S,&p);
               if(flag) p->rchild=NULL;
               else p->lchild=NULL;
             }
             else {Push(&S,p->rchild);flag=1;}
           }
        }
        DestroyStack(&S);
}*/
Status F_PostOrderTraverse(BiTree T,Status (*visit)(ElemType ))
{
        SElemType p;
        SqStack S;
        int a[100]={0},top=0;
        InitStack(&S);
        p=T;
        while(p||!StackEmpty(&S))
        {
         if(p) 
         {
          Push(&S,p); a[top]=2;top++;
          p=p->lchild;
         }
         else
         {
          Pop(&S,&p);top--;
          if(!visit(p->data)) return ERROR;
          p=p->rchild;
         }
        }
        DestroyStack(&S);
}

Status InitStack(SqStack *S)
{
   S->base=(SElemType *)malloc(STACK_INIT_SIZE *sizeof(SElemType));
   if (!S->base) return FALSE;
   S->top=S->base;
   S->stacksize=STACK_INIT_SIZE;
   return OK;
}
Status DestroyStack(SqStack *S)
{
    if(S->base)
    {
      free(S->base);
      S->top=NULL;
      S->stacksize=0;
    } 
    else return ERROR;
    return OK;  
}

Status GetTop(SqStack *S,SElemType *e)
{
       if(S->top==S->base) return ERROR;
       *e=*(S->top-1);
       return OK;
}
Status Push(SqStack *S,SElemType e)
{
       if(S->top - S->base >= S->stacksize)
       {
         S->base =(SElemType *)realloc(S->base,( S->stacksize+STACKINCREMENT)*sizeof(SElemType));
         if(!S->base) return FALSE;
         S->top= S->base + S->stacksize;
         S->stacksize = S->stacksize+STACKINCREMENT;
       }
       *(S->top) = e;
       S->top++;
       return OK;
}
Status Pop(SqStack *S,SElemType *e)
{
       if(S->top==S->base) return ERROR;
       *e=*(--S->top);
       return OK;
}
Status StackEmpty(SqStack *S)
{
       if(S->top==S->base) 
       	{
			return TRUE ;
		}
		else
		{
			return FALSE;
		}
}


Status InitQueue(Queue *Q)
{
       QNode *p;
       p=(QNode *)malloc(sizeof(QNode));
       if(!p) return ERROR;
       p->data=NULL;
       p->next=NULL;
       Q->head=p;
       Q->rear=p;
}
Status DestroyQueue(Queue *Q)
{
       free(Q->head);
}
Status EnQueue(Queue *Q,QElemType t)
{
       QNode *p;
       p=(QNode *)malloc(sizeof(QNode));
       p->data=t;
       p->next=NULL;
       Q->rear->next=p;
       Q->rear=p;
}
Status QueueEmpty(Queue *Q)
{
       if(!Q->head->next)   return TRUE ;
       else    return FALSE;
}
Status DeQueue(Queue *Q,QElemType *t)
{
       QNode *p;
       p=Q->head->next;
       Q->head->next=p->next;
       if(!p->next) Q->rear=Q->head;
       *t=p->data;
       free(p);
}
