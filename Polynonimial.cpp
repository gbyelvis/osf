//
//  main.cpp
//  Polynomial
//
//  Created by james gavin on 16/1/10.
//  Copyright © 2016年 james gavin. All rights reserved.
//

#include <iostream>
using namespace std;

typedef struct Poly{
    float coefficient;
    int index;
    struct Poly *next;
}Poly;

Poly* inputPoly() {
    int m;
    float n;
    Poly *head, *rear, *s;
    head = (Poly*)malloc(sizeof(Poly));
    rear = head;
    scanf(" %f", &n);
    scanf(" %d", &m);
    while(m != 0) {
        cout<<n<<","<<m<<endl;
        s = (Poly*)malloc(sizeof(Poly));
        s->coefficient = n;
        s->index = m;
        s->next = nullptr;
        rear->next = s;
        rear = s;
        scanf(" %f", &n);
        scanf(" %d", &m);
    }
    head = head->next;
    return head;
}

void sortPoly(Poly* head) {
    Poly *p, *q, *t;
    float temp;
    
    p = head;
    while (p != nullptr) {
        q = p;
        t = q->next;
        cout<<"gagagajg";
        while (t != nullptr) {
         //   cout<<"gagagajg";
            if (t->index > q->index) {
                q = t;
                t = t->next;
            }
            temp = p->index; p->index = q->index; q->index = temp;
            temp = p->coefficient ; p->coefficient = q->coefficient; q->coefficient = temp;
            p = p->next;
        }
       // p = p->next;
    }
}

void printPoly(Poly* head) {
    Poly* p;
    int firstFlag = 1;
    
    p = head;
    
    while (p != nullptr) {
        if (firstFlag == 1) {
            if (p->index == 0) {
                printf("%5.2f", p->coefficient);
            } else if (p->coefficient == 1 || p->coefficient == -1) {
                printf("X^%d", p->index);
            } else if (p->coefficient > 0) {
                printf("%5.2fX^%d", p->coefficient, p->index);
            } else if (p->coefficient < 0) {
                printf("%5.2fX^%d", p->coefficient, p->index);
            }
            firstFlag = 0;
        } else {
            if (p->index == 0) {
                if (p->coefficient > 0) {
                    printf("+%5.2f", p->coefficient);
                } else {
                    printf("%5.2f", p->coefficient);
                }
            } else if (p->coefficient == 1) {
                printf("+X^%d", p->index);
            } else if (p->coefficient == -1) {
                printf("X^%d", p->index);
            } else if (p->coefficient > 0) {
                printf("+%5.2fX^%d", p->coefficient, p->index);
            } else if (p->coefficient < 0) {
                printf("%5.2fX^%d", p->coefficient, p->index);
            }
        }
        p = p->next;
    }
    printf("\n");
}

Poly* addPoly(Poly* headone, Poly* headtwo) {
    Poly *heads, *p, *q, *s, *r;
    
    float csum;
    p = headone;
    q = headtwo;
    
    heads = (Poly*)malloc(sizeof(Poly));
    r = heads;
    
    while (p != nullptr && q != nullptr) {
        if(p->index == q->index) {
            csum = p->coefficient + q->coefficient;
            if (csum != 0) {
                s = (Poly*)malloc(sizeof(Poly));
                s->coefficient = csum;
                s->index = q->index;
                r->next = s;
                r = s;
            }
            q = q->next;
            p = p->next;
        } else if(p->index < q->index) {
            s = (Poly*)malloc(sizeof(Poly));
            s->coefficient = q->coefficient;
            s->index = q->index;
            r->next = s;
            r = s;
            q = q->next;
        } else {
            s = (Poly*)malloc(sizeof(Poly));
            s->coefficient = p->coefficient;
            s->index = p->index;
            r->next = s;
            r = s;
            p = p->next;
        }
    }
    
    while (p != nullptr) {
        s = (Poly*)malloc(sizeof(Poly));
        s->coefficient = p->coefficient;
        s->index = p->index;
        r->next = s;
        r = s;
        p = p->next;
    }
    
    while (q != nullptr) {
        s = (Poly*)malloc(sizeof(Poly));
        s->coefficient = q->coefficient;
        s->index = q->index;
        r->next = s;
        r = s;
        q = q->next;
    }
    r->next = nullptr;
    heads = heads->next;
    return heads;
}

Poly* subPoly(Poly* headone, Poly* headtwo) {
    Poly *heads, *p, *q, *s, *r;
    
    float csum;
    p = headone;
    q = headtwo;
    
    heads = (Poly*)malloc(sizeof(Poly));
    r = heads;
    
    while (p != nullptr && q != nullptr) {
        if(p->index == q->index) {
            csum = p->coefficient - q->coefficient;
            if (csum != 0) {
                s = (Poly*)malloc(sizeof(Poly));
                s->coefficient = csum;
                s->index = q->index;
                r->next = s;
                r = s;
            }
            q = q->next;
            p = p->next;
        } else if(p->index < q->index) {
            s = (Poly*)malloc(sizeof(Poly));
            s->coefficient = q->coefficient;
            s->index = q->index;
            r->next = s;
            r = s;
            q = q->next;
        } else {
            s = (Poly*)malloc(sizeof(Poly));
            s->coefficient = p->coefficient;
            s->index = p->index;
            r->next = s;
            r = s;
            p = p->next;
        }
    }
    
    while (p != nullptr) {
        s = (Poly*)malloc(sizeof(Poly));
        s->coefficient = p->coefficient;
        s->index = p->index;
        r->next = s;
        r = s;
        p = p->next;
    }
    
    while (q != nullptr) {
        s = (Poly*)malloc(sizeof(Poly));
        s->coefficient = q->coefficient;
        s->index = q->index;
        r->next = s;
        r = s;
        q = q->next;
    }
    r->next = nullptr;
    heads = heads->next;
    return heads;
}

void add_main() {
    Poly *a, *b, *c;
    
    cout<<"\n输入第一个一元多项式：\n系数  指数\n";
    a = inputPoly();
    sortPoly(a);
    
    cout<<"\n输入第二个一元多项式：\n系数  指数\n";
    b = inputPoly();
    sortPoly(b);
    
    c = addPoly(a, b);
    
    cout<<"第一个一元多项式：";
    printPoly(a);
    cout<<"第二个一元多项式：";
    printPoly(b);
    cout<<"两个式子相加如下："<<endl;
    printPoly(c);
}

void sub_main() {
    Poly *a, *b, *c;
    
    cout<<"\n输入第一个一元多项式：\n系数  指数\n";
    a = inputPoly();
    sortPoly(a);
    
    cout<<"\n输入第二个一元多项式：\n系数  指数\n";
    b = inputPoly();
    sortPoly(b);
    
    c = subPoly(a, b);
    
    cout<<"第一个一元多项式：";
    printPoly(a);
    cout<<"第二个一元多项式：";
    printPoly(b);
    cout<<"两个式子相减如下："<<endl;
    printPoly(c);
}

void open() {
    system("cls");
    cout<<"\n********************************************************\n";
    cout<<"功能：* 1：两个一元多项式相加；* 2：两个一元多项式相减；* 0：退出*";
    cout<<"\n********************************************************\n\n请选择操作:";
}


int main(int argc, const char * argv[]) {
    int choose = -1;
    open();
    while (choose != 0) {
        scanf(" %d", &choose);
        getchar();
        switch (choose) {
            case 0:
                return 0;
            case 1:
                cout<<"两个一元多项式相加\n";
                add_main();
                choose = -1;
                open();
                break;
            case 2:
                cout<<"两个一元多项式相减\n";
                sub_main();
                choose = -1;
                open();
                break;
            default:
                cout<<"尚未扩展，敬请期待……\n";
                open();
                break;
        }
    }
    return 0;
}
