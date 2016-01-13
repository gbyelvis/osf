#include<iostream>
#include<stdlib.h>
#include"stdio.h"
using namespace std;
typedef struct
{  float coef;//系数
   int expn;//指数
}polynomial;

typedef struct LNode
{ polynomial data;//链表类型
  struct LNode *next;
}LNode,*Link;

void createLink(Link &L,int n);   //创建函数
void printList(Link L);            //打印多项式
void addPolyn(Link &pc,Link pa,Link pb);  //多项式加法
void substractPolyn(Link &pc,Link pa,Link pb);   //多项式减法
void copyLink(Link &pc,Link pa);      //拷贝链表
void mulPolyn(Link &pc,Link pa,Link pb);   //多项式乘法
int locateLink(Link pa,Link e);     //判断指数是否有相同的项
void destroyLink(Link &L);     //销毁链表
int compareNum(int i);      //判断输入的整数是不是字符

void destroyLink(Link &L)   //销毁链表
{
 Link p;
 p=L->next;
 while(p)   //消除一个个节点
 {
     L->next=p->next;
  delete p;
  p=L->next;
 }
 delete L;   //删除头节点
 L=NULL;   //头指针置为NULL
}

void createLink(Link &L,int n)   //创建链表
{
  if(L!=NULL){destroyLink(L);}   //如果不为空，就销毁链表
  Link p,newp;
  //int k=0;
  L=new LNode;
  L->next=NULL;
  (L->data).expn=-1;//创建头结点
  p=L;
  for(int i=1;i<=n;i++)
  {
    newp=new LNode;
    cout<<"请输入第"<<i<<"项的系数和指数:"<<endl;
    cout<<"系数:";
    cin>>(newp->data).coef;

    cout<<"指数:";
    cin>>(newp->data).expn;
    if(newp->data.expn<0){cout<<"您输入有误，指数不允许为负值!"<<endl;delete newp;i--;continue;}
    newp->next=NULL;
    p=L;//重新指向头指针
    if(newp->data.coef==0){cout<<"系数为零，重新输入!"<<endl;delete newp;i--;continue;}
    while((p->next!=NULL)&&((p->next->data).expn<(newp->data).expn))//按照多项式指数次序调整
    {p=p->next;}
    if(!locateLink( L, newp))   //是否相同指数的项
    {
     newp->next=p->next;     //插入链表L中
     p->next=newp;
    }
    else
    {
     cout<<"输入的该项指数与多项式中已存在的某项相同,请重新创建一个正确的多项式"<<endl;
     delete newp;   //删除节点
     destroyLink(L);   //销毁链表
     createLink(L,n);   //创建链表
     break;
    }
  }
}


/*判断指数是否与多项式中已存在的某项相同*/
int locateLink(Link L,Link e)
{
 Link p;
 p=L->next;
 while(p!=NULL&&(e->data.expn!=p->data.expn))  //指数不同
  p=p->next;
 if(p==NULL)  //若为空，则没有相同项
    return 0;
 else return 1;
}


/*输出链表*/
void printList(Link L)
{
  Link p;

  if(L==NULL||L->next==NULL) cout<<"该一元多项式为空！"<<endl;
  else
  {
   p=L->next;
   //第一项元素
     if((p->data).coef>0)   //系数大于0
    {
    if((p->data).expn==0)   //指数等于0,输出系数
         cout<<(p->data).coef;
       else if((p->data).coef==1&&(p->data).expn==1)  //系数等于1，指数等于1
           cout<<"x";
       else if((p->data).coef==1&&(p->data).expn!=1)   //系数等于1，指数不等于1
              cout<<"x^"<<(p->data).expn;
       else  if((p->data).expn==1&&(p->data).coef!=1)   //系数不等于1，指数等于1
              cout<<(p->data).coef<<"x";
       else cout<<(p->data).coef<<"x^"<<(p->data).expn;  //输出其他情况
    }
     if((p->data).coef<0)   //系数小于0
      {
       if((p->data).expn==0)   //指数等于0
         cout<<(p->data).coef;
       else if(p->data.coef==-1&&p->data.expn==1)  //系数等于-1，指数等于1
        cout<<"-x";
          else if(p->data.coef==-1&&p->data.expn!=1)  //系数等于-1，指数不等于1
              cout<<"-x^"<<p->data.expn;
       else if(p->data.expn==1)   //指数等于1
              cout<<p->data.coef<<"x";
          else  cout<<(p->data).coef<<"x^"<<(p->data).expn;  //输出其他情况
      }
  p=p->next;
  //非第一项元素
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

/*把一个链表的内容复制给另一个链表*/
void copyLink(Link &pc,Link pa)
{
 Link p,q,r;
 pc=new LNode;
 pc->next=NULL;
 r=pc;
 p=pa;
 while(p->next!=NULL)  //遍历pa链表，直至全部拷贝完
 {q=new LNode;   //临时变量
  q->data.coef=p->next->data.coef;   //系数拷贝
  q->data.expn=p->next->data.expn;   //指数拷贝
  r->next=q;     //尾插法
  q->next=NULL;
  r=q;
  p=p->next;  //下一个节点
 }
}


/*将两个一元多项式相加*/
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
 while(p1!=NULL&&p2!=NULL)  //两个多项式都不为空
 {
  if(p1->data.expn<p2->data.expn)   //第一个指数小于第二个指数
  {
   p->next=p1;   //将一个多项式的元素插入PC链表中
   p=p->next;    //向后移动
   p1=p1->next;   //遍历下一个
  }
  else if(p1->data.expn>p2->data.expn)   //第一个指数大于第二个指数
  {
   p->next=p2;
   p=p->next;
   p2=p2->next;
  }
  else    //第一个指数等于第二个指数，做系数加法
  {
  // newp=new LNode;
   p1->data.coef=p1->data.coef+p2->data.coef;
   //p1->data.expn=p1->data.expn;
   if(p1->data.coef!=0)   //当加完之后系数不等于0，则插入链表中
   {
    p->next=p1;
    p=p->next;
    p1=p1->next;
    p2=p2->next;
   }
   else   //否则遍历一下个
   {
    pd=p1;
    p1=p1->next;
    p2=p2->next;
    delete pd;
   }

  }


 }
 if(p1!=NULL)  //第一个多项式不为空，第二个为空
 {
  p->next=p1;  //将第一个剩余的都插入到PC链表中
 }
 if(p2!=NULL)
 {
  p->next=p2;
 }

}

/*将两个多项式相减*/
void substractPolyn(Link &pc,Link pa,Link pb)
{
  Link p,pt;
  copyLink(pt,pb);   //拷贝链表
  p=pt;
  while(p!=NULL)  //遍历第二个链表
  {
     (p->data).coef=(-(p->data).coef);   //将第二个链表系数全部置反
        p=p->next;   //遍历下一个元素
  }
  addPolyn(pc,pa,pt);   //做加法运算
  destroyLink(pt);   //销毁临时链表
}
//清屏函数
void Clear()
{
 char a;
 cout<<"请按 回车键 继续……"<<endl;
 a=getchar();   //接收字符
 system("cls");
}

/*将两个一元多项式相乘*/
void mulPolyn(Link &pc,Link pa,Link pb)
{
 Link p1,p2,p,pd,newp,t;
 pc=new LNode;
 pc->next=NULL;
 p1=pa->next;  //第一个多项式
 p2=pb->next;  //第二个多项式
 while(p1!=NULL)   //遍历第一个多项式全部项
 {
  pd=new LNode;
     pd->next=NULL;
  p=new LNode;
  p->next=NULL;
  t=p;
  while(p2)   //遍历第二个多项式全部项
  {
   newp=new LNode;
   newp->next=NULL;  //每一项系数和指数做运算
   newp->data.coef=p1->data.coef*p2->data.coef;  //系数乘系数
   newp->data.expn=p1->data.expn+p2->data.expn;   //指数相加
   t->next=newp;
   t=t->next;
   p2=p2->next;  //第二个多项式的下一个元素
  }
  addPolyn(pd,pc,p);   //每个运算一次乘法都与结果链表做加法
  copyLink(pc,pd);
  p1=p1->next;  //遍历第一个多项式的下一个元素
  p2=pb->next;  //重新定位P2指针为第二个多项式的首元素
  destroyLink(p);
  destroyLink(pd);
 }
}



 //菜单函数
void menuPrint()
{

 cout<<"\t***********一元多项式的简单运算*************"<<endl;
 cout<<"\t\t 1创建要运算的两个一元多项式"<<endl;
 cout<<"\t\t 2将两个一元多项式相加"<<endl;
 cout<<"\t\t 3将两个一元多项式相减"<<endl;
 cout<<"\t\t 4将两个一元多项式相乘"<<endl;
 cout<<"\t\t 5显示两个一元多项式"<<endl;
 cout<<"\t\t 6销毁所创建的二个多项式"<<endl;
 cout<<"\t\t 7退出"<<endl;
 cout<<"请输入你要进行的操作（1-7）"<<endl;
}

//判断输入的整数是不是字符
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
      cout<<"请输入你要运算的第一个一元多项式的项数:"<<endl;
            cin>>n;
   if(compareNum(n)==0){cout<<"您的输入有误，请重新输入……"<<endl;Clear();break;}
            createLink(La,n);
            cout<<"请输入你要运算的第二个一元多项式的项数:"<<endl;
            cin>>n;
   if(compareNum(n)==0){cout<<"您的输入有误，请重新输入……"<<endl;Clear();break;}
            createLink(Lb,n);
   Clear();
          break;
    case 2:
    if(La==NULL||Lb==NULL){cout<<"您的多项式创建有误，请重新选择……"<<endl;Clear();break;}
       addPolyn(L,La,Lb);
          cout<<"相加的两个一元多项式为："<<endl;
             printList(La);
          printList(Lb);
          cout<<"相加后的结果为："<<endl;
          printList(L);
    Clear();
    destroyLink(L);
             break;
    case 3:
     if(La==NULL||Lb==NULL){cout<<"您的多项式创建有误，请重新选择……"<<endl;Clear();break;}
      substractPolyn(L,La,Lb);
      cout<<"相减的两个一元多项式为："<<endl;
            printList(La);
         printList(Lb);
       cout<<"相减后的结果为："<<endl;
       printList(L);
   Clear();
   destroyLink(L);
      break;
    case 4:
     if(La==NULL||Lb==NULL){cout<<"您的多项式创建有误，请重新选择……"<<endl;Clear();break;}
   mulPolyn(L,La,Lb);
            cout<<"相乘的两个一元多项式为："<<endl;
            printList(La);
         printList(Lb);
      cout<<"相乘后的结果为："<<endl;
      printList(L);
   destroyLink(L);
   Clear();
      break;
    case 5:
     if(La==NULL||Lb==NULL){cout<<"您的多项式创建有误，请重新选择……"<<endl;Clear();break;}
              cout<<"第一个一元多项式为："<<endl;
           printList(La);
            cout<<"第二个一元多项式为："<<endl;
              printList(Lb);
     Clear();
              break;
    case 6:
     if(La&&Lb){destroyLink(La);destroyLink(Lb);cout<<"销毁成功！"<<endl;Clear();}
     else
     { cout<<"多项式不存在，请重新选择^^^"<<endl;Clear();}
           break;
    case 7:
     exit(0);
    default:
     cout<<"您的输入有误，请重新选择操作……"<<endl;Clear();
     break;
    }

   }
   return 0;
}
