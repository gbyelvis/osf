//
//  osf.cpp
//  OSFP
//
//  Created by james gavin on 16/1/6.
//  Copyright ? 2016�� james gavin. All rights reserved.
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
    system("cls");    //����
}

int main()
{
    int i,choice1;
    char choice[50];  /*choice operation:dir,create,delete,open,delete,modify,read,write*/
    int choiceend=1;  /*whether choice end*/
    char *rtrim(char *str);  /*remove the trailing blanks.*/
    char *ltrim(char *str);  /*remove the heading blanks.*/

    if((fp_mfd=fopen("c:\\osfile\\mfd.txt","rb"))==NULL)    //��ȡ�洢�˻������ļ�
    {
        fp_mfd=fopen("c:\\osfile\\mfd.txt","wb");           //��������ڣ��򴴽��ļ�
        fclose(fp_mfd);                                     //�ر��ļ�ָ��
    }

    for(i=0;i<MAX;i++) fpaddrno[i]=0;                       //�������ַ��ʼ��Ϊ0
    //textattr(BLACK*16|WHITE);
    clrscr();   /*clear screen*/                            //����
    LoginF();   /*user login*/                              //�û�����
    clrscr();                                               //����

    if(loginsuc==1)  /*Login Successfully*/
    {
        while (1)
        {
            wgetchar=0;                                    //�Ƿ����β�ӷ�
            if (choiceend==1)
                printf("\n\nC:\\%s>",strupr(dirname));       //��ʾ��ǰ·��
            else
                printf("Bad command or file name.\nC:\\%s>",strupr(username));   //�����������
            gets(choice);                                              //��������
            strcpy(choice,ltrim(rtrim(strlwr(choice))));                //��ȥ����β�ո���ַ���������choise
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
            else choice1=12;                                            //������Ч����

            switch(choice1)                                             //�����������
            {
                case 1:DirF();choiceend=1;break;
                case 2:CreateF();choiceend=1;if(!wgetchar) getchar();break;     //�Ƿ��лس���ǰ��
                case 3:DeleteF();choiceend=1;if(!wgetchar)getchar();break;
                case 4:ModifyFM();choiceend=1;if(!wgetchar) getchar();break;
                case 5:OpenF();choiceend=1;if (!wgetchar) getchar();break;
                case 6:CloseF();choiceend=1;if (!wgetchar) getchar();break;
                case 7:ReadF();choiceend=1;if (!wgetchar) getchar();break;
                case 8:WriteF();choiceend=1;if (!wgetchar) getchar();break;
                case 9:printf("\nYou have exited this system.");
                    QuitF();exit(0);break;                                     //�˳�ϵͳ
                case 10:clrscr();choiceend=1;break;
                case 11:CdF();choiceend=1;break;
                case 20:help();choiceend=1;break;
                default:choiceend=0;                                            //��Ч����
            }
        }
    }
    else
        printf("\nAccess denied.");                                            //��Ȩ��
    return 0;
}

void help(void)                                                           //��������
{
    printf("\nThe Command List\n");
    printf("\nCd  Attrib  Create  write  Read  Open  Cls  Delete  Exit  Close\n");    //��ǰӵ�е�����
}

char *rtrim(char *str)  /*remove the trailing blanks.*/
{
    int n=strlen(str)-1;                 //��ȡ�ַ����ĳ���
    while(n>=0)                          //����������
    {
        if(*(str+n)!=' ')                //�ж����һ���Ƿ�Ϊ��
        {
            *(str+n+1)='\0';             //����Ϊ�գ���n+1���ַ�Ϊ'\0'
            break;
        }
        else n--;                        //��Ϊ�գ�����ǰ��
    }
    if (n<0) str[0]='\0';                //��������Ϊ�գ����һ���ַ�����'\0'
    return str;                          //��������ַ���
}

char *ltrim(char *str) /*remove the heading blanks.*/
{
    strrev(str);                         //��ת�ַ���
    rtrim(str);                          //ȥ��β���ո�
    strrev(str);                         //��ת�ַ���
    return str;                          //�����ַ���
}

void LoginF()  /*LOGIN FileSystem*/
{
    char loginame[MAXNAME],loginpw[9],logincpw[9],str[50];     //���þֲ�����
    int i,j,flag=1;
    char a[25];
    int findout; /*login user not exist*/

    while(1)
    {
        findout=0;
        printf("\n\nLogin Name:");                      //��ʾ��Ϣ
        gets(loginame);                                 //�����ַ���
        ltrim(rtrim(loginame));                         //ȥ����β�ո�
        fp_mfd=fopen("c:\\osfile\\mfd.txt","rb");       //���˻������ļ�
        for(i=0;fread(&ufd_lp,sizeof(OSUFD_LOGIN),1,fp_mfd)!=0;i++)   //��ȡ�û���Ϣ��ֱ��
            if (strcmp(strupr(ufd_lp.ufdname),strupr(loginame))==0)   //ƥ���û���
            {
                findout=1;                                            //��ƥ�䵽�û�
                strcpy(logincpw,ufd_lp.ufdpword);                     //�����û����뵽��ʱ����
        fclose(fp_mfd);
        if (findout==1)  /*user exist*/
        {
            printf("Login Password:");
            InputPW(loginpw); /*input password,use '*' replace*/
            if (strcmp(loginpw,logincpw)==0)                        //ƥ���û�����
            {
                strcpy(username,strupr(loginame));                   //������username����
                strcpy(dirname,username);                            //������dirname����
                fp_mfd=fopen("c:\\osfile\\mfd.txt","rb");
                for(j=0;fread(&ufd_lp,sizeof(OSUFD_LOGIN),1,fp_mfd)!=0;j++)   //�����˻��ļ�
                {
                    strcpy(str,"c:\\osfile\\");           //����ַ�����str
                    strcat(str,ufd_lp.ufdname);             //�û���
                    strcat(str,".txt");                     //׷�ӵ�str
                    ufd[j]=(OSUFD*)malloc(sizeof(OSUFD));    //����ռ�
                    strcpy(ufd[j]->ufdname,strupr(ufd_lp.ufdname));  //��¼�û���Ϣ
                    fp_ufd=fopen(str,"rb");     //���û��ļ�Ŀ¼
                    fcount[j]=0;     //��¼�û��ļ�����
                    for(i=0;fread(&ufd[j]->ufdfile[i],sizeof(OSFILE),1,fp_ufd)!=0;i++,fcount[j]++)   //�����û��ļ�Ŀ¼
                    {
                        ifopen[j][i].ifopen=0;                   //�����û��ļ�Ĭ��Ϊ�ر�״̬
                        ifopen[j][i].openmode=4;                 //���÷���ģʽΪ4
                    }
                    fclose(fp_ufd);     //�ر��û��ļ�ָ��
                }

                fclose(fp_mfd);        //�ر��˻��ļ�ָ��
                ucount=j;              //�û��ļ�����

                SetPANo(0);            //��ʼ���û��ļ���������
                printf("\n\nLogin successful! Welcome to this FileSystem\n\n");
                loginsuc=1;            //����ɹ�
                return;
            }
            else
            {
                printf("\n\n");
                flag=1;
                while(flag)   //ѭ���ж�
                {
                    printf("Login Failed!  Password Error.  Try Again(Y/N):");
                    gets(a);     //�����ַ�
                    ltrim(rtrim(a));
                    if (strcmp(strupr(a),"Y")==0)    //������������
                    {
                        loginsuc=0;
                        flag=0;
                    }
                    else if(strcmp(strupr(a),"N")==0)    //���ص���ʧ��
                    {
                        loginsuc=0;
                        flag=0;
                        return;
                    }
                }
            }
        }
        else                          //ע�����û�
        {
            printf("New Password(<=8):");      //��������
            InputPW(loginpw); /*input new password,use '*' replace*/
            printf("\nConfirm Password(<=8):"); /*input new password,use '*' replace*/
            InputPW(logincpw);
            if (strcmp(loginpw,logincpw)==0)     //�Ƚ����������Ƿ�һ��
            {
                strcpy(ufd_lp.ufdname,strupr(loginame));      //��¼�����û���Ϣ
                strcpy(ufd_lp.ufdpword,loginpw);
                fp_mfd=fopen("c:\\osfile\\mfd.txt","ab");      //��׷���ļ���Ϣ���ļ�
                fwrite(&ufd_lp,sizeof(OSUFD_LOGIN),1,fp_mfd);  //д�����û�����
                fclose(fp_mfd);
                strcpy(username,strupr(loginame));        //��¼��Ϣ��username��dirname
                strcpy(dirname,loginame);

                ////////////////////////////////////////////////////////
                strcpy(str,"c:\\osfile\\");
                strcat(str,username);
                strcat(str,".txt");       //�û��ļ�·��
                if((fp_ufd=fopen(str,"rb"))==NULL)     //���û��ļ�
                {
                    fp_ufd=fopen(str,"wb");            //��������ڣ��ʹ����û��ļ�
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
        if((fp_file_p=fopen("c:\\osfile\\file\\file_p.txt","rb"))==NULL)    //�ж��ļ��Ƿ����
        {
            fp_file_p=fopen("c:\\osfile\\file\\file_p.txt","wb");    //�����ڣ������ļ�
            fclose(fp_file_p);
        }
        fp_file_p=fopen("c:\\osfile\\file\\file_p.txt","rb");        //�������ż�¼�ļ�

        ///////////////////////////////////////////////////////////////////
        for(i=0;fread(&j,sizeof(int),1,fp_file_p)!=0;i++)       //�����ڵĿ�ż�¼Ϊ1
            fpaddrno[j]=1;

        /*for(i=1;i<MAX;i++)
         if ((i%13)==0)
         fpaddrno[i]=1;*/
    }
    else
    {
        fp_file_p=fopen("c:\\osfile\\file\\file_p.txt","wb");  //�������ż�¼�ļ�
        /*for(i=1;i<MAX;i++)
         if((i%13)==0)
         fpaddrno[i]=0;*/
        for(i=0;i<MAX;i++)
            if (fpaddrno[i]==1)
                fwrite(&i,sizeof(int),1,fp_file_p);   //�������������ż�¼
    }
    fclose(fp_file_p);
}

void InputPW(char *password)  /*input password,use '*' replace*/
{
    int j;
    for(j=0;j<=7;j++)
    {
        password[j]=getch();         //�����ַ�
        if ((int)(password[j])!=13)    //�Ƿ��ǻس���
        {
            if((int)(password[j])!=8)   //�Ƿ����˸��
                putchar('*');           //������ʾ��*��
            else
            {
                if (j>0)
                {
                    j--;
                    j--;
                    putchar('\b');putchar(' ');putchar('\b');   //ɾ��֮ǰ�ġ�*��
                }
                else j--;
            }
        }
        else
        {	password[j]='\0';      //�ڽ����Ӵ�������Ӵ�������
            break;
        }
    }
    password[j]='\0';     //������8λʱ�Զ�����Ӵ�������
}

void DirF()  /*Dir FileSystem*/
{
    int i,j,count=0;
    char sfmode[25],sfpaddr[25],str[25];     //���������ַ����������

    clrscr();                                //����
    if (strcmp(strupr(ltrim(rtrim(dirname))),"")!=0)    //�Ƚϵ�ǰ�ļ�Ŀ¼
    {
        printf("\n\nC:\\%s>dir\n",dirname);             //��ʾ
        printf("\n%14s%16s%14s%10s%18s\n","FileName","FileAddress","FileLength","Type","FileMode");    //��ʾ�ļ���Ϣ������
        j=ExistD(dirname);                             //�û��洢λ��
        for(i=0;i<fcount[j];i++)                       //�����û�Ŀ¼���ļ�
        {
            if ((i%16==0)&&(i!=0))
            {
                printf("\nPress any key to continue..");
                getch();
                clrscr();
                printf("\n%14s%16s%14s%10s%18s\n","FileName","FileAddress","FileLength","Type","FileMode");
            }
            /////////////////////////////////////////////////////////
            itoa(ufd[j]->ufdfile[i].fpaddr,str,10);     //�û��ļ�������
            strcpy(sfpaddr,"file");
            strcat(sfpaddr,str);
            if (ufd[j]->ufdfile[i].fmode==0) strcpy(sfmode,"Read Only");
            else if(ufd[j]->ufdfile[i].fmode==1) strcpy(sfmode,"Write Only");
            else if(ufd[j]->ufdfile[i].fmode==2)strcpy(sfmode,"Read And Write");
            else strcpy(sfmode,"Protect");
            printf("%14s%16s%14d%10s%18s\n",ufd[j]->ufdfile[i].fname,sfpaddr,ufd[j]->ufdfile[i].flength,"<FILE>",sfmode);   //��ʾ�û��ļ���Ϣ
        }
        printf("\n %3d file(s)\n",fcount[j]);   //�û��ļ�����
    }
    else
    {
        printf("\n\nC:\\>dir\n");           //��ʾ��Ŀ¼
        printf("\n%14s%18s%8s\n","DirName","OwnFileCount","Type");    //��ʾ�û��ļ���Ϣ
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
        printf("\n %3d dir(s),%5d file(s)\n",ucount,count);   //��ʾ�û�����
    }
}

int ExistD(char *dirname)  /*Whether DirName Exist,Exist-i,Not Exist-0*/
{
    int i;
    int exist=0;
    for(i=0;i<ucount;i++)      //�ҳ��û���Ϣ���洢��λ��
        if (strcmp(strupr(ufd[i]->ufdname),strupr(dirname))==0)
        {
            exist=1;
            break;
        }
    if (exist) return(i);    //���ش洢λ��
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


    if (strcmp(strupr(dirname),strupr(username))!=0)           //�ж��û��ǲ����ڵ�ǰĿ¼��
    {
        printf("\nError. You must create file in your own dir.\n");
        wgetchar=1;
    }
    else
    {
        printf("\nPlease input FileName:");
        gets(fname);                              //�����ļ���
        ltrim(rtrim(fname));
        if (ExistF(fname)>=0)                     //�ļ��Ƿ����
        {
            printf("\nError. Name \'%s\' has already existed.\n",fname);
            wgetchar=1;
        }
        else      //�ļ�������
        {
            printf("Please input FileMode(0-Read Only, 1-Write Only, 2-Read and Write, 3-Protect):");
            gets(fmode);
            ltrim(rtrim(fmode));          //�����ļ�����Ȩ��
            if((strcmp(fmode,"0")==0)||(strcmp(fmode,"1")==0)||(strcmp(fmode,"2")==0)||(strcmp(fmode,"3")==0))
            {
                fpaddrno=FindPANo();    //�ҵ��ɴ洢��������
                if (fpaddrno>=0)
                {
                    i=ExistD(username);      //�û�λ��
                    strcpy(ufd[i]->ufdfile[fcount[i]].fname,fname);        //�û��ļ���Ϣ�洢
                    ufd[i]->ufdfile[fcount[i]].fpaddr=fpaddrno;
                    ufd[i]->ufdfile[fcount[i]].fmode=atoi(fmode);
                    ifopen[i][fcount[i]].ifopen=0;
                    ifopen[i][fcount[i]].openmode=4;
                    strcpy(str,"c:\\osfile\\file\\file");    //�����û��ļ�
                    itoa(fpaddrno,str1,10);
                    strcat(str,str1);
                    strcat(str,".txt");
                    fp_file=fopen(str,"wb");
                    fclose(fp_file);
                    fcount[i]++;    //�û��ļ���������
                    while(flag)       //ѭ���ж�
                    {
                        printf("Input text now(Y/N):");
                        gets(a);
                        ltrim(rtrim(a));
                        ufd[i]->ufdfile[fcount[i]-1].flength=0;   //��ǰ�����ļ�����Ϊ0
                        if(strcmp(strupr(a),"Y")==0)       //д���ļ�����
                        {
                            fp_file=fopen(str,"wb+");
                            ufd[i]->ufdfile[fcount[i]-1].flength=WriteF1();    //д�����ݣ�����¼�ļ�����
                            flag=0;    //����ѭ��
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
                    printf("\nFail!No Disk Space. Please format your disk.\n");   //û������ռ�
                    wgetchar=1;
                }
            }
            else
            {
                printf("\nError. FileMode\'s Range is 0-3\n");    //�ļ�����ģʽ
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
    for(i=0;i<fcount[j];i++)                //�����ļ��Ƿ���ڣ�������λ��
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
    for(i=0;i<MAX;i++)             //���������ҵ���һ����Ϊ���������
        if (fpaddrno[i]==0)
        {
            fpaddrno[i]=1;
            break;
        }
    if (i<MAX) return(i);     //���ؿ���������
    else return(-1);
}

int WriteF1()  /*write file*/
{
    int length=0;
    char c;
    printf("Please input text(\'#\' stands for end):\n");
    while((c=getchar())!='#')                //������#��ֹͣ����
    {
        fprintf(fp_file,"%c",c);     //д���ļ�
        if (c!='\n') length++;       //ͳ�Ƴ���
    }
    fprintf(fp_file,"\n");           //д��س���
    fclose(fp_file);
    return(length);
}

void DeleteF() /*Delete File*/
{
    printf("\n\nC:\\%s>",strupr(dirname));    //��ʾ·��
    char fname[MAXNAME], str[50], str1[50];    //���������ַ�������
    int i, k, j;
    int fpaddrno1;        //��¼�ļ������ַ���
    if(strcmp(strupr(ltrim(rtrim(dirname))), "")==0){      //�ж��û��Ƿ����û�Ŀ¼��
        printf("\nError.Please convert to ufd dir before delete. \n");
        wgetchar=1;
    }
    if(strcmp(strupr(dirname), strupr(username))!=0){
        printf("\nError.You can only modify filemode in yourself dir.\n");
        wgetchar=1;
    } else {
        printf("\nPlease input FileName:");
        gets(fname);                //����ɾ�����ļ���
        ltrim(rtrim(fname));
        i=ExistF(fname);     //�û��ļ�λ��
        if(i>=0){
            k=ExistD(username);    //��ȡ�û����ڴ洢λ��
            if(ifopen[k][i].ifopen==1){           //�ļ�״̬���ڴ�״̬������ɾ��
                printf("\n Error.\'%s\' is in open status. Close it before delete.\n", fname);
                wgetchar=1;
            } else {
                if(ufd[k]->ufdfile[i].fmode==3){     //�����ļ�������ɾ��
                    printf("\nError.\'%s\'is in protect status. Close it before delete.\n", fname);
                    wgetchar=1;
                } else {
                    fpaddrno1=ufd[k]->ufdfile[i].fpaddr;      //��ȡ�ļ��������ַ���
                    fpaddrno[fpaddrno1]=0;    //���������ַ���
                    for(j=i; j<fcount[k]; j++)             //���ļ�����ǰ�ƶ�
                    {
                        ufd[k]->ufdfile[j]=ufd[k]->ufdfile[j+1];
                    }
                    strcpy(str , "c:\\osfile\\file\\");
                    itoa(fpaddrno1, str1, 10);     //������ת��Ϊ�ַ���
                    strcat(str, str1);
                    strcat(str, ".txt");
                    remove(str);                  //ɾ�������ļ�
                    fcount[k--];                   //�ļ�������һ
                    printf("\n\'%s\'is deleted successfully.\n", fname);
                    wgetchar=1;
                }
            }
        } else {
            printf("\nError.\'%s\' dose not exist.\n", fname);     //�ļ�������
            wgetchar=1;
        }
    }
}

void ModifyFM() /*Modify FileMode*/
{
    char fname[MAXNAME],str[50];    //���������ַ���
    int i,k;
    char fmode[25]; /*whether delete*/

    if (strcmp(strupr(dirname),strupr(username))!=0)      //�޸��û��ļ���Ҫ���û�Ŀ¼��
    {
        printf("\nError.You can only modify filemode in yourself dir.\n");
        wgetchar=1;
    }
    else
    {
        printf("\nPlease input FileName:");
        gets(fname);          //��ȡ��Ҫ�޸ĵ��ļ���
        ltrim(rtrim(fname));    //ȥ����β�Ŀո�
        i=ExistF(fname);       //��ȡ�ļ��������ַ
        if (i>=0)
        {
            k=ExistD(username);    //��ȡ�û����ļ���ַ
            if(ifopen[k][i].ifopen==1)    //�ļ��ѱ��򿪣��������޸�
            {
                printf("\nError.\'%s\' is in open status. Close it before modify.\n",fname);
                wgetchar=1;
            }
            else
            {
                if(ufd[k]->ufdfile[i].fmode==0) strcpy(str,"read only");    /*FileMode*/    //��ʾ��ǰ�ļ���Ϣ
                else if(ufd[k]->ufdfile[i].fmode==1) strcpy(str,"write only");
                else if(ufd[k]->ufdfile[i].fmode==2) strcpy(str,"read and write");
                else strcpy(str,"Protect");

                printf("\'%s\' filemode is %s.\n",fname,strupr(str));
                printf("Modify to(0-read only,1-write only,2-read and write,3-Protect):");   //��ʾ��Ϣ
                gets(fmode);       //���շ���Ȩ������
                ltrim(rtrim(fmode));
                if(strcmp(fmode,"0")==0)    //ֻ��
                {
                    ufd[k]->ufdfile[i].fmode=0;
                    printf("\n\'%s\' has been modified to READ ONLY mode successfully.\n",fname);
                    wgetchar=1;
                }
                else if(strcmp(fmode,"1")==0)    //ֻд
                {
                    ufd[k]->ufdfile[i].fmode=1;
                    printf("\n\'%s\' has been modified to WRITE ONLY mode successfully.\n",fname);
                    wgetchar=1;
                }
                else if(strcmp(fmode,"2")==0)    //��д
                {
                    ufd[k]->ufdfile[i].fmode=2;
                    printf("\n\'%s\' has been modified to READ AND WRITE mode successfully.\n",fname);
                    wgetchar=1;
                }
                else if(strcmp(fmode,"3")==0)    //�ܱ���
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
            printf("\nError. \'%s\' dose not exist.\n",fname);   //����Ҫ�޸��ļ�������
            wgetchar=1;
        }
    }
}


void OpenF() /*Open File*/
{
    printf("\n\nC:\\%s>",strupr(dirname));    //��ʾ��ǰ·��
    int fcoun, i;                     //�����������α���
    char fname[MAXNAME], fmode[25];  //���������ַ�������
    int fmod;                         //�ļ�ģʽ
    printf("\nPlease input FileName:");
    gets(fname);         //���մ��ļ����ļ���
    ltrim(rtrim(fname));
    if(ExistF(fname)<0)     //�ļ��Ƿ����
    {
        printf("\nError.FileName \'%s\' is not existed.\n", fname);
        wgetchar=1;
    } else {
        i=ExistD(username);    //��ȡ�û�������Ϣ
        for(int a=0; a < fcount[i]; a++)       //�����û��ļ�
        {
            if(strcmp(fname, ufd[i]->ufdfile[a].fname)==0)   //�ҵ��ļ�
            {
                fcoun=a;
                break;
            }
        }
        ifopen[i][fcoun].ifopen=1;    //���ļ�״̬��Ϊ��״̬
        printf("Please input OpenMode(0-Read Only, 1-Write Only, 2-Read and Write, 3-Protect:");   //���ļ�ģʽ
        gets(fmode);
        fmod=atoi(fmode);
        ifopen[i][fcoun].openmode=fmod;   //���ļ���ģʽ��ΪOpenMode
        printf("\nOpen Successed");
        wgetchar=1;
    }
}

void CloseF() /*Close File*/
{
    printf("\n\nC:\\%s>",strupr(dirname));   //��ʾ·��
    char fname[MAXNAME];                     //�����ַ�������
    int i, k, n=0;

    if(strcmp(strupr(dirname), strupr(username))!=0)    //�ر��û��ļ������û�Ŀ¼��
    {
        printf("\nError,You can only modify filemode in yourself dir.\n");
    } else {
		printf("\n\nOpened File(s) List:\n");    //�����Ѵ��ڴ򿪵��ļ�
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
        gets(fname);      //���չر��ļ����ļ���
        ltrim(rtrim(fname));   //��ȥ��β�ո�
        i=ExistF(fname);   //��ȡ�ļ������ַ
        if(i >= 0)
        {
            k=ExistD(username);     //��ȡ�û��ļ���Ϣ
            if(ifopen[k][i].ifopen==0)    //�ļ����ڹر�״̬
            {
                printf("\nError.\'%s\'has been closed you can not close it again.\n", fname);
            } else {    //���ļ��ر�
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
    int i,k,n=0;        //�����������α���
    char fname[MAXNAME];      //�����ַ�������
    char str[255],str1[255],c;     //�����ַ������ַ�����
    if (strcmp(strupr(ltrim(rtrim(dirname))),"")==0)    //��Ҫ���û�Ŀ¼��
    {
        printf("\nError.Please convert to ufd dir before read.\n");
        wgetchar=1;
        return;
    }
    printf("\nCaution:Open file first\n");
    printf("Opened File(s) List:\n");
    k=ExistD(dirname);      //��ȡ�û��ļ���Ϣ
    for(i=0;i<fcount[k];i++)              //�����û��ļ�����ʾ�Ѿ��򿪵��ļ�
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
    if(n!=0)    //�д򿪵��ļ�
    {
        printf("\nPlease input FileName:");
        gets(fname);         //��ȡ�ļ���
        ltrim(rtrim(fname));       //��ȥ��β�ո�
        i=ExistF(fname);        //��ȡ�ļ������ַ
        if(i>=0)    //�ļ�����
        {
            if(ifopen[k][i].ifopen==1)            //���ڴ�״̬
            {
                if((ifopen[k][i].openmode==0) ||(ifopen[k][i].openmode==2))   //����ֻд�Ͷ�дȨ��
                {
                    itoa(ufd[k]->ufdfile[i].fpaddr,str,10);     //��ȡ�ļ��������ļ�
                    strcpy(str1,"file");
                    strcat(str1,str);
                    strcpy(str,"c:\\osfile\\file\\");
                    strcat(str,str1);
                    strcat(str,".txt");
                    fp_file=fopen(str,"rb");
                    fseek(fp_file,0,0);                  //�ļ�ָ��ָ��ͷ��
                    printf("\nThe text is:\n\n");
                    printf("   ");
                    while(fscanf(fp_file,"%c",&c)!=EOF)     //���ַ����ֱ���ļ�����
                        if (c=='\n') printf("\n   ");
                        else printf("%c",c);
                    printf("\n\n%d Length.\n",ufd[k]->ufdfile[i].flength);    //����ļ�����
                    fclose(fp_file);     //�ر��ļ�ָ��
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
    printf("\n\nC:\\%s>",strupr(dirname));   //��ʾ�û�·��
    int i, k, m=0;    //�������α���
    int length;        //���峤�����α���
    char fname[MAXNAME];   //�����ļ����ַ���
    char str[255], str1[255];   //���������ַ�������
    if(strcmp(strupr(dirname), strupr(username))!=0) {     //�û��Ƿ����û�Ŀ¼��
        printf("\nError.Please convert to ufd dir before write.\n");
        wgetchar=1;
        return;
    }
    printf("\nCaution:Open file first\n");
    printf("Opened File(s) List:\n");
    k=ExistD(dirname);   //��ȡ�û��ļ���Ϣ
    for(i=0; i<fcount[k]; i++)    //�����û��µ��ļ�
    {
        if(ifopen[k][i].ifopen==1) {       //�ļ����ڴ�״̬
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
        gets(fname);     //�����ļ���
        ltrim(rtrim(fname));
        i=ExistF(fname);    //��ȡ�ļ������ַ
        if(i>=0) {    //�ļ�����
            if(ifopen[k][i].ifopen==1) {     //�ļ����ڴ�״̬
                if(ifopen[k][i].openmode==1 || ifopen[k][i].openmode==2) {   //�ļ�Ȩ����ֻд���д
                    itoa(ufd[k]->ufdfile[i].fpaddr, str, 10);    //��ȡ�ļ�·��
                    strcpy(str1, "file");
                    strcat(str1, str);
                    strcpy(str, "c:\\osfile\\file\\");
                    strcat(str, str1);
                    strcat(str, ".txt");    //�ļ�·��
					char str2[3];
					int choice=3;
					strcpy(str2,"ab");
					printf("You can choise [0-Covered W] [1-Additonal W]:");     //׷�ӻ��Ǹ���
					scanf(" %d", &choice);
					if(choice == 0)
						strcpy(str2, "wb");
                    fp_file=fopen(str, str2);    //���ļ�
                    length=WriteF1();
                    ufd[k]->ufdfile[i].flength=ufd[k]->ufdfile[i].flength+length;   //�޸��ļ�����
					if(choice == 0)
						ufd[k]->ufdfile[i].flength=length;
                    printf("\n\nYou have write file successfully!!!");
					fclose(fp_file);
                    wgetchar=0;
                } else if(ifopen[k][i].openmode==0) {    //�ļ����ڹر�״̬��������д
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

    SetPANo(1);     //���������ַ���
    if (fp_mfd!=NULL) fclose(fp_mfd);    //�ر��˻���Ϣָ��
    if (fp_ufd!=NULL) fclose(fp_ufd);     //�ر��û���Ϣָ��
    if (fp_file!=NULL) fclose(fp_file);    //�ر��û��ļ�ָ��
    for(j=0;j<ucount;j++)        //�����û�����
    {
        strcpy(str,"c:\\osfile\\");
        strcat(str,ufd[j]->ufdname);
        ltrim(rtrim(str));
        strcat(str,".txt");
        fp_ufd=fopen(str,"wb");     //��д�û���Ϣ
        fclose(fp_ufd);
        fp_ufd=fopen(str,"ab");   //׷���û���Ϣ
        for(i=0;i<fcount[j];i++)
            fwrite(&ufd[j]->ufdfile[i],sizeof(OSFILE),1,fp_ufd);
        fclose(fp_ufd);
    }
}
