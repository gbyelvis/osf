#include<iostream>
#include<stdlib.h>
#include"stdio.h"
using namespace std;
typedef struct
{  float coef;//ϵ��
   int expn;//ָ��
}polynomial;

typedef struct LNode
{ polynomial data;//��������
  struct LNode *next;
}LNode,*Link;

void createLink(Link &L,int n);   //��������
void printList(Link L);            //��ӡ����ʽ
void addPolyn(Link &pc,Link pa,Link pb);  //����ʽ�ӷ�
void substractPolyn(Link &pc,Link pa,Link pb);   //����ʽ����
void copyLink(Link &pc,Link pa);      //��������
void mulPolyn(Link &pc,Link pa,Link pb);   //����ʽ�˷�
int locateLink(Link pa,Link e);     //�ж�ָ���Ƿ�����ͬ����
void destroyLink(Link &L);     //��������
int compareNum(int i);      //�ж�����������ǲ����ַ�

void destroyLink(Link &L)   //��������
{
 Link p;
 p=L->next;
 while(p)   //����һ�����ڵ�
 {
     L->next=p->next;
  delete p;
  p=L->next;
 }
 delete L;   //ɾ��ͷ�ڵ�
 L=NULL;   //ͷָ����ΪNULL
}

void createLink(Link &L,int n)   //��������
{
  if(L!=NULL){destroyLink(L);}   //�����Ϊ�գ�����������
  Link p,newp;
  //int k=0;
  L=new LNode;
  L->next=NULL;
  (L->data).expn=-1;//����ͷ���
  p=L;
  for(int i=1;i<=n;i++)
  {
    newp=new LNode;
    cout<<"�������"<<i<<"���ϵ����ָ��:"<<endl;
    cout<<"ϵ��:";
    cin>>(newp->data).coef;

    cout<<"ָ��:";
    cin>>(newp->data).expn;
    if(newp->data.expn<0){cout<<"����������ָ��������Ϊ��ֵ!"<<endl;delete newp;i--;continue;}
    newp->next=NULL;
    p=L;//����ָ��ͷָ��
    if(newp->data.coef==0){cout<<"ϵ��Ϊ�㣬��������!"<<endl;delete newp;i--;continue;}
    while((p->next!=NULL)&&((p->next->data).expn<(newp->data).expn))//���ն���ʽָ���������
    {p=p->next;}
    if(!locateLink( L, newp))   //�Ƿ���ָͬ������
    {
     newp->next=p->next;     //��������L��
     p->next=newp;
    }
    else
    {
     cout<<"����ĸ���ָ�������ʽ���Ѵ��ڵ�ĳ����ͬ,�����´���һ����ȷ�Ķ���ʽ"<<endl;
     delete newp;   //ɾ���ڵ�
     destroyLink(L);   //��������
     createLink(L,n);   //��������
     break;
    }
  }
}


/*�ж�ָ���Ƿ������ʽ���Ѵ��ڵ�ĳ����ͬ*/
int locateLink(Link L,Link e)
{
 Link p;
 p=L->next;
 while(p!=NULL&&(e->data.expn!=p->data.expn))  //ָ����ͬ
  p=p->next;
 if(p==NULL)  //��Ϊ�գ���û����ͬ��
    return 0;
 else return 1;
}


/*�������*/
void printList(Link L)
{
  Link p;

  if(L==NULL||L->next==NULL) cout<<"��һԪ����ʽΪ�գ�"<<endl;
  else
  {
   p=L->next;
   //��һ��Ԫ��
     if((p->data).coef>0)   //ϵ������0
    {
    if((p->data).expn==0)   //ָ������0,���ϵ��
         cout<<(p->data).coef;
       else if((p->data).coef==1&&(p->data).expn==1)  //ϵ������1��ָ������1
           cout<<"x";
       else if((p->data).coef==1&&(p->data).expn!=1)   //ϵ������1��ָ��������1
              cout<<"x^"<<(p->data).expn;
       else  if((p->data).expn==1&&(p->data).coef!=1)   //ϵ��������1��ָ������1
              cout<<(p->data).coef<<"x";
       else cout<<(p->data).coef<<"x^"<<(p->data).expn;  //����������
    }
     if((p->data).coef<0)   //ϵ��С��0
      {
       if((p->data).expn==0)   //ָ������0
         cout<<(p->data).coef;
       else if(p->data.coef==-1&&p->data.expn==1)  //ϵ������-1��ָ������1
        cout<<"-x";
          else if(p->data.coef==-1&&p->data.expn!=1)  //ϵ������-1��ָ��������1
              cout<<"-x^"<<p->data.expn;
       else if(p->data.expn==1)   //ָ������1
              cout<<p->data.coef<<"x";
          else  cout<<(p->data).coef<<"x^"<<(p->data).expn;  //����������
      }
  p=p->next;
  //�ǵ�һ��Ԫ��
  while(p!=NULL)
  {
   if((p->data).coef>0)
   {
    if((p->data).expn==0)
     cout<<"+"<<(p->data).coef;
          else if((p->data).expn==1&&(p->data).coef!=1)
     cout<<"+"<<(p->data).coef<<"x";
          else if((p->data).expn==1&&(p->data).coef==1)
                    cout<<"+"<<"x";
          else if((p->data).coef==1&&(p->data).expn!=1)
                                  cout<<"+"<<"x^"<<(p->data).expn;
          else cout<<"+"<<(p->data).coef<<"x^"<<(p->data).expn;
   }
      if((p->data).coef<0)
   {
     if((p->data).expn==0)
     cout<<(p->data).coef;
   else if(p->data.coef==-1&&p->data.expn==1)
    cout<<"-x";
      else if(p->data.coef==-1&&p->data.expn!=1)
          cout<<"-x^"<<p->data.expn;
   else if(p->data.expn==1)
          cout<<p->data.coef<<"x";
      else  cout<<(p->data).coef<<"x^"<<(p->data).expn;
   }

    p=p->next;
  }
  }

  cout<<endl;
}

/*��һ����������ݸ��Ƹ���һ������*/
void copyLink(Link &pc,Link pa)
{
 Link p,q,r;
 pc=new LNode;
 pc->next=NULL;
 r=pc;
 p=pa;
 while(p->next!=NULL)  //����pa����ֱ��ȫ��������
 {q=new LNode;   //��ʱ����
  q->data.coef=p->next->data.coef;   //ϵ������
  q->data.expn=p->next->data.expn;   //ָ������
  r->next=q;     //β�巨
  q->next=NULL;
  r=q;
  p=p->next;  //��һ���ڵ�
 }
}


/*������һԪ����ʽ���*/
void addPolyn(Link &pc,Link pa,Link pb)
{
 Link p1,p2,p,pd;
 copyLink(p1,pa);
 copyLink(p2,pb);
 pc=new LNode;
 pc->next=NULL;
 p=pc;
 p1=p1->next;
 p2=p2->next;
 while(p1!=NULL&&p2!=NULL)  //��������ʽ����Ϊ��
 {
  if(p1->data.expn<p2->data.expn)   //��һ��ָ��С�ڵڶ���ָ��
  {
   p->next=p1;   //��һ������ʽ��Ԫ�ز���PC������
   p=p->next;    //����ƶ�
   p1=p1->next;   //������һ��
  }
  else if(p1->data.expn>p2->data.expn)   //��һ��ָ�����ڵڶ���ָ��
  {
   p->next=p2;
   p=p->next;
   p2=p2->next;
  }
  else    //��һ��ָ�����ڵڶ���ָ������ϵ���ӷ�
  {
  // newp=new LNode;
   p1->data.coef=p1->data.coef+p2->data.coef;
   //p1->data.expn=p1->data.expn;
   if(p1->data.coef!=0)   //������֮��ϵ��������0�������������
   {
    p->next=p1;
    p=p->next;
    p1=p1->next;
    p2=p2->next;
   }
   else   //�������һ�¸�
   {
    pd=p1;
    p1=p1->next;
    p2=p2->next;
    delete pd;
   }

  }


 }
 if(p1!=NULL)  //��һ������ʽ��Ϊ�գ��ڶ���Ϊ��
 {
  p->next=p1;  //����һ��ʣ��Ķ����뵽PC������
 }
 if(p2!=NULL)
 {
  p->next=p2;
 }

}

/*����������ʽ���*/
void substractPolyn(Link &pc,Link pa,Link pb)
{
  Link p,pt;
  copyLink(pt,pb);   //��������
  p=pt;
  while(p!=NULL)  //�����ڶ�������
  {
     (p->data).coef=(-(p->data).coef);   //���ڶ�������ϵ��ȫ���÷�
        p=p->next;   //������һ��Ԫ��
  }
  addPolyn(pc,pa,pt);   //���ӷ�����
  destroyLink(pt);   //������ʱ����
}
//��������
void Clear()
{
 char a;
 cout<<"�밴 �س��� ��������"<<endl;
 a=getchar();   //�����ַ�
 system("cls");
}

/*������һԪ����ʽ���*/
void mulPolyn(Link &pc,Link pa,Link pb)
{
 Link p1,p2,p,pd,newp,t;
 pc=new LNode;
 pc->next=NULL;
 p1=pa->next;  //��һ������ʽ
 p2=pb->next;  //�ڶ�������ʽ
 while(p1!=NULL)   //������һ������ʽȫ����
 {
  pd=new LNode;
     pd->next=NULL;
  p=new LNode;
  p->next=NULL;
  t=p;
  while(p2)   //�����ڶ�������ʽȫ����
  {
   newp=new LNode;
   newp->next=NULL;  //ÿһ��ϵ����ָ��������
   newp->data.coef=p1->data.coef*p2->data.coef;  //ϵ����ϵ��
   newp->data.expn=p1->data.expn+p2->data.expn;   //ָ�����
   t->next=newp;
   t=t->next;
   p2=p2->next;  //�ڶ�������ʽ����һ��Ԫ��
  }
  addPolyn(pd,pc,p);   //ÿ������һ�γ˷��������������ӷ�
  copyLink(pc,pd);
  p1=p1->next;  //������һ������ʽ����һ��Ԫ��
  p2=pb->next;  //���¶�λP2ָ��Ϊ�ڶ�������ʽ����Ԫ��
  destroyLink(p);
  destroyLink(pd);
 }
}



 //�˵�����
void menuPrint()
{

 cout<<"\t***********һԪ����ʽ�ļ�����*************"<<endl;
 cout<<"\t\t 1����Ҫ���������һԪ����ʽ"<<endl;
 cout<<"\t\t 2������һԪ����ʽ���"<<endl;
 cout<<"\t\t 3������һԪ����ʽ���"<<endl;
 cout<<"\t\t 4������һԪ����ʽ���"<<endl;
 cout<<"\t\t 5��ʾ����һԪ����ʽ"<<endl;
 cout<<"\t\t 6�����������Ķ�������ʽ"<<endl;
 cout<<"\t\t 7�˳�"<<endl;
 cout<<"��������Ҫ���еĲ�����1-7��"<<endl;
}

//�ж�����������ǲ����ַ�
int compareNum(int i)
{
 if(i==-32766)
  return 0;
 else return 1;
}

int main()
{
   int n=-32766;
   Link L,La=NULL,Lb=NULL;
   int choose;
   while(1)
   {
    menuPrint();
       cin>>choose;
    switch(choose)
    {
    case 1:
      cout<<"��������Ҫ����ĵ�һ��һԪ����ʽ������:"<<endl;
            cin>>n;
   if(compareNum(n)==0){cout<<"���������������������롭��"<<endl;Clear();break;}
            createLink(La,n);
            cout<<"��������Ҫ����ĵڶ���һԪ����ʽ������:"<<endl;
            cin>>n;
   if(compareNum(n)==0){cout<<"���������������������롭��"<<endl;Clear();break;}
            createLink(Lb,n);
   Clear();
          break;
    case 2:
    if(La==NULL||Lb==NULL){cout<<"���Ķ���ʽ��������������ѡ�񡭡�"<<endl;Clear();break;}
       addPolyn(L,La,Lb);
          cout<<"��ӵ�����һԪ����ʽΪ��"<<endl;
             printList(La);
          printList(Lb);
          cout<<"��Ӻ�Ľ��Ϊ��"<<endl;
          printList(L);
    Clear();
    destroyLink(L);
             break;
    case 3:
     if(La==NULL||Lb==NULL){cout<<"���Ķ���ʽ��������������ѡ�񡭡�"<<endl;Clear();break;}
      substractPolyn(L,La,Lb);
      cout<<"���������һԪ����ʽΪ��"<<endl;
            printList(La);
         printList(Lb);
       cout<<"�����Ľ��Ϊ��"<<endl;
       printList(L);
   Clear();
   destroyLink(L);
      break;
    case 4:
     if(La==NULL||Lb==NULL){cout<<"���Ķ���ʽ��������������ѡ�񡭡�"<<endl;Clear();break;}
   mulPolyn(L,La,Lb);
            cout<<"��˵�����һԪ����ʽΪ��"<<endl;
            printList(La);
         printList(Lb);
      cout<<"��˺�Ľ��Ϊ��"<<endl;
      printList(L);
   destroyLink(L);
   Clear();
      break;
    case 5:
     if(La==NULL||Lb==NULL){cout<<"���Ķ���ʽ��������������ѡ�񡭡�"<<endl;Clear();break;}
              cout<<"��һ��һԪ����ʽΪ��"<<endl;
           printList(La);
            cout<<"�ڶ���һԪ����ʽΪ��"<<endl;
              printList(Lb);
     Clear();
              break;
    case 6:
     if(La&&Lb){destroyLink(La);destroyLink(Lb);cout<<"���ٳɹ���"<<endl;Clear();}
     else
     { cout<<"����ʽ�����ڣ�������ѡ��^^^"<<endl;Clear();}
           break;
    case 7:
     exit(0);
    default:
     cout<<"������������������ѡ���������"<<endl;Clear();
     break;
    }

   }
   return 0;
}
