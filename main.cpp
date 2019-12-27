#include <iostream>
#include <graphics.h>
#include <cstring>
#include <windows.h>
#include <mmsystem.h>

using namespace std;

// for colors
int itsYellow = COLOR(255, 214, 98);
int itsBg = COLOR(39, 43, 59);
char player1[20]="Red", player2[20]="Blue";
int player1Color=COLOR(221, 65, 50), player2Color=COLOR(51, 51, 255), player1ClickColor=COLOR(154, 36, 25), player2ClickColor=COLOR(0, 0, 204);
short active1Color=1, active2Color=1;

// for matrix
short i1, i2, j1, j2, i3, j3;
char M[6][6];
short dl[]= {-1, 0, 1, 0};
short dc[]= {0, 1, 0, -1};
// the jump
short lin[]= {-2, -1, 1, 2, 2, 1, -1, -2};
short col[]= {1, 2, 2, 1, -1, -2, -2, -1};
// the third coordinate
short pi3[]= {-2, 0, -1, 0, 0, 1, 0, 2, 2, 0, 1, 0, 0, -1, 0, -2};
short pj3[]= {0, 1, 0, 2, 2, 0, 1, 0, 0, -1, 0, -2, -2, 0, -1, 0};

// for menu
bool isMenu=1;
bool isChoosing=0;
bool isGame=0;
bool isOptions=0;
bool isInfo=0;
void showMenu();
void showGameChoosing();
void showOptions();
void showInfo();
void initMenu();
void initGameChoosing();
void initGame();
void initOptions();
void initInfo();

// default language (1 for english or 2 for romanian)
short lang=1;

// music is on/off by default
bool musicOn=1;

// default song name and number
char songName[50] = "Miyagi - Badabum";
short songNr=1;

// get screen width and height
DWORD screenWidth = GetSystemMetrics(SM_CXSCREEN);
DWORD screenHeight = GetSystemMetrics(SM_CYSCREEN);

struct piesaneutra
{
    short i,j;
} stea, diez;

struct L
{
    short i1, j1, i2, j2, i3, j3;
} L1, L2;

void itsSquare(short a, short b, short c, short d, short e, int color)
{
    setfillstyle(SOLID_FILL, color);
    bar(b+c, a+d, b+c+e, a+d+e);
}

void itsRectangle(short a, short b, short c, short d, short e, int color)
{
    setcolor(color);
    setlinestyle(SOLID_LINE, 0, 2);
    rectangle(b+c, a+d, b+c+e, a+d+e);
}

void drawSquare(short a, short b, short c, short d, short e, int color)
{
    setfillstyle(SOLID_FILL, color);
    setcolor(BLACK);
    bar(b+c, a+d, b+c+e, a+d+e);
    setlinestyle(SOLID_LINE, 0, THICK_WIDTH);
    rectangle(b+c, a+d, b+c+e, a+d+e);
}

void drawButton(short x, short y, int fs, int color, int background, char* content)
{
    setcolor(color);
    setbkcolor(background);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, fs);
    outtextxy(x, y, content);
}

void goBack()
{
    if (lang == 1)
    {
        char backEn[] = "Go Back";
        drawButton(getmaxx()-150, getmaxy()-80, 3, WHITE, itsBg, backEn);
    }
    else if (lang == 2)
    {
        char backRo[] = "Inapoi";
        drawButton(getmaxx()-150, getmaxy()-80, 3, WHITE, itsBg, backRo);
    }
}

bool checkClick(short x, short y, short x1, short x2, short y1, short y2)
{
    return x>=x1 && x<=x2 && y>=y1 && y<=y2;
}

void startConfig()
{
    short i, j;
    for(i=1; i<=4; i++)
        for(j=1; j<=4; j++)
            M[i][j]='0';
    M[1][1]='*';
    M[4][4]='#';
    M[1][2]=M[1][3]=M[2][3]=M[3][3]='1';
    M[2][2]=M[3][2]=M[4][2]=M[4][3]='2';
    stea.i=stea.j=1;
    diez.i=diez.j=4;
    L1.i1=1;
    L1.j1=2;
    L1.i2=L1.j2=3;
    L2.i1=L2.j1=2;
    L2.i2=4;
    L2.j2=3;
}

bool ok(short player, short i1, short j1, short i2, short j2, short i3, short j3)
{
    if(i1<1 || i1>4 || j1<1 || j1>4 || i2<1 || i2>4 || j2<1 || j2>4 || i3<1 || i3>4 || j3<1 || j3>4)
        return false;
    short i, j, vecini, k, st=0, diz=0, unu=0, doi=0, k1, k2;
    char N[6][6];
    for(i=1; i<=4; i++)
        for(j=1; j<=4; j++)
            N[i][j]=M[i][j];
    N[i1][j1]=N[i2][j2]=N[i3][j3]='$';
    for(i=1; i<=4; i++)
    {
        for(j=1; j<=4; j++)
        {
            vecini=0;
            k1=k2=-1;
            for(k=0; k<4; k++)
            {
                if(N[i+dl[k]][j+dc[k]]=='$')
                {
                    if(k1==-1)
                        k1=k;
                    else
                        k2=k;
                    vecini++;
                }
                if(vecini==2)
                {
                    if(k2-k1!=2)
                        return false;
                    if(N[i-1][j-1]!='$' && N[i-1][j+1]!='$' && N[i+1][j-1]!='$' && N[i+1][j+1]!='$')
                        return false;
                    N[i][j]='$';
                    i=j=10;
                    break;
                }
            }
        }
    }
    if(i!=11)
        return false;
    for(i=1; i<=4; i++)
    {
        for(j=1; j<=4; j++)
        {
            if(N[i][j]=='*')
                st=1;
            if(N[i][j]=='#')
                diz=1;
            if(N[i][j]=='1')
                unu++;
            if(N[i][j]=='2')
                doi++;
        }
    }
    if(player==1)
    {
        if(doi!=4 || st==0 || diz==0 || unu==0)
            return false;
    }
    else
    {
        if(unu!=4 || st==0 || diz==0 || doi==0)
            return false;
    }
    return true;
}

int mutariposibile(short player)
{
    short i, j, nr=0, k, pi, pj;
    char C[6][6];
    if(player==1)
    {
        for(i=1; i<=4; i++)
        {
            for(j=1; j<=4; j++)
            {
                if(M[i][j]!='1')
                    C[i][j]=M[i][j];
                else
                    C[i][j]='0';
            }
        }
        for(i=1; i<=4; i++)
        {
            for(j=1; j<=4; j++)
            {
                if(C[i][j]=='0')
                {
                    for(k=0; k<8; k++)
                    {
                        pi=i+lin[k];
                        pj=j+col[k];
                        if(C[pi][pj]=='0')
                        {
                            if(ok(1, i, j, pi, pj, i+pi3[2*k], j+pj3[2*k]))
                                nr++;
                            if(ok(1, i, j, pi, pj, i+pi3[2*k+1], j+pj3[2*k+1]))
                                nr++;
                            C[i][j]='%';
                        }
                    }
                }
            }
        }
    }
    else
    {
        for(i=1; i<=4; i++)
        {
            for(j=1; j<=4; j++)
            {
                if(M[i][j]!='2')
                    C[i][j]=M[i][j];
                else
                    C[i][j]='0';
            }
        }
        for(i=1; i<=4; i++)
        {
            for(j=1; j<=4; j++)
            {
                if(C[i][j]=='0')
                {
                    for(k=0; k<8; k++)
                    {
                        pi=i+lin[k];
                        pj=j+col[k];
                        if(C[pi][pj]=='0')
                        {
                            if(ok(2, i, j, pi, pj, i+pi3[2*k], j+pj3[2*k]))
                                nr++;
                            if(ok(2, i, j, pi, pj, i+pi3[2*k+1], j+pj3[2*k+1]))
                                nr++;
                            C[i][j]='%';
                        }
                    }
                }
            }
        }
    }
    return nr;
}

void mutare(short player, short i1, short j1, short i2, short j2, short i3, short j3)
{
    short i, j, k, vecini=0;
    if(player==1)
    {
        for(i=1; i<=4; i++)
            for(j=1; j<=4; j++)
                if(M[i][j]=='1')
                    M[i][j]='0';
        M[i1][j1]=M[i2][j2]=M[i3][j3]='1';
        for(i=1; i<=4; i++)
        {
            for(j=1; j<=4; j++)
            {
                if(M[i][j]=='0')
                {
                    vecini=0;
                    for(k=0; k<4; k++)
                    {
                        if(M[i+dl[k]][j+dc[k]]=='1')
                            vecini++;
                        if(vecini==2)
                        {
                            M[i][j]='1';
                            return;
                        }
                    }
                }
            }
        }
    }
    else
    {
        for(i=1; i<=4; i++)
            for(j=1; j<=4; j++)
                if(M[i][j]=='2')
                    M[i][j]='0';
        M[i1][j1]=M[i2][j2]=M[i3][j3]='2';
        for(i=1; i<=4; i++)
        {
            for(j=1; j<=4; j++)
            {
                if(M[i][j]=='0')
                {
                    vecini=0;
                    for(k=0; k<4; k++)
                    {
                        if(M[i+dl[k]][j+dc[k]]=='2')
                            vecini++;
                        if(vecini==2)
                        {
                            M[i][j]='2';
                            return;
                        }
                    }
                }
            }
        }
    }
}

void startGame(short x, short y, short z, int color)
{
    short i=1, j=1, k, l;
    for (k=1; k<=4*z; k+=z)
    {
        j=1;
        for (l=1; l<=4*z; l+=z)
        {
            if (M[i][j] == '0')
            {
                drawSquare(k, l, x, y, z, WHITE);
            }
            else if (M[i][j] == '*' || M[i][j] == '#')
            {
                drawSquare(k, l, x, y, z, COLOR(251, 238, 4));
            }
            else if (M[i][j] == '1')
            {
                drawSquare(k, l, x, y, z, player1Color);
            }
            else if (M[i][j] == '2')
            {
                drawSquare(k, l, x, y, z, player2Color);
            }
            j++;
        }
        i++;
    }
}

void initMenu()
{
    // game title
    setcolor(WHITE);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 7);
    if (lang == 1)
    {
        char titleEn[] = "BONOL GAME";
        outtextxy(69, 69, titleEn);
    }
    else if (lang == 2)
    {
        char titleRo[] = "JOC BONOL";
        outtextxy(69, 69, titleRo);
    }

    // adds an image to the menu
    readimagefile("bonol_main.jpg", getmaxx()/1.6, getmaxy()/4, getmaxx()/3, -getmaxy()/4);

    // copyright
    setcolor(WHITE);
    setbkcolor(itsBg);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 2);
    if (lang == 1)
    {
        char copyrightEn[] = "Developed with <3 by Nicolae Alexandrescu & Valentin Tean";
        outtextxy(getmaxx()-720, getmaxy()-65, copyrightEn);
    }
    else if (lang == 2)
    {
        char copyrightRo[] = "Creat cu <3 de catre Nicolae Alexandrescu & Valentin Tean";
        outtextxy(getmaxx()-720, getmaxy()-65, copyrightRo);
    }
}

void initButtons()
{
    delay(50);
    // play button
    char playEn[] = "PLAY";
    char playRo[] = "JOACA";
    if (checkClick(mousex(), mousey(), getmaxx()/10, getmaxx()/10+105, getmaxy()/2-130, getmaxy()/2-90))
    {
        if (lang == 1)
        {
            drawButton(getmaxx()/10, getmaxy()/2-130, 5, itsYellow, itsBg, playEn);
        }
        else if (lang == 2)
        {
            drawButton(getmaxx()/10, getmaxy()/2-130, 5, itsYellow, itsBg, playRo);
        }
    }
    else
    {
        if (lang == 1)
        {
            drawButton(getmaxx()/10, getmaxy()/2-130, 5, itsBg, itsYellow, playEn);
        }
        else if (lang == 2)
        {
            drawButton(getmaxx()/10, getmaxy()/2-130, 5, itsBg, itsYellow, playRo);
        }
    }

    // options button
    char optionsEn[] = "OPTIONS";
    char optionsRo[] = "OPTIUNI";
    if (checkClick(mousex(), mousey(), getmaxx()/10, getmaxx()/10+182, getmaxy()/2-60, getmaxy()/2-20))
    {
        if (lang == 1)
        {
            drawButton(getmaxx()/10, getmaxy()/2-60, 5, itsYellow, itsBg, optionsEn);
        }
        else if (lang == 2)
        {
            drawButton(getmaxx()/10, getmaxy()/2-60, 5, itsYellow, itsBg, optionsRo);
        }
    }
    else
    {
        if (lang == 1)
        {
            drawButton(getmaxx()/10, getmaxy()/2-60, 5, itsBg, itsYellow, optionsEn);
        }
        else if (lang == 2)
        {
            drawButton(getmaxx()/10, getmaxy()/2-60, 5, itsBg, itsYellow, optionsRo);
        }
    }

    // game info button
    char infoEn[] = "GAME INFO";
    char infoRo[] = "INFORMATII";
    if (checkClick(mousex(), mousey(), getmaxx()/10, getmaxx()/10+232, getmaxy()/2+10, getmaxy()/2+50))
    {
        if (lang == 1)
        {
            drawButton(getmaxx()/10, getmaxy()/2+10, 5, itsYellow, itsBg, infoEn);
        }
        else if (lang == 2)
        {
            drawButton(getmaxx()/10, getmaxy()/2+10, 5, itsYellow, itsBg, infoRo);
        }
    }
    else
    {
        if (lang == 1)
        {
            drawButton(getmaxx()/10, getmaxy()/2+10, 5, itsBg, itsYellow, infoEn);
        }
        else if (lang == 2)
        {
            drawButton(getmaxx()/10, getmaxy()/2+10, 5, itsBg, itsYellow, infoRo);
        }
    }

    // exit button
    char exitEn[] = "EXIT";
    char exitRo[] = "IESI";
    if (checkClick(mousex(), mousey(), getmaxx()/10, getmaxx()/10+102, getmaxy()/2+80, getmaxy()/2+120))
    {
        if (lang == 1)
        {
            drawButton(getmaxx()/10, getmaxy()/2+80, 5, itsYellow, itsBg, exitEn);
        }
        else if (lang == 2)
        {
            drawButton(getmaxx()/10, getmaxy()/2+80, 5, itsYellow, itsBg, exitRo);
        }
    }
    else
    {
        if (lang == 1)
        {
            drawButton(getmaxx()/10, getmaxy()/2+80, 5, itsBg, itsYellow, exitEn);
        }
        else if (lang == 2)
        {
            drawButton(getmaxx()/10, getmaxy()/2+80, 5, itsBg, itsYellow, exitRo);
        }
    }
}

void initGameChoosing()
{
    setcolor(WHITE);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 5);
    if (lang == 1)
    {
        char chooseGameEn[] = "Choose game type:";
        outtextxy(getmaxx()/16, getmaxy()/2-110, chooseGameEn);
    }
    else if (lang == 2)
    {
        char chooseGameRo[] = "Alege tipul jocului:";
        outtextxy(getmaxx()/16, getmaxy()/2-110, chooseGameRo);
    }

    // add image
    readimagefile("bonol_choosing.jpg", getmaxx()/1.6, getmaxy()/4, getmaxx()/3, -getmaxy()/4);

    // go back button
    goBack();
}

void initChooseButtons()
{
    delay(50);
    // PVP button
    char pvpEn[] = "Player vs Player";
    char pvpRo[] = "Jucator vs Jucator";
    if (checkClick(mousex(), mousey(), getmaxx()/11, getmaxx()/11+418, getmaxy()/2-30, getmaxy()/2+12)) {
        if (lang == 1)
        {
            drawButton(getmaxx()/11, getmaxy()/2-30, 5, itsYellow, itsBg, pvpEn);
        }
        else if (lang == 2)
        {
            drawButton(getmaxx()/11, getmaxy()/2-30, 5, itsYellow, itsBg, pvpRo);
        }
    }
    else {
        if (lang == 1)
        {
            drawButton(getmaxx()/11, getmaxy()/2-30, 5, itsBg, itsYellow, pvpEn);
        }
        else if (lang == 2)
        {
            drawButton(getmaxx()/11, getmaxy()/2-30, 5, itsBg, itsYellow, pvpRo);
        }
    }

    // PVC button
    char pvcEn[] = "Player vs Computer";
    char pvcRo[] = "Jucator vs Calculator";
    if (checkClick(mousex(), mousey(), getmaxx()/11, getmaxx()/11+470, getmaxy()/2+50, getmaxy()/2+92))
    {
        if (lang == 1)
        {
            drawButton(getmaxx()/11, getmaxy()/2+50, 5, itsYellow, itsBg, pvcEn);
        }
        else if (lang == 2)
        {
            drawButton(getmaxx()/11, getmaxy()/2+50, 5, itsYellow, itsBg, pvcRo);
        }
    }
    else
    {
        if (lang == 1)
        {
            drawButton(getmaxx()/11, getmaxy()/2+50, 5, itsBg, itsYellow, pvcEn);
        }
        else if (lang == 2)
        {
            drawButton(getmaxx()/11, getmaxy()/2+50, 5, itsBg, itsYellow, pvcRo);
        }
    }
}

void initGame()
{
    short xx=getmaxx()/10, yy=getmaxy()/10-20, zz=getmaxx()/14;
    short fsize, winsize, winy;
    char startTextEn[100], startTextRo[100];
    short mutari;
    short in, jn, k1, k2;

    // display the game table;

    startConfig();
    startGame(xx, yy, zz, itsBg);

    setcolor(WHITE);

    if (getmaxx() >= 1600)
        fsize=3;
    else
        fsize=2;

    settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
    if (lang == 1)
    {
        strcpy(startTextEn, "Choose the player which will start the game");
        outtextxy(getmaxx()/2-150, 100, startTextEn);
        char choiceTextEn[50] = "Button 1 for ";
        strcat(choiceTextEn, player1);
        strcat(choiceTextEn, " or 2 for ");
        strcat(choiceTextEn, player2);
        outtextxy(getmaxx()/2-150, 140, choiceTextEn);
    }
    else if (lang == 2)
    {
        strcpy(startTextRo, "Alegeti jucatorul care va incepe jocul");
        outtextxy(getmaxx()/2-150, 100, startTextRo);
        char choiceTextRo[50] = "Tasta 1 pentru ";
        strcat(choiceTextRo, player1);
        strcat(choiceTextRo, " sau 2 pentru ");
        strcat(choiceTextRo, player2);
        outtextxy(getmaxx()/2-150, 140, choiceTextRo);
    }

    short player;

    while (1)
    {
        player=getch()-'0';
        cout << player << '\n';
        if (player == 1)
        {
            break;
        }
        else if (player == 2)
        {
            break;
        }
        else
        {
            if (lang == 1)
            {
                cleardevice();
                startGame(xx, yy, zz, itsBg);
                setcolor(WHITE);
                strcpy(startTextEn, "This player doesn't exist. Please type 1 or 2");
                outtextxy(getmaxx()/2-150, 100, startTextEn);

            }
            else if (lang == 2)
            {
                cleardevice();
                startGame(xx, yy, zz, itsBg);
                setcolor(WHITE);
                strcpy(startTextRo, "Acest jucator nu exista. Va rugam tastati 1 sau 2");
                outtextxy(getmaxx()/2-150, 100, startTextRo);

            }
        }
    }

    // go back button
    goBack();

    while(1)
    {
        if(player==1)
        {
            if (lang == 1)
            {
                cleardevice();
                setcolor(WHITE);
                settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
                char pTurnTextEn[50] = "It's ";
                strcat(pTurnTextEn, player1);
                strcat(pTurnTextEn, "'s turn");
                outtextxy(getmaxx()/2-150, 100, pTurnTextEn);
                startGame(xx, yy, zz, itsBg);
                goBack();
            }
            else if (lang == 2)
            {
                cleardevice();
                setcolor(WHITE);
                settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
                char pTurnTextRo[50] = "Este randul lui ";
                strcat(pTurnTextRo, player1);
                outtextxy(getmaxx()/2-150, 100, pTurnTextRo);
                startGame(xx, yy, zz, itsBg);
                goBack();
            }
        }
        else
        {
            if (lang == 1)
            {
                cleardevice();
                setcolor(WHITE);
                settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
                char pTurnTextEn[50] = "It's ";
                strcat(pTurnTextEn, player2);
                strcat(pTurnTextEn, "'s turn");
                outtextxy(getmaxx()/2-150, 100, pTurnTextEn);
                startGame(xx, yy, zz, itsBg);
                goBack();
            }
            else if (lang == 2)
            {
                cleardevice();
                setcolor(WHITE);
                settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
                char pTurnTextRo[50] = "Este randul lui ";
                strcat(pTurnTextRo, player2);
                outtextxy(getmaxx()/2-150, 100, pTurnTextRo);
                startGame(xx, yy, zz, itsBg);
                goBack();
            }
        }

        mutari=mutariposibile(player);
        char m1[3];
        m1[0] = '0'+mutari/10;
        m1[1] = '0'+mutari%10;
        m1[2] = '\0';
        if(player==1)
        {
            if (lang == 1)
            {
                setcolor(WHITE);
                settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
                char pMovesTextEn[50];
                strcpy(pMovesTextEn, player1);
                strcat(pMovesTextEn, " has ");
                strcat(pMovesTextEn, m1);
                strcat(pMovesTextEn, " moves");
                outtextxy(getmaxx()/2-150, 140, pMovesTextEn);
            }
            else if (lang == 2)
            {
                setcolor(WHITE);
                settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
                char pMovesTextRo[50];
                strcpy(pMovesTextRo, player1);
                strcat(pMovesTextRo, " are ");
                strcat(pMovesTextRo, m1);
                strcat(pMovesTextRo, " mutari");
                outtextxy(getmaxx()/2-150, 140, pMovesTextRo);
            }
        }
        else
        {

            if (lang == 1)
            {
                setcolor(WHITE);
                settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
                char pMovesTextEn[50];
                strcpy(pMovesTextEn, player2);
                strcat(pMovesTextEn, " has ");
                strcat(pMovesTextEn, m1);
                strcat(pMovesTextEn, " moves");
                outtextxy(getmaxx()/2-150, 140, pMovesTextEn);
            }
            else if (lang == 2)
            {
                setcolor(WHITE);
                settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
                char pMovesTextRo[50];
                strcpy(pMovesTextRo, player2);
                strcat(pMovesTextRo, " are ");
                strcat(pMovesTextRo, m1);
                strcat(pMovesTextRo, " mutari");
                outtextxy(getmaxx()/2-150, 140, pMovesTextRo);
            }
        }

        if(mutari==0)
        {
            if(player==1)
            {
                if (lang == 1)
                {
                    cleardevice();
                    char p2Won[20];
                    strcpy(p2Won, player2);
                    setcolor(player2Color);
                    settextstyle(COMPLEX_FONT, HORIZ_DIR, 7);
                    outtextxy(getmaxx()/2-150, 100, p2Won);
                    char p2Won2[] = "has won!!!";
                    outtextxy(getmaxx()/2-150, 160, p2Won2);
                    setcolor(WHITE);
                    settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
                    char goBackEn[] = "Click on 'Go back' button to go to the menu";
                    outtextxy(getmaxx()/2-150, 240, goBackEn);
                    startGame(xx, yy, zz, itsBg);
                    goBack();
                }
                else if (lang == 2)
                {
                    cleardevice();
                    char p2Won[20];
                    strcpy(p2Won, player2);
                    setcolor(player2Color);
                    settextstyle(COMPLEX_FONT, HORIZ_DIR, 7);
                    outtextxy(getmaxx()/2-150, 100, p2Won);
                    char p2Won2[] = "a castigat!!!";
                    outtextxy(getmaxx()/2-150, 160, p2Won2);
                    setcolor(WHITE);
                    settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
                    char goBackRo[] = "Click pe 'Inapoi' pentru a te intoarce in meniu";
                    outtextxy(getmaxx()/2-150, 240, goBackRo);
                    startGame(xx, yy, zz, itsBg);
                    goBack();
                }
            }
            else
            {
                if (lang == 1)
                {
                    cleardevice();
                    char p1Won[20];
                    strcpy(p1Won, player1);
                    setcolor(player1Color);
                    settextstyle(COMPLEX_FONT, HORIZ_DIR, 7);
                    outtextxy(getmaxx()/2-150, 100, p1Won);
                    char p1Won2[] = "has won!!!";
                    outtextxy(getmaxx()/2-150, 160, p1Won2);
                    setcolor(WHITE);
                    settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
                    char goBackEn[] = "Click on 'Go back' button to go to the menu";
                    outtextxy(getmaxx()/2-150, 240, goBackEn);
                    startGame(xx, yy, zz, itsBg);
                    goBack();
                }
                else if (lang == 2)
                {
                    cleardevice();
                    char p1Won[20];
                    strcpy(p1Won, player1);
                    setcolor(player1Color);
                    settextstyle(COMPLEX_FONT, HORIZ_DIR, 7);
                    outtextxy(getmaxx()/2-150, 100, p1Won);
                    char p1Won2[] = "a castigat!!!";
                    outtextxy(getmaxx()/2-150, 160, p1Won2);
                    setcolor(WHITE);
                    settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
                    char goBackRo[] = "Click pe 'Inapoi' pentru a te intoarce in meniu";
                    outtextxy(getmaxx()/2-150, 240, goBackRo);
                    startGame(xx, yy, zz, itsBg);
                    goBack();
                }
            }

            do
            {
                if (ismouseclick(WM_LBUTTONDOWN) && checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
                {
                    isMenu=1;
                    isGame=0;
                    cleardevice();
                    showMenu();
                    clearmouseclick(WM_LBUTTONDOWN);
                }
                delay(5);
            }
            while (1);
        }

        else
        {
            if (lang == 1)
            {
                setcolor(WHITE);
                settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
                strcpy(startTextEn, "Click on 3 squares: 2 ends and the corner of new L");
                outtextxy(getmaxx()/2-150, 180, startTextEn);
            }
            else if (lang == 2)
            {
                setcolor(WHITE);
                settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
                strcpy(startTextRo, "Click pe 3 patratele: 2 capete si coltul noului L");
                outtextxy(getmaxx()/2-150, 180, startTextRo);
            }
            clearmouseclick(WM_LBUTTONDOWN);
            while(1)
            {
                short nr=0;

                do
                {
                    if (ismouseclick(WM_LBUTTONDOWN))
                    {
                        if (checkClick(mousex(), mousey(), xx, xx+4*zz, yy, yy+4*zz))
                        {
                            cout << mousex() << " " << mousey() << endl;
                            nr++;

                            if (nr == 1)
                            {
                                i1=(mousey()-yy)/zz+1;
                                j1=(mousex()-xx)/zz+1;
                                cout << i1 << " " << j1 << '\n';

                                if (M[i1][j1] == '1')
                                {
                                    drawSquare((i1-1)*zz+1, (j1-1)*zz+1, xx, yy, zz, player1ClickColor);
                                }
                                else if (M[i1][j1] == '2')
                                {
                                    drawSquare((i1-1)*zz+1, (j1-1)*zz+1, xx, yy, zz, player2ClickColor);
                                }
                                else if (M[i1][j1] == '0')
                                {
                                    drawSquare((i1-1)*zz+1, (j1-1)*zz+1, xx, yy, zz, COLOR(150, 150, 150));
                                }
                                else if (M[i1][j1] == '*' || M[i1][j1] == '#')
                                {
                                    drawSquare((i1-1)*zz+1, (j1-1)*zz+1, xx, yy, zz, COLOR(151, 143, 2));
                                }
                            }
                            else if (nr == 2)
                            {
                                i2=(mousey()-yy)/zz+1;
                                j2=(mousex()-xx)/zz+1;
                                cout << i2 << " " << j2 << '\n';

                                if (M[i2][j2] == '1')
                                {
                                    drawSquare((i2-1)*zz+1, (j2-1)*zz+1, xx, yy, zz, player1ClickColor);
                                }
                                else if (M[i2][j2] == '2')
                                {
                                    drawSquare((i2-1)*zz+1, (j2-1)*zz+1, xx, yy, zz, player2ClickColor);
                                }
                                else if (M[i2][j2] == '0')
                                {
                                    drawSquare((i2-1)*zz+1, (j2-1)*zz+1, xx, yy, zz, COLOR(150, 150, 150));
                                }
                                else if (M[i2][j2] == '*' || M[i2][j2] == '#')
                                {
                                    drawSquare((i2-1)*zz+1, (j2-1)*zz+1, xx, yy, zz, COLOR(151, 143, 2));
                                }
                            }
                            else if (nr == 3)
                            {
                                i3=(mousey()-yy)/zz+1;
                                j3=(mousex()-xx)/zz+1;
                                cout << i3 << " " << j3 << '\n';

                                if (M[i3][j3] == '1')
                                {
                                    drawSquare((i3-1)*zz+1, (j3-1)*zz+1, xx, yy, zz, player1ClickColor);
                                }
                                else if (M[i3][j3] == '2')
                                {
                                    drawSquare((i3-1)*zz+1, (j3-1)*zz+1, xx, yy, zz, player2ClickColor);
                                }
                                else if (M[i3][j3] == '0')
                                {
                                    drawSquare((i3-1)*zz+1, (j3-1)*zz+1, xx, yy, zz, COLOR(150, 150, 150));
                                }
                                else if (M[i3][j3] == '*' || M[i3][j3] == '#')
                                {
                                    drawSquare((i3-1)*zz+1, (j3-1)*zz+1, xx, yy, zz, COLOR(151, 143, 2));
                                }
                                delay(500);
                            }
                        }

                        // go back
                        else if (checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
                        {
                            isMenu=1;
                            isGame=0;
                            cleardevice();
                            showMenu();
                        }

                        clearmouseclick(WM_LBUTTONDOWN);
                    }
                    delay(5);
                }
                while (nr<3);

                if(ok(player,i1,j1,i2,j2,i3,j3))
                {
                    mutare(player,i1,j1,i2,j2,i3,j3);
                    cout << "s-a mutat" << '\n';
                    break;
                }
                else
                {
                    if (lang == 1)
                    {
                        setcolor(WHITE);
                        settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
                        strcpy(startTextEn, "A new L can't be created using selected squares. Select other squares");
                        outtextxy(getmaxx()/2-150, 220, startTextEn);
                        startGame(xx, yy, zz, itsBg);
                        goBack();
                    }
                    else if (lang == 2)
                    {
                        setcolor(WHITE);
                        settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
                        strcpy(startTextRo, "Nu se poate forma un nou L cu patratele selectate. Alege alte patrate");
                        outtextxy(getmaxx()/2-150, 220, startTextRo);
                        startGame(xx, yy, zz, itsBg);
                        goBack();
                    }
                }
            }

            while(1)
            {
                cleardevice();
                startGame(xx, yy, zz, itsBg);
                goBack();
                setcolor(WHITE);
                settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
                if (lang == 1)
                {
                    char readPieceEn1[] = "Click on neutral piece which you want to move or use";
                    char readPieceEn2[] = "right click to skip this step";
                    outtextxy(getmaxx()/2-150, 100, readPieceEn1);
                    outtextxy(getmaxx()/2-150, 140, readPieceEn2);
                }
                else if (lang == 2)
                {
                    char readPieceRo1[] = "Click pe piesa neutra pe care doresti sa o muti sau";
                    char readPieceRo2[] = "click dreapta pentru a sari peste acest pas";
                    outtextxy(getmaxx()/2-150, 100, readPieceRo1);
                    outtextxy(getmaxx()/2-150, 140, readPieceRo2);
                }

                clearmouseclick(WM_LBUTTONDOWN);
                clearmouseclick(WM_RBUTTONDOWN);
                do
                {
                    if (ismouseclick(WM_LBUTTONDOWN))
                    {
                        if (checkClick(mousex(), mousey(), xx, xx+4*zz, yy, yy+4*zz))
                        {
                            cout << mousex() << " " << mousey() << endl;
                            in=(mousey()-yy)/zz+1;
                            jn=(mousex()-xx)/zz+1;
                            cout << in << " " << jn << '\n';

                            if (M[in][jn] == '*' || M[in][jn] == '#')
                            {
                                drawSquare((in-1)*zz+1, (jn-1)*zz+1, xx, yy, zz, COLOR(151, 143, 2));
                                setcolor(WHITE);
                            }

                            clearmouseclick(WM_LBUTTONDOWN);
                        }

                        // go back
                        else if (checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
                        {
                            isMenu=1;
                            isGame=0;
                            cleardevice();
                            clearmouseclick(WM_LBUTTONDOWN);
                            showMenu();
                        }

                        break;
                    }
                    else if (ismouseclick(WM_RBUTTONDOWN))
                    {
                        in=jn=-1;
                        clearmouseclick(WM_RBUTTONDOWN);
                        break;
                    }
                    delay(5);
                }
                while (1);

                if (in!=-1)
                {
                    if(M[in][jn]=='*')
                    {
                        setcolor(WHITE);
                        settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
                        if (lang == 1)
                        {
                            char nCoordTextEn[] = "Choose a new free square where do you want to move the piece";
                            outtextxy(getmaxx()/2-150, 180, nCoordTextEn);
                        }
                        else if (lang == 2)
                        {
                            char nCoordTextRo[] = "Alege un patrat liber unde doresti sa muti piesa neutra";
                            outtextxy(getmaxx()/2-150, 180, nCoordTextRo);
                        }

                        do
                        {
                            if (ismouseclick(WM_LBUTTONDOWN))
                            {
                                if (checkClick(mousex(), mousey(), xx, xx+4*zz, yy, yy+4*zz))
                                {
                                    cout << mousex() << " " << mousey() << endl;
                                    k1=(mousey()-yy)/zz+1;
                                    k2=(mousex()-xx)/zz+1;
                                    cout << k1 << " " << k2 << '\n';
                                    clearmouseclick(WM_LBUTTONDOWN);
                                }

                                // go back
                                else if (checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
                                {
                                    isMenu=1;
                                    isGame=0;
                                    cleardevice();
                                    clearmouseclick(WM_LBUTTONDOWN);
                                    showMenu();
                                }

                                break;
                            }
                            delay(5);
                        }
                        while (1);

                        if(M[k1][k2]=='0')
                        {
                            M[k1][k2]='*';
                            M[stea.i][stea.j]='0';
                            stea.i=k1;
                            stea.j=k2;
                            break;
                        }
                        else
                        {
                            setcolor(WHITE);
                            settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
                            if (lang == 1)
                            {
                                char notFreeEn[] = "The position is not free, choose another one";
                                outtextxy(getmaxx()/2-150, 220, notFreeEn);
                            }
                            else if (lang == 2)
                            {
                                char notFreeRo[] = "Pozitia este deja ocupata, alege alta pozitie";
                                outtextxy(getmaxx()/2-150, 220, notFreeRo);
                            }
                            delay(2000);
                        }
                    }

                    else if(M[in][jn]=='#')
                    {
                        setcolor(WHITE);
                        settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
                        if (lang == 1)
                        {
                            char nCoordTextEn[] = "Choose a new free square where do you want to move the piece";
                            outtextxy(getmaxx()/2-150, 180, nCoordTextEn);
                        }
                        else if (lang == 2)
                        {
                            char nCoordTextRo[] = "Alege un patrat liber unde doresti sa muti piesa neutra";
                            outtextxy(getmaxx()/2-150, 180, nCoordTextRo);
                        }

                        do
                        {
                            if (ismouseclick(WM_LBUTTONDOWN))
                            {
                                if (checkClick(mousex(), mousey(), xx, xx+4*zz, yy, yy+4*zz))
                                {
                                    cout << mousex() << " " << mousey() << endl;
                                    k1=(mousey()-yy)/zz+1;
                                    k2=(mousex()-xx)/zz+1;
                                    cout << k1 << " " << k2 << '\n';
                                    clearmouseclick(WM_LBUTTONDOWN);
                                }

                                // go back
                                else if (checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
                                {
                                    isMenu=1;
                                    isGame=0;
                                    cleardevice();
                                    clearmouseclick(WM_LBUTTONDOWN);
                                    showMenu();
                                }

                                break;
                            }
                            delay(5);
                        }
                        while (1);

                        if(M[k1][k2]=='0')
                        {
                            M[k1][k2]='#';
                            M[diez.i][diez.j]='0';
                            diez.i=k1;
                            diez.j=k2;
                            break;
                        }
                        else
                        {
                            if (lang == 1)
                            {
                                char notFreeEn[] = "The position is not free, choose another one";
                                outtextxy(getmaxx()/2-150, 220, notFreeEn);
                            }
                            else if (lang == 2)
                            {
                                char notFreeRo[] = "Pozitia este deja ocupata, alege alta pozitie";
                                outtextxy(getmaxx()/2-150, 220, notFreeRo);
                            }
                            delay(2000);
                        }
                    }
                }
                else
                    break;
            }
        }
        player=3-player;
    }
}

void initOptions()
{
    // the title
    setcolor(itsYellow);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 5);
    if (lang == 1)
    {
        char goptionsEn[] = "Game Options";
        outtextxy(100, 100, goptionsEn);
    }
    else if (lang == 2)
    {
        char goptionsRo[] = "Optiuni Joc";
        outtextxy(100, 100, goptionsRo);
    }

    // options themselves
    setcolor(WHITE);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 4);

    // change language
    if (lang == 1)
    {
        char langEn[] = "Change Language:";
        outtextxy(120, 200, langEn);
    }
    else if (lang == 2)
    {
        char langRo[] = "Schimba Limba:";
        outtextxy(120, 200, langRo);
    }
    if (lang == 2)
    {
        setcolor(WHITE);
    }
    else
    {
        setcolor(itsYellow);
    }
    char roText[] = "RO";
    outtextxy(460, 200, roText);
    if (lang == 1)
    {
        setcolor(WHITE);
    }
    else
    {
        setcolor(itsYellow);
    }
    char enText[] = "EN";
    outtextxy(520, 200, enText);

    // start/stop music
    setcolor(WHITE);
    /*if (lang == 1)
    {
        char musicEn[] = "Music:";
        outtextxy(120, 260, musicEn);
    }
    else if (lang == 2)
    {
        char musicRo[] = "Muzica:";
        outtextxy(120, 260, musicRo);
    }

    if (musicOn == 1)
        setcolor(WHITE);
    else
        setcolor(itsYellow);
    char onBtn[] = "ON";
    outtextxy(460, 260, onBtn);
    if (musicOn == 0)
        setcolor(WHITE);
    else
        setcolor(itsYellow);
    char offBtn[] = "OFF";
    outtextxy(520, 260, offBtn);*/

    // choose song
    if (lang == 1)
    {
        char songEn[] = "Song:";
        outtextxy(120, 260, songEn);
    }
    else if (lang == 2)
    {
        char songRo[] = "Cantec:";
        outtextxy(120, 260, songRo);
    }

    char leftBtn[] = "<";
    outtextxy(460, 260, leftBtn);

    char stopBtn[] = "X";
    outtextxy(500, 260, stopBtn);

    char rightBtn[] = ">";
    outtextxy(537, 260, rightBtn);

    settextstyle(COMPLEX_FONT, HORIZ_DIR, 2);
    outtextxy(580, 265, songName);

    settextstyle(COMPLEX_FONT, HORIZ_DIR, 4);
    // change player 1 square color
    setcolor(WHITE);
    if (lang == 1)
    {
        char ptwoEn[] = "Player 1:";
        outtextxy(120, 320, ptwoEn);
    }
    else if (lang == 2)
    {
        char ptwoRo[] = "Jucator 1:";
        outtextxy(120, 320, ptwoRo);
    }

    if (active1Color == 1)
        itsRectangle(30, 30, 433, 292, 30, WHITE);
    else if (active1Color == 2)
        itsRectangle(30, 30, 493, 292, 30, WHITE);
    else if (active1Color == 3)
        itsRectangle(30, 30, 553, 292, 30, WHITE);
    else if (active1Color == 4)
        itsRectangle(30, 30, 613, 292, 30, WHITE);
    else if (active1Color == 5)
        itsRectangle(30, 30, 673, 292, 30, WHITE);
    itsSquare(30, 30, 433, 292, 30, COLOR(221, 65, 50));
    itsSquare(30, 30, 493, 292, 30, COLOR(249, 103, 20));
    itsSquare(30, 30, 553, 292, 30, COLOR(255, 165, 0));
    itsSquare(30, 30, 613, 292, 30, COLOR(231, 75, 231));
    itsSquare(30, 30, 673, 292, 30, COLOR(255, 128, 149));

    // change player 2 square color
    setcolor(WHITE);
    if (lang == 1)
    {
        char poneEn[] = "Player 2:";
        outtextxy(120, 380, poneEn);
    }
    else if (lang == 2)
    {
        char poneRo[] = "Jucator 2:";
        outtextxy(120, 380, poneRo);
    }

    if (active2Color == 1)
        itsRectangle(30, 30, 433, 348, 30, WHITE);
    else if (active2Color == 2)
        itsRectangle(30, 30, 493, 348, 30, WHITE);
    else if (active2Color == 3)
        itsRectangle(30, 30, 553, 348, 30, WHITE);
    else if (active2Color == 4)
        itsRectangle(30, 30, 613,348, 30, WHITE);
    else if (active2Color == 5)
        itsRectangle(30, 30, 673, 348, 30, WHITE);
    itsSquare(30, 30, 433, 348, 30, COLOR(51, 51, 255));
    itsSquare(30, 30, 493, 348, 30, COLOR(0, 153, 255));
    itsSquare(30, 30, 553, 348, 30, COLOR(0, 255, 255));
    itsSquare(30, 30, 613, 348, 30, COLOR(77, 255, 136));
    itsSquare(30, 30, 673, 348, 30, COLOR(162, 193, 113));

    // go back button
    goBack();
}

void initInfo()
{
    // the title
    setcolor(itsYellow);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 5);
    if (lang == 1)
    {
        char aboutEn[] = "About Bonol";
        outtextxy(100, 100, aboutEn);
    }
    else if (lang == 2)
    {
        char aboutRo[] = "Despre Bonol";
        outtextxy(100, 100, aboutRo);
    }

    // content itself
    setcolor(WHITE);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 2);

    if (lang == 1)
    {
        char info1En[] = "   The L game is a two-player game played on a board of 4x4 squares. Each player has a 3x2 L-shaped";
        char info2En[] = "piece, and there are two 1x1 neutral pieces (yellow squares in the diagram).";
        char info3En[] = "   On each turn, a player must first move their L piece, and then may optionally move one of the ne-";
        char info4En[] = "utral pieces. The game is won by leaving the opponent unable to move their L piece to a new position.";
        char info5En[] = "   Pieces may not overlap or cover other pieces. On moving the L piece, it is picked up and then pla-";
        char info6En[] = "ced in empty squares anywhere on the board. It may be rotated or even flipped over in doing so. The";
        char info7En[] = "only rule is that it must end in a different position from the position it started - thus covering at";
        char info8En[] = "least one square it did not previously cover. To move a neutral piece, a player simply picks it up,";
        char info9En[] = "then places it in an empty square anywhere on the board.";

        outtextxy(120, 200, info1En);
        outtextxy(120, 225, info2En);
        outtextxy(120, 275, info3En);
        outtextxy(120, 300, info4En);
        outtextxy(120, 350, info5En);
        outtextxy(120, 375, info6En);
        outtextxy(120, 400, info7En);
        outtextxy(120, 425, info8En);
        outtextxy(120, 450, info9En);
    }
    else if (lang == 2)
    {
        char info1Ro[] = "   Jocul L game este un joc pentru doua persoane, reprezentat pe o tabla de marimea 4x4. Fiecare ju-";
        char info2Ro[] = "cator are o piesa de marimea 3x2 sub forma literei L. Mai sunt doua piese neutre 1x1 (patratele gal-";
        char info3Ro[] = "bene din diagrama).";
        char info4Ro[] = "   La fiecare mutare, un jucator trebuie sa mute intai piesa L, iar mai apoi poate muta optional si";
        char info5Ro[] = "una din piesele neutre. Jocul este castigat prin lasarea oponentului fara sanse de a-si mai misca";
        char info6Ro[] = "piesa L pe o noua pozitie.";
        char info7Ro[] = "   Piesele nu trebuie sa se suprapuna sau sa acopere alte piese. La mutarea piesei L, aceasta este";
        char info8Ro[] = "ridicata si mai apoi plasata in patratele libere undeva pe tabla. Aceasta poate fi rotita sau chiar";
        char info9Ro[] = "rasturnata in acest sens. Singura regula este ca piesa trebuie sa se termine intr-o pozitie diferita";
        char info10Ro[] = "fata de cea de inceput - acopering astfel cel putin un patrat acoperit anterior. Pentru a muta piesa";
        char info11Ro[] = "neutra, un jucator pur si simplu o ridica, iar mai apoi o plaseaza intr-un patrat liber de pe tabla.";

        outtextxy(120, 200, info1Ro);
        outtextxy(120, 225, info2Ro);
        outtextxy(120, 250, info3Ro);
        outtextxy(120, 300, info4Ro);
        outtextxy(120, 325, info5Ro);
        outtextxy(120, 350, info6Ro);
        outtextxy(120, 400, info7Ro);
        outtextxy(120, 425, info8Ro);
        outtextxy(120, 450, info9Ro);
        outtextxy(120, 475, info10Ro);
        outtextxy(120, 500, info11Ro);
    }

    // go back button
    goBack();
}

void showMenu()
{
    initMenu();

    do
    {
        initButtons();
        setbkcolor(itsBg);

        // if player clicks the PLAY button
        if (ismouseclick(WM_LBUTTONDOWN) && checkClick(mousex(), mousey(), getmaxx()/10, getmaxx()/10+105, getmaxy()/2-130, getmaxy()/2-90))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            isMenu=0;
            //isGame=1;
            isChoosing=1;
            cleardevice();
            //initGame();
            showGameChoosing();
        }

        // if player clicks the OPTIONS button
        if (ismouseclick(WM_LBUTTONDOWN) && checkClick(mousex(), mousey(), getmaxx()/10, getmaxx()/10+182, getmaxy()/2-60, getmaxy()/2-20))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            isMenu=0;
            isOptions=1;
            cleardevice();
            showOptions();
        }

        // if player clicks the GAME INFO button
        if (ismouseclick(WM_LBUTTONDOWN) && checkClick(mousex(), mousey(), getmaxx()/10, getmaxx()/10+232, getmaxy()/2+10, getmaxy()/2+50))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            isMenu=0;
            isInfo=1;
            cleardevice();
            showInfo();
        }

        // if player clicks the EXIT button
        if (ismouseclick(WM_LBUTTONDOWN) && checkClick(mousex(), mousey(), getmaxx()/10, getmaxx()/10+102, getmaxy()/2+80, getmaxy()/2+120))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            closegraph();
            exit(0);
        }
        // prevent some click bugs
        clearmouseclick(WM_LBUTTONDOWN);
        delay(5);
    }
    while (isMenu == 1);
}

void showGameChoosing()
{
    initGameChoosing();

    do
    {
        initChooseButtons();
        setbkcolor(itsBg);

        if (ismouseclick(WM_LBUTTONDOWN))
        {
            // start PVP
            if (checkClick(mousex(), mousey(), getmaxx()/11, getmaxx()/11+418, getmaxy()/2-30, getmaxy()/2+12))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                isChoosing=0;
                isGame=1;
                cleardevice();
                initGame();
            }

            // start PVC
            if (checkClick(mousex(), mousey(), getmaxx()/11, getmaxx()/11+470, getmaxy()/2+50, getmaxy()/2+92))
            {
                cout << mousex() << " " << mousey() << '\n';
            }

            // go back
            if (checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                isMenu=1;
                isOptions=0;
                cleardevice();
                showMenu();
            }
        }


        // prevent some click bugs
        clearmouseclick(WM_LBUTTONDOWN);
        delay(5);
    } while (isChoosing == 1);
}

void showOptions()
{
    initOptions();

    do
    {
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            // change language button
            if (checkClick(mousex(), mousey(), 460, 496, 205, 224))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                lang=2;

                // change players' names
                if (active1Color == 1)
                    strcpy(player1, "Rosu");
                else if (active1Color == 2)
                    strcpy(player1, "Portocaliu Inchis");
                else if (active1Color == 3)
                    strcpy(player1, "Portocaliu");
                else if (active1Color == 4)
                    strcpy(player1, "Violet");
                else if (active1Color == 5)
                    strcpy(player1, "Roz");

                if (active2Color == 1)
                    strcpy(player2, "Albastru");
                else if (active2Color == 2)
                    strcpy(player2, "Albastru Rece");
                else if (active2Color == 3)
                    strcpy(player2, "Aqua");
                else if (active2Color == 4)
                    strcpy(player2, "Verde");
                else if (active2Color == 5)
                    strcpy(player2, "Greenery");
                cleardevice();
                showOptions();
            }

            if (checkClick(mousex(), mousey(), 520, 560, 205, 224))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                lang=1;

                // change players' names
                if (active1Color == 1)
                    strcpy(player1, "Red");
                else if (active1Color == 2)
                    strcpy(player1, "Dark Orange");
                else if (active1Color == 3)
                    strcpy(player1, "Orange");
                else if (active1Color == 4)
                    strcpy(player1, "Violet");
                else if (active1Color == 5)
                    strcpy(player1, "Pink");

                if (active2Color == 1)
                    strcpy(player2, "Blue");
                else if (active2Color == 2)
                    strcpy(player2, "Cool Blue");
                else if (active2Color == 3)
                    strcpy(player2, "Aqua");
                else if (active2Color == 4)
                    strcpy(player2, "Green");
                else if (active2Color == 5)
                    strcpy(player2, "Greenery");

                strcpy(player1, "Red");
                strcpy(player2, "Blue");
                cleardevice();
                showOptions();
            }

            // play/stop music
            /*if (checkClick(mousex(), mousey(), 460, 500, 260, 285))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                musicOn=1;
                PlaySound(TEXT("miyagi_badabum.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
                cleardevice();
                showOptions();
            }

            if (checkClick(mousex(), mousey(), 522, 580, 260, 285))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                musicOn=0;
                PlaySound(NULL, 0, 0);
                cleardevice();
                showOptions();
            }*/

            // change song
            cout << mousex() << " " << mousey() << '\n';
            if (checkClick(mousex(), mousey(), 460, 480, 268, 284))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                if (songNr > 1 && songNr <= 3)
                {
                    songNr-=1;
                }
                cout << songNr;
                if (songNr == 1)
                {
                    strcpy(songName, "Miyagi - Badabum");
                    PlaySound(TEXT("miyagi_badabum.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
                }
                else if (songNr == 2)
                {
                    strcpy(songName, "Post Malone - Better Now");
                    PlaySound(TEXT("post_malone_better_now.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
                }
                else if (songNr == 3)
                {
                    strcpy(songName, "Rihanna - BITCH better have my money");
                    PlaySound(TEXT("rihanna_b_better_have_my_money.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
                }
                cleardevice();
                showOptions();
            }

            if (checkClick(mousex(), mousey(), 538, 555, 268, 284))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                if (songNr >= 1 && songNr < 3)
                {
                    songNr+=1;
                }
                cout << songNr;
                if (songNr == 1)
                {
                    strcpy(songName, "MiyaGi - Badabum");
                    PlaySound(TEXT("miyagi_badabum.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
                }
                else if (songNr == 2)
                {
                    strcpy(songName, "Post Malone - Better Now");
                    PlaySound(TEXT("post_malone_better_now.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
                }
                else if (songNr == 3)
                {
                    strcpy(songName, "Rihanna - BITCH better have my money");
                    PlaySound(TEXT("rihanna_b_better_have_my_money.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
                }

                cleardevice();
                showOptions();
            }

            // stop song
            if (checkClick(mousex(), mousey(), 500, 520, 268, 284))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                strcpy(songName, "OFF");
                PlaySound(NULL, 0, 0);
                cleardevice();
                showOptions();
            }

            // change first player's color to red
            if (checkClick(mousex(), mousey(), 462, 492, 322, 352))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                active1Color=1;
                if (lang == 1)
                {
                    strcpy(player1, "Red");
                }
                else if (lang == 2)
                {
                    strcpy(player1, "Rosu");
                }
                player1Color=COLOR(221, 65, 50);
                player1ClickColor=COLOR(154, 36, 25);
                cleardevice();
                showOptions();
            }

            // change first player's color to dark orange
            if (checkClick(mousex(), mousey(), 522, 552, 322, 352))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                active1Color=2;
                if (lang == 1)
                {
                    strcpy(player1, "Dark Orange");
                }
                else if (lang == 2)
                {
                    strcpy(player1, "Portocaliu Inchis");
                }
                player1Color=COLOR(249, 103, 20);
                player1ClickColor=COLOR(199, 76, 5);
                cleardevice();
                showOptions();
            }

            // change first player's color to orange
            if (checkClick(mousex(), mousey(), 582, 612, 322, 352))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                active1Color=3;
                if (lang == 1)
                {
                    strcpy(player1, "Orange");
                }
                else if (lang == 2)
                {
                    strcpy(player1, "Portocaliu");
                }
                player1Color=COLOR(255, 165, 0);
                player1ClickColor=COLOR(204, 133, 0);
                cleardevice();
                showOptions();
            }

            // change first player's color to violet
            if (checkClick(mousex(), mousey(), 642, 672, 322, 352))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                active1Color=4;
                strcpy(player1, "Violet");
                player1Color=COLOR(231, 75, 231);
                player1ClickColor=COLOR(180, 24, 180);
                cleardevice();
                showOptions();
            }

            // change first player's color to pink
            if (checkClick(mousex(), mousey(), 702, 732, 322, 352))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                active1Color=5;
                if (lang == 1)
                {
                    strcpy(player1, "Pink");
                }
                else if (lang == 2)
                {
                    strcpy(player1, "Roz");
                }
                player1Color=COLOR(255, 128, 149);
                player1ClickColor=COLOR(255, 77, 106);
                cleardevice();
                showOptions();
            }

            // change second player's color to blue
            if (checkClick(mousex(), mousey(), 462, 492, 376, 406))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                active2Color=1;
                if (lang == 1)
                {
                    strcpy(player2, "Blue");
                }
                else if (lang == 2)
                {
                    strcpy(player2, "Albastru");
                }
                player2Color=COLOR(51, 51, 255);
                player2ClickColor=COLOR(0, 0, 204);
                cleardevice();
                showOptions();
            }

            // change second player's color to cool blue
            if (checkClick(mousex(), mousey(), 522, 552, 376, 406))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                active2Color=2;
                if (lang == 1)
                {
                    strcpy(player2, "Cool Blue");
                }
                else if (lang == 2)
                {
                    strcpy(player2, "Albastru Rece");
                }
                player2Color=COLOR(0, 153, 255);
                player2ClickColor=COLOR(0, 107, 179);
                cleardevice();
                showOptions();
            }

            // change second player's color to aqua
            if (checkClick(mousex(), mousey(), 582, 612, 376, 406))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                active2Color=3;
                strcpy(player2, "Aqua");
                player2Color=COLOR(0, 255, 255);
                player2ClickColor=COLOR(0, 179, 179);
                cleardevice();
                showOptions();
            }

            // change second player's color to green
            if (checkClick(mousex(), mousey(), 642, 672, 376, 406))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                active2Color=4;
                if (lang == 1)
                {
                    strcpy(player2, "Green");
                }
                else if (lang == 2)
                {
                    strcpy(player2, "Verde");
                }
                player2Color=COLOR(77, 255, 136);
                player2ClickColor=COLOR(0, 204, 68);
                cleardevice();
                showOptions();
            }

            // change second player's color to greenery
            if (checkClick(mousex(), mousey(), 702, 732, 376, 406))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                active2Color=5;
                strcpy(player2, "Greenery");
                player2Color=COLOR(162, 193, 113);
                player2ClickColor=COLOR(111, 142, 62);
                cleardevice();
                showOptions();
            }

            // go back
            if (checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                isMenu=1;
                isOptions=0;
                cleardevice();
                showMenu();
            }
        }
        // prevent some click bugs
        clearmouseclick(WM_LBUTTONDOWN);
        delay(5);
    }
    while (isOptions == 1);
}

void showInfo()
{
    initInfo();

    do
    {
        // go back
        if (ismouseclick(WM_LBUTTONDOWN) && checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            isMenu=1;
            isInfo=0;
            cleardevice();
            showMenu();
        }
        // prevent some click bugs
        clearmouseclick(WM_LBUTTONDOWN);
        delay(5);
    }
    while (isInfo == 1);
}

int main()
{
    if (musicOn)
    {
        PlaySound(TEXT("miyagi_badabum.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    }
    initwindow(screenWidth, screenHeight, "BONOL", -3, -3);
    setbkcolor(itsBg);
    cleardevice();

    showMenu();

    getch();
    return 0;
}
