#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<conio.h>
#include<windows.h>
HANDLE out;
COORD point={0,0};
int changed=1;
int block[4][4]={};
int lose=0,score=0,ran=1;
inline int color(int n){
    int p=0;
    while(!(n&1)){
        n>>=1;
        ++p;
    }
    SetConsoleTextAttribute(out,(p+6)%15+1);
    return 0;
}
int randomBlock(){
    int sed,sum=0;
    srand(ran*time(0));
    ran=rand();
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(!block[i][j])sum++;
    if(sum==1)
        lose=1;
    if(sum==0){
        lose=1;
        return 0;
    }
    sed=(ran%(2*sum))/2;
    for(int i=0;i<4;i++) 
        for(int j=0;j<4;j++)
            if(!block[i][j])
                if(sed>0)
                    --sed;
                else{
                    score+=(block[i][j]=(ran%2+1)*2);
                    return 0;
                }
    return 0;
}
int merge(int*a,int*b,int*c,int*d){
    int*l[]={a,b,c,d},p=0,m[]={0,0,0,0};
    for(int i=0;i<4;i++)if(*(l[i])){
        if(!m[p])
            m[p]=*(l[i]);
        else if(m[p]==*(l[i]))
            m[p++]*=2;
        else
            m[++p]=*(l[i]);
    }
    if(!changed)changed=(*a!=m[0]||*b!=m[1]||*c!=m[2]||*d!=m[3]);
    *a=m[0];*b=m[1];*c=m[2];*d=m[3];
    return 0;
}
int show(){
    char tmp[10]={};
    int m=0,t;
    SetConsoleCursorPosition(out,point);
    //system("cls");
    printf("□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□\n");
    for(int i=0;i<4;i++){
        for(int j=0;j<7;j++){
            printf("□");
            for(int k=0;k<4;k++)
                if(!block[i][k])
                    printf("              ");
                else{
                    color(block[i][k]);
                    if(j==0||j==6)
                        printf("■■■■■■■");
                    else if(j!=3)
                        printf("■          ■");
                    else{
                        m=0;
                        sprintf(tmp,"%d",block[i][k]);
                        printf("■");
                        for(;m<(10-strlen(tmp))/2;m++)
                            printf(" ");
                        t=m;
                        while(tmp[m-t])printf("%c",tmp[(m++)-t]);
                        while(m++<10)printf(" ");
                        printf("■");
                    }
                }
            SetConsoleTextAttribute(out,7);
            printf("□");
            if(i==1&&j==2)printf("    Use \"W,A,S,D\" to move");
            else if(i==1&&j==4)printf("    Press Space to stop");
            printf("\n");
        }
    }
    printf("□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□\n\n     Score: %d",score);
    return 0;
}
int mov(char c){
    switch(c){
        case'a':
            for(int i=0;i<4;i++)merge(&block[i][0],&block[i][1],&block[i][2],&block[i][3]);
            break;
        case'w':
            for(int i=0;i<4;i++)merge(&block[0][i],&block[1][i],&block[2][i],&block[3][i]);
            break;
        case'd':
            for(int i=0;i<4;i++)merge(&block[i][3],&block[i][2],&block[i][1],&block[i][0]);
            break;
        case's':
            for(int i=0;i<4;i++)merge(&block[3][i],&block[2][i],&block[1][i],&block[0][i]);
            break;
    }
    return 0;
}
inline int haveLost(){
    for(int i=0;i<4;i++)for(int j=0;(j<4&&i!=3)||j<3;j++)if((i<3&&block[i][j]==block[i+1][j])||(j<3&&block[i][j]==block[i][j+1]))return 0;
    return 1;
}
int main(){
    system("cls");
    out=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTitle("  [2048]");
    SetConsoleTextAttribute(out,7);
    char c;
    START:
    randomBlock();
    while(1){
        if(changed){
            randomBlock();
            changed=0;
        }
        re:
        show();
        if(lose==1)
            if((lose=haveLost())==1)
                break;
        while(!kbhit())Sleep(25);
        c=getch();
        if(c=='w'||c=='a'||c=='s'||c=='d')
            mov(c);
        else if(c==' '){
            system("cls");
            printf("\n\n\n\n\n          PAUSE,press any bottom to continue.");
            while(!kbhit())Sleep(25);
            c=getch();c=1;
            Sleep(25);
            goto re;
        }else
            goto re;
    }
    Sleep(1000);
    point.X=16;point.Y=12;
    SetConsoleCursorPosition(out,point);
    printf("          GameOver.         ");
    ++point.Y;
    SetConsoleCursorPosition(out,point);
    printf("                            ");
    ++point.Y;
    SetConsoleCursorPosition(out,point);
    printf("   Your Score Is:%7d   ",score);
    ++point.Y;
    SetConsoleCursorPosition(out,point);
    printf("                            ");
    ++point.Y;
    SetConsoleCursorPosition(out,point);
    printf("    Press '=' to restart.   ");
    ++point.Y;
    SetConsoleCursorPosition(out,point);
    printf("                            ");
    while(!kbhit()||getch()!='=')Sleep(25);
    memset(block,0,sizeof(block));
    system("cls");
    point.X=point.Y=score=0;changed=1;
    goto START;
}
