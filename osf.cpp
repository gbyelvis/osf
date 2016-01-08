//
//  osf.cpp
//  OSFP
//
//  Created by james gavin on 16/1/6.
//  Copyright ? 2016年 james gavin. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>

#define MAXNAME 25  /*the largest length of mfdname,ufdname,filename*/
#define MAXCHILD 50 /*the largest child*/
#define MAX (MAXCHILD*MAXCHILD) /*the size of fpaddrno*/

typedef struct  /*the structure of OSFILE*/
{
    int  fpaddr;                /*file physical address*/
    int  flength;               /*file length*/
    int  fmode;   /*file mode:0-Read Only;1-Write Only;2-Read and Write; 3-Protect;*/
    char fname[MAXNAME];        /*file name*/
} OSFILE;

typedef struct     /*the structure of OSUFD*/
{
    char ufdname[MAXNAME];   /*ufd name*/
    OSFILE ufdfile[MAXCHILD];   /*ufd own file*/
}OSUFD;

typedef struct  /*the structure of OSUFD'LOGIN*/
{
    char ufdname[MAXNAME];       /*ufd name*/
    char ufdpword[8];            /*ufd password*/
} OSUFD_LOGIN;

typedef struct     /*file open mode*/
{
    int ifopen;     /*ifopen:0-close,1-open*/
    int openmode;   /*0-read only,1-write only,2-read and write,3-initial*/
}OSUFD_OPENMODE;

OSUFD *ufd[MAXCHILD];   /*ufd and ufd own files*/
OSUFD_LOGIN ufd_lp;

int ucount=0;  /*the count of mfd's ufds*/
int fcount[MAXCHILD];  /*the count of ufd's files*/
int loginsuc=0; /*whether login successfully*/
char username[MAXNAME];  /*record login user's name22*/
char dirname[MAXNAME];/*record current directory*/
int fpaddrno[MAX];  /*record file physical address num*/
OSUFD_OPENMODE ifopen[MAXCHILD][MAXCHILD]; /*record file open/close*/
int wgetchar; /*whether getchar()*/

FILE *fp_mfd,*fp_ufd,*fp_file_p,*fp_file;

void LoginF();  /*LOGIN FileSystem*/
void DirF();  /*Dir FileSystem*/
void CdF();  /*Change Dir*/
void CreateF();  /*Create File*/
void DeleteF(); /*Delete File*/
void ModifyFM(); /*Modify FileMode*/
void OpenF();  /*Open File*/
void CloseF();  /*Close File*/
void ReadF(); /*Read File*/
void WriteF(); /*Write File*/
void QuitF(); /*Quit FileSystem*/
void help();

char *rtrim(char *str);  /*remove the trailing blanks.*/
char *ltrim(char *str);  /*remove the heading blanks.*/
void InputPW(char *password);  /*input password,use '*' replace*/
void SetPANo(int RorW);  /*Set physical address num*/
int ExistD(char *dirname);  /*Whether DirName Exist,Exist-i,Not Exist-0*/
int WriteF1(); /*write file*/
int ExistF(char *filename);  /*Whether FileName Exist,Exist-i,Not Exist-0*/
int FindPANo();  /*find out physical address num*/



void clrscr()
{
    system("cls");    //清屏
}

int main()
{
    int i,choice1;
    char choice[50];  /*choice operation:dir,create,delete,open,delete,modify,read,write*/
    int choiceend=1;  /*whether choice end*/
    char *rtrim(char *str);  /*remove the trailing blanks.*/
    char *ltrim(char *str);  /*remove the heading blanks.*/

    if((fp_mfd=fopen("c:\\osfile\\mfd.txt","rb"))==NULL)    //读取存储账户密码文件
    {
        fp_mfd=fopen("c:\\osfile\\mfd.txt","wb");           //如果不存在，则创建文件
        fclose(fp_mfd);                                     //关闭文件指针
    }

    for(i=0;i<MAX;i++) fpaddrno[i]=0;                       //给物理地址初始化为0
    //textattr(BLACK*16|WHITE);
    clrscr();   /*clear screen*/                            //清屏
    LoginF();   /*user login*/                              //用户登入
    clrscr();                                               //清屏

    if(loginsuc==1)  /*Login Successfully*/
    {
        while (1)
        {
            wgetchar=0;                                    //是否接收尾子符
            if (choiceend==1)
                printf("\n\nC:\\%s>",strupr(dirname));       //显示当前路径
            else
                printf("Bad command or file name.\nC:\\%s>",strupr(username));   //输入命令错误
            gets(choice);                                              //接收命令
            strcpy(choice,ltrim(rtrim(strlwr(choice))));                //将去除首尾空格的字符串拷贝到choise
            if (strcmp(choice,"dir")==0) choice1=1;
            else if(strcmp(choice,"create")==0) choice1=2;
            else if(strcmp(choice,"delete")==0) choice1=3;
            else if(strcmp(choice,"attrib")==0) choice1=4;
            else if(strcmp(choice,"open")==0) choice1=5;
            else if(strcmp(choice,"close")==0) choice1=6;
            else if(strcmp(choice,"read")==0) choice1=7;
            else if(strcmp(choice,"write")==0) choice1=8;
            else if(strcmp(choice,"exit")==0) choice1=9;
            else if(strcmp(choice,"cls")==0) choice1=10;
            else if(strcmp(choice,"cd")==0) choice1=11;
            else if(strcmp(choice,"help")==0) choice1=20;
            else choice1=12;                                            //其他无效命令

            switch(choice1)                                             //进入各种命令
            {
                case 1:DirF();choiceend=1;break;
                case 2:CreateF();choiceend=1;if(!wgetchar) getchar();break;     //是否有回车符前导
                case 3:DeleteF();choiceend=1;if(!wgetchar)getchar();break;
                case 4:ModifyFM();choiceend=1;if(!wgetchar) getchar();break;
                case 5:OpenF();choiceend=1;if (!wgetchar) getchar();break;
                case 6:CloseF();choiceend=1;if (!wgetchar) getchar();break;
                case 7:ReadF();choiceend=1;if (!wgetchar) getchar();break;
                case 8:WriteF();choiceend=1;if (!wgetchar) getchar();break;
                case 9:printf("\nYou have exited this system.");
                    QuitF();exit(0);break;                                     //退出系统
                case 10:clrscr();choiceend=1;break;
                case 11:CdF();choiceend=1;break;
                case 20:help();choiceend=1;break;
                default:choiceend=0;                                            //无效命令
            }
        }
    }
    else
        printf("\nAccess denied.");                                            //无权限
    return 0;
}

void help(void)                                                           //帮助命令
{
    printf("\nThe Command List\n");
    printf("\nCd  Attrib  Create  write  Read  Open  Cls  Delete  Exit  Close\n");    //当前拥有的命令
}

char *rtrim(char *str)  /*remove the trailing blanks.*/
{
    int n=strlen(str)-1;                 //存取字符串的长度
    while(n>=0)                          //遍历整个串
    {
        if(*(str+n)!=' ')                //判断最后一个是否为空
        {
            *(str+n+1)='\0';             //若不为空，则n+1个字符为'\0'
            break;
        }
        else n--;                        //若为空，则往前导
    }
    if (n<0) str[0]='\0';                //若整个串为空，则第一个字符加上'\0'
    return str;                          //返回这个字符串
}

char *ltrim(char *str) /*remove the heading blanks.*/
{
    strrev(str);                         //反转字符串
    rtrim(str);                          //去除尾部空格
    strrev(str);                         //反转字符串
    return str;                          //返回字符串
}

void LoginF()  /*LOGIN FileSystem*/
{
    char loginame[MAXNAME],loginpw[9],logincpw[9],str[50];     //设置局部变量
    int i,j,flag=1;
    char a[25];
    int findout; /*login user not exist*/

    while(1)
    {
        findout=0;
        printf("\n\nLogin Name:");                      //提示信息
        gets(loginame);                                 //接收字符串
        ltrim(rtrim(loginame));                         //去除首尾空格
        fp_mfd=fopen("c:\\osfile\\mfd.txt","rb");       //打开账户密码文件
        for(i=0;fread(&ufd_lp,sizeof(OSUFD_LOGIN),1,fp_mfd)!=0;i++)   //读取用户信息，直到
            if (strcmp(strupr(ufd_lp.ufdname),strupr(loginame))==0)   //匹配用户名
            {
                findout=1;                                            //已匹配到用户
                strcpy(logincpw,ufd_lp.ufdpword);                     //拷贝用户密码到临时变量
        fclose(fp_mfd);
        if (findout==1)  /*user exist*/
        {
            printf("Login Password:");
            InputPW(loginpw); /*input password,use '*' replace*/
            if (strcmp(loginpw,logincpw)==0)                        //匹配用户密码
            {
                strcpy(username,strupr(loginame));                   //拷贝到username变量
                strcpy(dirname,username);                            //拷贝到dirname变量
                fp_mfd=fopen("c:\\osfile\\mfd.txt","rb");
                for(j=0;fread(&ufd_lp,sizeof(OSUFD_LOGIN),1,fp_mfd)!=0;j++)   //遍历账户文件
                {
                    strcpy(str,"c:\\osfile\\");           //添加字符串到str
                    strcat(str,ufd_lp.ufdname);             //用户名
                    strcat(str,".txt");                     //追加到str
                    ufd[j]=(OSUFD*)malloc(sizeof(OSUFD));    //申请空间
                    strcpy(ufd[j]->ufdname,strupr(ufd_lp.ufdname));  //记录用户信息
                    fp_ufd=fopen(str,"rb");     //打开用户文件目录
                    fcount[j]=0;     //记录用户文件数量
                    for(i=0;fread(&ufd[j]->ufdfile[i],sizeof(OSFILE),1,fp_ufd)!=0;i++,fcount[j]++)   //遍历用户文件目录
                    {
                        ifopen[j][i].ifopen=0;                   //设置用户文件默认为关闭状态
                        ifopen[j][i].openmode=4;                 //设置访问模式为4
                    }
                    fclose(fp_ufd);     //关闭用户文件指针
                }

                fclose(fp_mfd);        //关闭账户文件指针
                ucount=j;              //用户文件个数

                SetPANo(0);            //初始化用户文件的物理储存
                printf("\n\nLogin successful! Welcome to this FileSystem\n\n");
                loginsuc=1;            //登入成功
                return;
            }
            else
            {
                printf("\n\n");
                flag=1;
                while(flag)   //循环判断
                {
                    printf("Login Failed!  Password Error.  Try Again(Y/N):");
                    gets(a);     //接收字符
                    ltrim(rtrim(a));
                    if (strcmp(strupr(a),"Y")==0)    //重新输入密码
                    {
                        loginsuc=0;
                        flag=0;
                    }
                    else if(strcmp(strupr(a),"N")==0)    //返回登入失败
                    {
                        loginsuc=0;
                        flag=0;
                        return;
                    }
                }
            }
        }
        else                          //注册新用户
        {
            printf("New Password(<=8):");      //输入密码
            InputPW(loginpw); /*input new password,use '*' replace*/
            printf("\nConfirm Password(<=8):"); /*input new password,use '*' replace*/
            InputPW(logincpw);
            if (strcmp(loginpw,logincpw)==0)     //比较两次密码是否一致
            {
                strcpy(ufd_lp.ufdname,strupr(loginame));      //记录登入用户信息
                strcpy(ufd_lp.ufdpword,loginpw);
                fp_mfd=fopen("c:\\osfile\\mfd.txt","ab");      //以追加文件信息打开文件
                fwrite(&ufd_lp,sizeof(OSUFD_LOGIN),1,fp_mfd);  //写入新用户数据
                fclose(fp_mfd);
                strcpy(username,strupr(loginame));        //记录信息到username、dirname
                strcpy(dirname,loginame);

                ////////////////////////////////////////////////////////
                strcpy(str,"c:\\osfile\\");
                strcat(str,username);
                strcat(str,".txt");       //用户文件路径
                if((fp_ufd=fopen(str,"rb"))==NULL)     //打开用户文件
                {
                    fp_ufd=fopen(str,"wb");            //如果不存在，就创建用户文件
                    fclose(fp_ufd);
                }
                fp_mfd=fopen("c:\\osfile\\mfd.txt","rb");
                for(j=0;fread(&ufd_lp,sizeof(OSUFD_LOGIN),1,fp_mfd)!=0;j++)
                {
                    /////////////////////////////////////
                    strcpy(str,"c:\\osfile\\");
                    strcat(str,ufd_lp.ufdname);
                    strcat(str,".txt");
                    ufd[j]=(OSUFD*)malloc(sizeof(OSUFD));
                    strcpy(ufd[j]->ufdname,strupr(ufd_lp.ufdname));
                    fp_ufd=fopen(str,"rb");
                    for(i=0;fread(&ufd[j]->ufdfile[i],sizeof(OSFILE),1,fp_ufd)!=0;i++,fcount[j]++)
                    {
                        ifopen[j][i].ifopen=0;
                        ifopen[j][i].openmode=4;
                    }
                    fclose(fp_ufd);
                }
                fclose(fp_mfd);
                ucount=j;
                SetPANo(0);
                printf("\n\nLogin Successful! Welcome to this System\n\n");
                loginsuc=1;
                return;
            }
            else
            {
                printf("\n\n");
                flag=1;
                while(flag)
                {
                    printf("Login Failed! Password Error. Try Again(Y/N):");
                    gets(a);
                    ltrim(rtrim(a));
                    if (strcmp(strupr(a),"Y")==0)
                    {
                        loginsuc=0;
                        flag=0;
                    }
                    else if(strcmp(strupr(a),"N")==0)
                    {
                        loginsuc=0;
                        flag=0;
                        return;
                    }
                    }
                }
            }
        }
    }
}

void SetPANo(int RorW)  /*Set physical address num,0-read,1-write*/
{
    int i,j;
    if (RorW==0)
    {
        if((fp_file_p=fopen("c:\\osfile\\file\\file_p.txt","rb"))==NULL)    //判断文件是否存着
        {
            fp_file_p=fopen("c:\\osfile\\file\\file_p.txt","wb");    //不存在，创建文件
            fclose(fp_file_p);
        }
        fp_file_p=fopen("c:\\osfile\\file\\file_p.txt","rb");        //打开物理块号记录文件

        ///////////////////////////////////////////////////////////////////
        for(i=0;fread(&j,sizeof(int),1,fp_file_p)!=0;i++)       //将存在的块号记录为1
            fpaddrno[j]=1;

        /*for(i=1;i<MAX;i++)
         if ((i%13)==0)
         fpaddrno[i]=1;*/
    }
    else
    {
        fp_file_p=fopen("c:\\osfile\\file\\file_p.txt","wb");  //打开物理块号记录文件
        /*for(i=1;i<MAX;i++)
         if((i%13)==0)
         fpaddrno[i]=0;*/
        for(i=0;i<MAX;i++)
            if (fpaddrno[i]==1)
                fwrite(&i,sizeof(int),1,fp_file_p);   //将工作的物理块号记录
    }
    fclose(fp_file_p);
}

void InputPW(char *password)  /*input password,use '*' replace*/
{
    int j;
    for(j=0;j<=7;j++)
    {
        password[j]=getch();         //接收字符
        if ((int)(password[j])!=13)    //是否是回车符
        {
            if((int)(password[j])!=8)   //是否是退格符
                putchar('*');           //不是显示‘*’
            else
            {
                if (j>0)
                {
                    j--;
                    j--;
                    putchar('\b');putchar(' ');putchar('\b');   //删除之前的‘*’
                }
                else j--;
            }
        }
        else
        {	password[j]='\0';      //在接收子串后面加子串结束符
            break;
        }
    }
    password[j]='\0';     //当满了8位时自动添加子串结束符
}

void DirF()  /*Dir FileSystem*/
{
    int i,j,count=0;
    char sfmode[25],sfpaddr[25],str[25];     //定义三个字符串数组变量

    clrscr();                                //清屏
    if (strcmp(strupr(ltrim(rtrim(dirname))),"")!=0)    //比较当前文件目录
    {
        printf("\n\nC:\\%s>dir\n",dirname);             //显示
        printf("\n%14s%16s%14s%10s%18s\n","FileName","FileAddress","FileLength","Type","FileMode");    //显示文件信息标题栏
        j=ExistD(dirname);                             //用户存储位置
        for(i=0;i<fcount[j];i++)                       //遍历用户目录下文件
        {
            if ((i%16==0)&&(i!=0))
            {
                printf("\nPress any key to continue..");
                getch();
                clrscr();
                printf("\n%14s%16s%14s%10s%18s\n","FileName","FileAddress","FileLength","Type","FileMode");
            }
            /////////////////////////////////////////////////////////
            itoa(ufd[j]->ufdfile[i].fpaddr,str,10);     //用户文件物理块号
            strcpy(sfpaddr,"file");
            strcat(sfpaddr,str);
            if (ufd[j]->ufdfile[i].fmode==0) strcpy(sfmode,"Read Only");
            else if(ufd[j]->ufdfile[i].fmode==1) strcpy(sfmode,"Write Only");
            else if(ufd[j]->ufdfile[i].fmode==2)strcpy(sfmode,"Read And Write");
            else strcpy(sfmode,"Protect");
            printf("%14s%16s%14d%10s%18s\n",ufd[j]->ufdfile[i].fname,sfpaddr,ufd[j]->ufdfile[i].flength,"<FILE>",sfmode);   //显示用户文件信息
        }
        printf("\n %3d file(s)\n",fcount[j]);   //用户文件个数
    }
    else
    {
        printf("\n\nC:\\>dir\n");           //显示根目录
        printf("\n%14s%18s%8s\n","DirName","OwnFileCount","Type");    //显示用户文件信息
        for(i=0;i<ucount;i++)
        {
            if ((i%16==0)&&(i!=0))
            {
                printf("\nPress any key to continue...");
                getch();
                clrscr();
                printf("\n%14s%18s%8s\n","DirName","OwnFileCount","Type");
            }
            printf("%14s%18d%8s\n",ufd[i]->ufdname,fcount[i],"<UFD>");
            count=count+fcount[i];
        }
        printf("\n %3d dir(s),%5d file(s)\n",ucount,count);   //显示用户个数
    }
}

int ExistD(char *dirname)  /*Whether DirName Exist,Exist-i,Not Exist-0*/
{
    int i;
    int exist=0;
    for(i=0;i<ucount;i++)      //找出用户信息所存储的位置
        if (strcmp(strupr(ufd[i]->ufdname),strupr(dirname))==0)
        {
            exist=1;
            break;
        }
    if (exist) return(i);    //返回存储位置
    else return(-1);
}

void CdF() /*Exchange Dir*/
{
    char dname[MAXNAME];

    printf("\nPlease input DirName (cd..-Previous dir; DirNAME-cd [DirNAME]):");
    gets(dname);
    ltrim(rtrim(dname));
    if (ExistD(dname)>=0)  strcpy(dirname,strupr(dname));
    else
        if(strcmp(strupr(dname),"CD..")==0)  strcpy(ltrim(rtrim(dirname)),"");
        else printf("\nError.\'%s\' does not exist.\n",dname);
}

void CreateF()  /*Create File*/
{
    int fpaddrno,flag=1,i;
    char fname[MAXNAME],str[50],str1[50],a[25];
    char fmode[25];


    if (strcmp(strupr(dirname),strupr(username))!=0)           //判读用户是不是在当前目录下
    {
        printf("\nError. You must create file in your own dir.\n");
        wgetchar=1;
    }
    else
    {
        printf("\nPlease input FileName:");
        gets(fname);                              //接收文件名
        ltrim(rtrim(fname));
        if (ExistF(fname)>=0)                     //文件是否存在
        {
            printf("\nError. Name \'%s\' has already existed.\n",fname);
            wgetchar=1;
        }
        else      //文件不存在
        {
            printf("Please input FileMode(0-Read Only, 1-Write Only, 2-Read and Write, 3-Protect):");
            gets(fmode);
            ltrim(rtrim(fmode));          //输入文件访问权限
            if((strcmp(fmode,"0")==0)||(strcmp(fmode,"1")==0)||(strcmp(fmode,"2")==0)||(strcmp(fmode,"3")==0))
            {
                fpaddrno=FindPANo();    //找到可存储的物理块号
                if (fpaddrno>=0)
                {
                    i=ExistD(username);      //用户位置
                    strcpy(ufd[i]->ufdfile[fcount[i]].fname,fname);        //用户文件信息存储
                    ufd[i]->ufdfile[fcount[i]].fpaddr=fpaddrno;
                    ufd[i]->ufdfile[fcount[i]].fmode=atoi(fmode);
                    ifopen[i][fcount[i]].ifopen=0;
                    ifopen[i][fcount[i]].openmode=4;
                    strcpy(str,"c:\\osfile\\file\\file");    //创建用户文件
                    itoa(fpaddrno,str1,10);
                    strcat(str,str1);
                    strcat(str,".txt");
                    fp_file=fopen(str,"wb");
                    fclose(fp_file);
                    fcount[i]++;    //用户文件个数增加
                    while(flag)       //循环判断
                    {
                        printf("Input text now(Y/N):");
                        gets(a);
                        ltrim(rtrim(a));
                        ufd[i]->ufdfile[fcount[i]-1].flength=0;   //当前创建文件长度为0
                        if(strcmp(strupr(a),"Y")==0)       //写入文件内容
                        {
                            fp_file=fopen(str,"wb+");
                            ufd[i]->ufdfile[fcount[i]-1].flength=WriteF1();    //写入内容，并记录文件长度
                            flag=0;    //跳出循环
                        }
                        else if(strcmp(strupr(a),"N")==0)
                        {
                            flag=0;
                            wgetchar=1;
                        }
                    }
                    printf("\n\'%s\' has been created successfully!\n",fname);
                }
                else
                {
                    printf("\nFail!No Disk Space. Please format your disk.\n");   //没有物理空间
                    wgetchar=1;
                }
            }
            else
            {
                printf("\nError. FileMode\'s Range is 0-3\n");    //文件访问模式
                wgetchar=1;
            }
        }
    }
}

int ExistF(char *filename)  /*Whether FileName Exist,Exist-i,Not Exist-0*/
{
    int i,j;
    int exist=0;

    j=ExistD(dirname);
    for(i=0;i<fcount[j];i++)                //遍历文件是否存在，并返回位置
        if (strcmp(strupr(ufd[j]->ufdfile[i].fname),strupr(filename))==0)
        {
            exist=1;
            break;
        }
    if (exist) return(i);
    else return(-1);
}

int FindPANo()  /*find out physical address num*/
{
    int i;
    for(i=0;i<MAX;i++)             //遍历数组找到第一个不为零的物理块号
        if (fpaddrno[i]==0)
        {
            fpaddrno[i]=1;
            break;
        }
    if (i<MAX) return(i);     //返回空闲物理块号
    else return(-1);
}

int WriteF1()  /*write file*/
{
    int length=0;
    char c;
    printf("Please input text(\'#\' stands for end):\n");
    while((c=getchar())!='#')                //遇到‘#’停止输入
    {
        fprintf(fp_file,"%c",c);     //写入文件
        if (c!='\n') length++;       //统计长度
    }
    fprintf(fp_file,"\n");           //写入回车符
    fclose(fp_file);
    return(length);
}

void DeleteF() /*Delete File*/
{
    printf("\n\nC:\\%s>",strupr(dirname));    //显示路径
    char fname[MAXNAME], str[50], str1[50];    //定义三个字符串变量
    int i, k, j;
    int fpaddrno1;        //记录文件物理地址块号
    if(strcmp(strupr(ltrim(rtrim(dirname))), "")==0){      //判断用户是否在用户目录下
        printf("\nError.Please convert to ufd dir before delete. \n");
        wgetchar=1;
    }
    if(strcmp(strupr(dirname), strupr(username))!=0){
        printf("\nError.You can only modify filemode in yourself dir.\n");
        wgetchar=1;
    } else {
        printf("\nPlease input FileName:");
        gets(fname);                //接收删除的文件名
        ltrim(rtrim(fname));
        i=ExistF(fname);     //用户文件位置
        if(i>=0){
            k=ExistD(username);    //获取用户所在存储位置
            if(ifopen[k][i].ifopen==1){           //文件状态处于打开状态，不许删除
                printf("\n Error.\'%s\' is in open status. Close it before delete.\n", fname);
                wgetchar=1;
            } else {
                if(ufd[k]->ufdfile[i].fmode==3){     //保护文件，不让删除
                    printf("\nError.\'%s\'is in protect status. Close it before delete.\n", fname);
                    wgetchar=1;
                } else {
                    fpaddrno1=ufd[k]->ufdfile[i].fpaddr;      //获取文件的物理地址块号
                    fpaddrno[fpaddrno1]=0;    //回收物理地址块号
                    for(j=i; j<fcount[k]; j++)             //将文件都向前移动
                    {
                        ufd[k]->ufdfile[j]=ufd[k]->ufdfile[j+1];
                    }
                    strcpy(str , "c:\\osfile\\file\\");
                    itoa(fpaddrno1, str1, 10);     //将整数转化为字符串
                    strcat(str, str1);
                    strcat(str, ".txt");
                    remove(str);                  //删除物理文件
                    fcount[k--];                   //文件个数减一
                    printf("\n\'%s\'is deleted successfully.\n", fname);
                    wgetchar=1;
                }
            }
        } else {
            printf("\nError.\'%s\' dose not exist.\n", fname);     //文件不存在
            wgetchar=1;
        }
    }
}

void ModifyFM() /*Modify FileMode*/
{
    char fname[MAXNAME],str[50];    //定义两个字符串
    int i,k;
    char fmode[25]; /*whether delete*/

    if (strcmp(strupr(dirname),strupr(username))!=0)      //修改用户文件需要在用户目录下
    {
        printf("\nError.You can only modify filemode in yourself dir.\n");
        wgetchar=1;
    }
    else
    {
        printf("\nPlease input FileName:");
        gets(fname);          //获取需要修改的文件名
        ltrim(rtrim(fname));    //去除首尾的空格
        i=ExistF(fname);       //获取文件的物理地址
        if (i>=0)
        {
            k=ExistD(username);    //获取用户的文件地址
            if(ifopen[k][i].ifopen==1)    //文件已被打开，不允许被修改
            {
                printf("\nError.\'%s\' is in open status. Close it before modify.\n",fname);
                wgetchar=1;
            }
            else
            {
                if(ufd[k]->ufdfile[i].fmode==0) strcpy(str,"read only");    /*FileMode*/    //显示当前文件信息
                else if(ufd[k]->ufdfile[i].fmode==1) strcpy(str,"write only");
                else if(ufd[k]->ufdfile[i].fmode==2) strcpy(str,"read and write");
                else strcpy(str,"Protect");

                printf("\'%s\' filemode is %s.\n",fname,strupr(str));
                printf("Modify to(0-read only,1-write only,2-read and write,3-Protect):");   //提示信息
                gets(fmode);       //接收访问权限数字
                ltrim(rtrim(fmode));
                if(strcmp(fmode,"0")==0)    //只读
                {
                    ufd[k]->ufdfile[i].fmode=0;
                    printf("\n\'%s\' has been modified to READ ONLY mode successfully.\n",fname);
                    wgetchar=1;
                }
                else if(strcmp(fmode,"1")==0)    //只写
                {
                    ufd[k]->ufdfile[i].fmode=1;
                    printf("\n\'%s\' has been modified to WRITE ONLY mode successfully.\n",fname);
                    wgetchar=1;
                }
                else if(strcmp(fmode,"2")==0)    //读写
                {
                    ufd[k]->ufdfile[i].fmode=2;
                    printf("\n\'%s\' has been modified to READ AND WRITE mode successfully.\n",fname);
                    wgetchar=1;
                }
                else if(strcmp(fmode,"3")==0)    //受保护
                {
                    ufd[k]->ufdfile[i].fmode=3;
                    printf("\n\'%s\' has been modified to FORBID mode successfully.\n",fname);
                    wgetchar=1;
                }
                else
                {
                    printf("\nError.\'%s\' is not modified.\n",fname);
                    wgetchar=1;
                }
            }
        }
        else
        {
            printf("\nError. \'%s\' dose not exist.\n",fname);   //所需要修改文件不存在
            wgetchar=1;
        }
    }
}


void OpenF() /*Open File*/
{
    printf("\n\nC:\\%s>",strupr(dirname));    //显示当前路径
    int fcoun, i;                     //定义两个整形变量
    char fname[MAXNAME], fmode[25];  //定义两个字符串变量
    int fmod;                         //文件模式
    printf("\nPlease input FileName:");
    gets(fname);         //接收打开文件的文件名
    ltrim(rtrim(fname));
    if(ExistF(fname)<0)     //文件是否存在
    {
        printf("\nError.FileName \'%s\' is not existed.\n", fname);
        wgetchar=1;
    } else {
        i=ExistD(username);    //获取用户物理信息
        for(int a=0; a < fcount[i]; a++)       //遍历用户文件
        {
            if(strcmp(fname, ufd[i]->ufdfile[a].fname)==0)   //找到文件
            {
                fcoun=a;
                break;
            }
        }
        ifopen[i][fcoun].ifopen=1;    //将文件状态置为打开状态
        printf("Please input OpenMode(0-Read Only, 1-Write Only, 2-Read and Write, 3-Protect:");   //打开文件模式
        gets(fmode);
        fmod=atoi(fmode);
        ifopen[i][fcoun].openmode=fmod;   //将文件的模式置为OpenMode
        printf("\nOpen Successed");
        wgetchar=1;
    }
}

void CloseF() /*Close File*/
{
    printf("\n\nC:\\%s>",strupr(dirname));   //显示路径
    char fname[MAXNAME];                     //定义字符串变量
    int i, k, n=0;

    if(strcmp(strupr(dirname), strupr(username))!=0)    //关闭用户文件需在用户目录下
    {
        printf("\nError,You can only modify filemode in yourself dir.\n");
    } else {
		printf("\n\nOpened File(s) List:\n");    //罗列已处于打开的文件
		k=ExistD(dirname);
		for(i=0;i<fcount[k];i++)
		{
			if (ifopen[k][i].ifopen==1)
				if ((ifopen[k][i].openmode==0) ||(ifopen[k][i].openmode==2))
				{
					printf("%15s",ufd[k]->ufdfile[i].fname);
					n++;
				}
			if((n%4==0)&&(n!=0)) printf("\n");
		}
		printf("\n%d files openned.\n",n);

		if (n==0) wgetchar=1;

        printf("\nPlease input FileName:");
        gets(fname);      //接收关闭文件的文件名
        ltrim(rtrim(fname));   //除去首尾空格
        i=ExistF(fname);   //获取文件物理地址
        if(i >= 0)
        {
            k=ExistD(username);     //获取用户文件信息
            if(ifopen[k][i].ifopen==0)    //文件处于关闭状态
            {
                printf("\nError.\'%s\'has been closed you can not close it again.\n", fname);
            } else {    //将文件关闭
                ifopen[k][i].ifopen=0;
                ifopen[k][i].openmode=4;
                printf("\'%s\' has been closed successfully!", fname);
            }
        } else {
            printf("\nError.\'%s\' does not exist.\n", fname);
        }
    }
}

void ReadF() /*Read File*/
{
    int i,k,n=0;        //定义三个整形变量
    char fname[MAXNAME];      //定义字符串变量
    char str[255],str1[255],c;     //定义字符串和字符变量
    if (strcmp(strupr(ltrim(rtrim(dirname))),"")==0)    //需要在用户目录下
    {
        printf("\nError.Please convert to ufd dir before read.\n");
        wgetchar=1;
        return;
    }
    printf("\nCaution:Open file first\n");
    printf("Opened File(s) List:\n");
    k=ExistD(dirname);      //获取用户文件信息
    for(i=0;i<fcount[k];i++)              //遍历用户文件，显示已经打开的文件
    {
        if (ifopen[k][i].ifopen==1)
            if ((ifopen[k][i].openmode==0) ||(ifopen[k][i].openmode==2))
            {
                printf("%15s",ufd[k]->ufdfile[i].fname);
                n++;
            }
        if((n%4==0)&&(n!=0)) printf("\n");
    }
    printf("\n%d files openned.\n",n);

    if (n==0) wgetchar=1;
    if(n!=0)    //有打开的文件
    {
        printf("\nPlease input FileName:");
        gets(fname);         //获取文件名
        ltrim(rtrim(fname));       //除去首尾空格
        i=ExistF(fname);        //获取文件物理地址
        if(i>=0)    //文件存在
        {
            if(ifopen[k][i].ifopen==1)            //处于打开状态
            {
                if((ifopen[k][i].openmode==0) ||(ifopen[k][i].openmode==2))   //处于只写和读写权限
                {
                    itoa(ufd[k]->ufdfile[i].fpaddr,str,10);     //读取文件的物理文件
                    strcpy(str1,"file");
                    strcat(str1,str);
                    strcpy(str,"c:\\osfile\\file\\");
                    strcat(str,str1);
                    strcat(str,".txt");
                    fp_file=fopen(str,"rb");
                    fseek(fp_file,0,0);                  //文件指针指向头部
                    printf("\nThe text is:\n\n");
                    printf("   ");
                    while(fscanf(fp_file,"%c",&c)!=EOF)     //以字符输出直至文件结束
                        if (c=='\n') printf("\n   ");
                        else printf("%c",c);
                    printf("\n\n%d Length.\n",ufd[k]->ufdfile[i].flength);    //输出文件长度
                    fclose(fp_file);     //关闭文件指针
                    wgetchar=1;
                }
                else
                {
                    printf("\nError.\'%s\' has been opened with WRITE ONLY mode. It isn\'t read.\n",fname);
                    wgetchar=1;
                }
            }
            else
            {
                printf("\nError.\'%s\' is in closing status. Please open it before read\n",fname);
                wgetchar=1;
            }
        }
        else
        {
            printf("\nError. \'%s\' does not exist.\n",fname);
            wgetchar=1;
        }
    }
}

void WriteF() /*Write File*/
{
    printf("\n\nC:\\%s>",strupr(dirname));   //显示用户路径
    int i, k, m=0;    //定义整形变量
    int length;        //定义长度整形变量
    char fname[MAXNAME];   //定义文件名字符串
    char str[255], str1[255];   //定义两个字符串变量
    if(strcmp(strupr(dirname), strupr(username))!=0) {     //用户是否在用户目录下
        printf("\nError.Please convert to ufd dir before write.\n");
        wgetchar=1;
        return;
    }
    printf("\nCaution:Open file first\n");
    printf("Opened File(s) List:\n");
    k=ExistD(dirname);   //获取用户文件信息
    for(i=0; i<fcount[k]; i++)    //遍历用户下的文件
    {
        if(ifopen[k][i].ifopen==1) {       //文件处于打开状态
            printf("%15s", ufd[k]->ufdfile[i].fname);
            m++;
        }
        if(m%4 == 0 && m!=0)
            printf("\n");
	}
    printf("\n%d files opened.\n", m);
    if(m == 0)
        wgetchar=1;
    if(m!=0)
    {
        printf("\nPlease input FileName:");
        gets(fname);     //接收文件名
        ltrim(rtrim(fname));
        i=ExistF(fname);    //获取文件物理地址
        if(i>=0) {    //文件存在
            if(ifopen[k][i].ifopen==1) {     //文件处于打开状态
                if(ifopen[k][i].openmode==1 || ifopen[k][i].openmode==2) {   //文件权限是只写或读写
                    itoa(ufd[k]->ufdfile[i].fpaddr, str, 10);    //获取文件路径
                    strcpy(str1, "file");
                    strcat(str1, str);
                    strcpy(str, "c:\\osfile\\file\\");
                    strcat(str, str1);
                    strcat(str, ".txt");    //文件路径
					char str2[3];
					int choice=3;
					strcpy(str2,"ab");
					printf("You can choise [0-Covered W] [1-Additonal W]:");     //追加还是覆盖
					scanf(" %d", &choice);
					if(choice == 0)
						strcpy(str2, "wb");
                    fp_file=fopen(str, str2);    //打开文件
                    length=WriteF1();
                    ufd[k]->ufdfile[i].flength=ufd[k]->ufdfile[i].flength+length;   //修改文件长度
					if(choice == 0)
						ufd[k]->ufdfile[i].flength=length;
                    printf("\n\nYou have write file successfully!!!");
					fclose(fp_file);
                    wgetchar=0;
                } else if(ifopen[k][i].openmode==0) {    //文件处于关闭状态，不允许写
                    printf("\nError.\'%s\' has been opened with Read Only mode. It isn\'t write.\n", fname);
                    wgetchar=1;
                } else {
					printf("\nError.\'%s\' is in closing status.Please open it before write\n", fname);
                    wgetchar=1;
                }
			}
        } else {
            printf("\nError.\'%s\' does not exist.\n", fname);
            wgetchar=1;
        }
    }
}

void QuitF() /*Quit FileSystem*/
{
    int i,j;
    char str[50];

    SetPANo(1);     //设置物理地址块号
    if (fp_mfd!=NULL) fclose(fp_mfd);    //关闭账户信息指针
    if (fp_ufd!=NULL) fclose(fp_ufd);     //关闭用户信息指针
    if (fp_file!=NULL) fclose(fp_file);    //关闭用户文件指针
    for(j=0;j<ucount;j++)        //遍历用户个数
    {
        strcpy(str,"c:\\osfile\\");
        strcat(str,ufd[j]->ufdname);
        ltrim(rtrim(str));
        strcat(str,".txt");
        fp_ufd=fopen(str,"wb");     //回写用户信息
        fclose(fp_ufd);
        fp_ufd=fopen(str,"ab");   //追加用户信息
        for(i=0;i<fcount[j];i++)
            fwrite(&ufd[j]->ufdfile[i],sizeof(OSFILE),1,fp_ufd);
        fclose(fp_ufd);
    }
}
