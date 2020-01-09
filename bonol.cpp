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
bool isChoosingDifficulty=0;
bool isOptions=0;
bool isInfo=0;
void showMenu();
void showGameChoosing();
void showGameDiffChoosing();
void showOptions();
void showInfo();
void initMenu();
void initGameChoosing();
void initGameDifficulty();
void initPvpGame();
void initEasyPvcGame();
void initMediumPvcGame();
void initHardPvcGame();
void initOptions();
void initInfo();
void mutare(short player, short i1, short j1, short i2, short j2, short i3, short j3);

// default language (1 for English, 2 for Romanian or 3 for French)
short lang=1;

// music is on/off by default
bool musicOn=1;

// default song name and number
char songName[50] = "Miyagi - Badabum";
short songNr=1;

short fsize;

char startText[100], choiceText[100], pTurnText[50], pMovesText[50], pWonTxt1[20], pWonTxt2[20], goToMenuTxt[70], newCoordTxt[75], notFreeTxt[75], readPieceTxt1[70], readPieceTxt2[70];

// get screen width and height
DWORD screenWidth = GetSystemMetrics(SM_CXSCREEN);
DWORD screenHeight = GetSystemMetrics(SM_CYSCREEN);

struct piesaneutra
{
    short i, j;
} stea, diez, liber[15];

struct L
{
    short i1, j1, i2, j2, i3, j3;
} L1, L2;

struct hardMoves
{
    short pMoves, i1, j1, i2, j2, i3, j3;
} v[20];

struct liber
{
    short i, j, pMoves;
} liberDiez[10], liberStea[10];

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

void goToMenu()
{
    char menuTxt[20];
    if (lang == 1)
    {
        strcpy(menuTxt, "Menu");
    }
    else if (lang == 2)
    {
        strcpy(menuTxt, "Meniu");
    }
    else if (lang == 3)
    {
        strcpy(menuTxt, "Menu");
    }
    drawButton(getmaxx()-120, getmaxy()-80, 3, WHITE, itsBg, menuTxt);
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

bool okPvc(short player, short i1, short j1, short i2, short j2, short i3, short j3, short x, short y, char neutralPiece)
{
    if(i1<1 || i1>4 || j1<1 || j1>4 || i2<1 || i2>4 || j2<1 || j2>4 || i3<1 || i3>4 || j3<1 || j3>4)
        return false;
    short i, j, vecini, k, st=0, diz=0, unu=0, doi=0, k1, k2;
    char N[6][6];
    for(i=1; i<=4; i++)
        for(j=1; j<=4; j++)
            N[i][j]=M[i][j];
    N[i1][j1]=N[i2][j2]=N[i3][j3]='$';
    if (neutralPiece == '*')
    {
        N[x][y]=neutralPiece;
        N[stea.i][stea.j]='0';
    }
    else
    {
        N[x][y]=neutralPiece;
        N[diez.i][diez.j]='0';
    }
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

int possibleMoves(short player, short i1, short j1, short i2, short j2, short i3, short j3)
{
    short i, j, nr=0, k, pi, pj;
    char C[6][6], A[6][6];
    for (i=1; i<=4; i++)
    {
        for (j=1; j<=4; j++)
        {
            A[i][j]=M[i][j];
        }
    }
    mutare(3-player, i1, j1, i2, j2, i3, j3);
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
    for (i=1; i<=4; i++)
    {
        for (j=1; j<=4; j++)
        {
            M[i][j]=A[i][j];
        }
    }
    return nr;
}

int mPosNeutPiece(short player, short x, short y, char neutralPiece)
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
        if (neutralPiece == '*')
        {
            C[x][y]=neutralPiece;
            C[stea.i][stea.j]='0';
        }
        else
        {
            C[x][y]=neutralPiece;
            C[diez.i][diez.j]='0';
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
                            if(okPvc(1, i, j, pi, pj, i+pi3[2*k], j+pj3[2*k], x, y, neutralPiece))
                                nr++;
                            if(okPvc(1, i, j, pi, pj, i+pi3[2*k+1], j+pj3[2*k+1], x, y, neutralPiece))
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
        if (neutralPiece == '*')
        {
            C[x][y]=neutralPiece;
            C[stea.i][stea.j]='0';
        }
        else
        {
            C[x][y]=neutralPiece;
            C[diez.i][diez.j]='0';
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
                            if(okPvc(2, i, j, pi, pj, i+pi3[2*k], j+pj3[2*k], x, y, neutralPiece))
                                nr++;
                            if(okPvc(2, i, j, pi, pj, i+pi3[2*k+1], j+pj3[2*k+1], x, y, neutralPiece))
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

int mutariPosibilePvc(short player)
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
                            {
                                nr++;
                                v[nr].i1=i;
                                v[nr].j1=j;
                                v[nr].i2=pi;
                                v[nr].j2=pj;
                                v[nr].i3=i+pi3[2*k];
                                v[nr].j3=j+pj3[2*k];
                            }
                            if(ok(1, i, j, pi, pj, i+pi3[2*k+1], j+pj3[2*k+1]))
                            {
                                nr++;
                                v[nr].i1=i;
                                v[nr].j1=j;
                                v[nr].i2=pi;
                                v[nr].j2=pj;
                                v[nr].i3=i+pi3[2*k+1];
                                v[nr].j3=j+pj3[2*k+1];
                            }
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
                            {
                                nr++;
                                v[nr].i1=i;
                                v[nr].j1=j;
                                v[nr].i2=pi;
                                v[nr].j2=pj;
                                v[nr].i3=i+pi3[2*k];
                                v[nr].j3=j+pj3[2*k];
                            }
                            if(ok(2, i, j, pi, pj, i+pi3[2*k+1], j+pj3[2*k+1]))
                            {
                                nr++;
                                v[nr].i1=i;
                                v[nr].j1=j;
                                v[nr].i2=pi;
                                v[nr].j2=pj;
                                v[nr].i3=i+pi3[2*k+1];
                                v[nr].j3=j+pj3[2*k+1];
                            }
                            C[i][j]='%';
                        }
                    }
                }
            }
        }
    }
    return nr;
}

int mutariPosibileHardPvc(short player)
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
                            {
                                nr++;
                                v[nr].i1=i;
                                v[nr].j1=j;
                                v[nr].i2=pi;
                                v[nr].j2=pj;
                                v[nr].i3=i+pi3[2*k];
                                v[nr].j3=j+pj3[2*k];
                                v[nr].pMoves=possibleMoves(3-player, i, j, pi, pj, i+pi3[2*k], j+pj3[2*k]);
                            }
                            if(ok(1, i, j, pi, pj, i+pi3[2*k+1], j+pj3[2*k+1]))
                            {
                                nr++;
                                v[nr].i1=i;
                                v[nr].j1=j;
                                v[nr].i2=pi;
                                v[nr].j2=pj;
                                v[nr].i3=i+pi3[2*k+1];
                                v[nr].j3=j+pj3[2*k+1];
                                v[nr].pMoves=possibleMoves(3-player, i, j, pi, pj, i+pi3[2*k+1], j+pj3[2*k+1]);
                            }
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
                            {
                                nr++;
                                v[nr].i1=i;
                                v[nr].j1=j;
                                v[nr].i2=pi;
                                v[nr].j2=pj;
                                v[nr].i3=i+pi3[2*k];
                                v[nr].j3=j+pj3[2*k];
                                v[nr].pMoves=possibleMoves(3-player, i, j, pi, pj, i+pi3[2*k], j+pj3[2*k]);
                            }
                            if(ok(2, i, j, pi, pj, i+pi3[2*k+1], j+pj3[2*k+1]))
                            {
                                nr++;
                                v[nr].i1=i;
                                v[nr].j1=j;
                                v[nr].i2=pi;
                                v[nr].j2=pj;
                                v[nr].i3=i+pi3[2*k+1];
                                v[nr].j3=j+pj3[2*k+1];
                                v[nr].pMoves=possibleMoves(3-player, i, j, pi, pj, i+pi3[2*k+1], j+pj3[2*k+1]);
                            }
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
    char gameTitle[15];
    if (lang == 1)
    {
        strcpy(gameTitle, "BONOL GAME");
    }
    else if (lang == 2)
    {
        strcpy(gameTitle, "JOC BONOL");
    }
    else if (lang == 3)
    {
        strcpy(gameTitle, "JEU BONOL");
    }
    outtextxy(69, 69, gameTitle);

    // adds an image to the menu
    readimagefile("img/bonol_main.jpg", getmaxx()/1.6, getmaxy()/4, getmaxx()/3, -getmaxy()/4);

    // copyright
    setcolor(WHITE);
    setbkcolor(itsBg);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 2);
    char copyright[70];
    if (lang == 1)
    {
        strcpy(copyright, "Developed with <3 by Nicolae Alexandrescu & Valentin Tean");
    }
    else if (lang == 2)
    {
        strcpy(copyright, "Creat cu <3 de catre Nicolae Alexandrescu & Valentin Tean");
    }
    else if (lang == 3)
    {
        strcpy(copyright, "Développé avec <3 par Nicolae Alexandrescu & Valentin Tean");
    }
    outtextxy(getmaxx()-720, getmaxy()-65, copyright);
}

void initButtons()
{
    delay(50);
    // play button
    char playTxt[10];

    if (lang == 1)
    {
        strcpy(playTxt, "PLAY");
    }
    else if (lang == 2)
    {
        strcpy(playTxt, "JOACA");
    }
    else if (lang == 3)
    {
        strcpy(playTxt, "JOUER");
    }

    if (checkClick(mousex(), mousey(), getmaxx()/10, getmaxx()/10+105, getmaxy()/2-130, getmaxy()/2-90))
    {
        drawButton(getmaxx()/10, getmaxy()/2-130, 5, itsYellow, itsBg, playTxt);
    }
    else
    {
        drawButton(getmaxx()/10, getmaxy()/2-130, 5, itsBg, itsYellow, playTxt);
    }

    // options button
    char optionsTxt[10];

    if (lang == 1 || lang == 3)
    {
        strcpy(optionsTxt, "OPTIONS");
    }
    else if (lang == 2)
    {
        strcpy(optionsTxt, "OPTIUNI");
    }

    if (checkClick(mousex(), mousey(), getmaxx()/10, getmaxx()/10+182, getmaxy()/2-60, getmaxy()/2-20))
    {
        drawButton(getmaxx()/10, getmaxy()/2-60, 5, itsYellow, itsBg, optionsTxt);
    }
    else
    {
        drawButton(getmaxx()/10, getmaxy()/2-60, 5, itsBg, itsYellow, optionsTxt);
    }

    // game info button
    char infoTxt[15];

    if (lang == 1 || lang == 3)
    {
        strcpy(infoTxt, "INFORMATIONS");
    }
    else if (lang == 2)
    {
        strcpy(infoTxt, "INFORMATII");
    }

    if (checkClick(mousex(), mousey(), getmaxx()/10, getmaxx()/10+310, getmaxy()/2+10, getmaxy()/2+50))
    {
        drawButton(getmaxx()/10, getmaxy()/2+10, 5, itsYellow, itsBg, infoTxt);
    }
    else
    {
        drawButton(getmaxx()/10, getmaxy()/2+10, 5, itsBg, itsYellow, infoTxt);
    }

    // exit button
    char exitTxt[15];

    if (lang == 1)
    {
        strcpy(exitTxt, "EXIT GAME");
    }
    else if (lang == 2)
    {
        strcpy(exitTxt, "IESIRE");
    }
    else if (lang == 3)
    {
        strcpy(exitTxt, "SORTIE");
    }

    if (checkClick(mousex(), mousey(), getmaxx()/10, getmaxx()/10+232, getmaxy()/2+80, getmaxy()/2+120))
    {
        drawButton(getmaxx()/10, getmaxy()/2+80, 5, itsYellow, itsBg, exitTxt);
    }
    else
    {
        drawButton(getmaxx()/10, getmaxy()/2+80, 5, itsBg, itsYellow, exitTxt);
    }
}

void initGameChoosing()
{
    setcolor(WHITE);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 5);
    char chooseGameTxt[30];

    if (lang == 1)
    {
        strcpy(chooseGameTxt, "Choose game type:");
    }
    else if (lang == 2)
    {
        strcpy(chooseGameTxt, "Alege tipul jocului:");
    }
    else if (lang == 3)
    {
        strcpy(chooseGameTxt, "Choisissez le type de jeu:");
    }
    outtextxy(getmaxx()/16, getmaxy()/2-110, chooseGameTxt);

    // add image
    readimagefile("img/bonol_choosing.jpg", getmaxx()/1.6, getmaxy()/4, getmaxx()/3, -getmaxy()/4);

    // go to menu button
    goToMenu();
}

void initChooseButtons()
{
    delay(50);

    // PVP button
    char pvpTxt[20];

    if (lang == 1)
    {
        strcpy(pvpTxt, "Player vs Player");
    }
    else if (lang == 2)
    {
        strcpy(pvpTxt, "Jucator vs Jucator");
    }
    else if (lang == 3)
    {
        strcpy(pvpTxt, "Joueur vs Joueur");
    }

    if (checkClick(mousex(), mousey(), getmaxx()/11, getmaxx()/11+418, getmaxy()/2-30, getmaxy()/2+12))
    {
        drawButton(getmaxx()/11, getmaxy()/2-30, 5, itsYellow, itsBg, pvpTxt);
    }
    else
    {
        drawButton(getmaxx()/11, getmaxy()/2-30, 5, itsBg, itsYellow, pvpTxt);
    }

    // PVC button
    char pvcTxt[25];

    if (lang == 1)
    {
        strcpy(pvcTxt, "Player vs Computer");
    }
    else if (lang == 2)
    {
        strcpy(pvcTxt, "Jucator vs Calculator");
    }
    else if (lang == 3)
    {
        strcpy(pvcTxt, "Joueur vs Ordinateur");
    }

    if (checkClick(mousex(), mousey(), getmaxx()/11, getmaxx()/11+470, getmaxy()/2+50, getmaxy()/2+92))
    {
        drawButton(getmaxx()/11, getmaxy()/2+50, 5, itsYellow, itsBg, pvcTxt);
    }
    else
    {
        drawButton(getmaxx()/11, getmaxy()/2+50, 5, itsBg, itsYellow, pvcTxt);
    }
}

void initGameDifficulty()
{
    setcolor(WHITE);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 5);
    char chooseGameDifficulty[30];

    if (lang == 1)
    {
        strcpy(chooseGameDifficulty, "Choose game difficulty:");
    }
    else if (lang == 2)
    {
        strcpy(chooseGameDifficulty, "Alege dificultatea jocului:");
    }
    else if (lang == 3)
    {
        strcpy(chooseGameDifficulty, "Choisissez la difficulté du jeu:");
    }
    outtextxy(getmaxx()/2, getmaxy()/2-140, chooseGameDifficulty);

    // add img
    readimagefile("img/final_pos.jpg", getmaxx()/13.66, getmaxy()/3.2, getmaxx()/2.27, getmaxy()/1.42);

    // go to menu button
    goToMenu();
}

void initDifficultyButtons()
{
    delay(50);
    char easyTxt[10], mediumTxt[10], hardTxt[10];

    if (lang == 1)
    {
        strcpy(easyTxt, "Easy");
        strcpy(mediumTxt, "Medium");
        strcpy(hardTxt, "Hard");
    }
    else if (lang == 2)
    {
        strcpy(easyTxt, "Usor");
        strcpy(mediumTxt, "Mediu");
        strcpy(hardTxt, "Dificil");
    }
    else if (lang == 3)
    {
        strcpy(easyTxt, "Simple");
        strcpy(mediumTxt, "Médium");
        strcpy(hardTxt, "Difficile");
    }

    // easy button
    if (checkClick(mousex(), mousey(), getmaxx()/2+60, getmaxx()/2+195, getmaxy()/2-60, getmaxy()/2-8))
    {
        drawButton(getmaxx()/2+60, getmaxy()/2-60, 6, itsYellow, itsBg, easyTxt);
    }
    else
    {
        drawButton(getmaxx()/2+60, getmaxy()/2-60, 6, itsBg, itsYellow, easyTxt);
    }

    // medium button
    if (checkClick(mousex(), mousey(), getmaxx()/2+60, getmaxx()/2+260, getmaxy()/2+20, getmaxy()/2+72))
    {
        drawButton(getmaxx()/2+60, getmaxy()/2+20, 6, itsYellow, itsBg, mediumTxt);
    }
    else
    {
        drawButton(getmaxx()/2+60, getmaxy()/2+20, 6, itsBg, itsYellow, mediumTxt);
    }

    // hard button
    if (checkClick(mousex(), mousey(), getmaxx()/2+60, getmaxx()/2+195, getmaxy()/2+100, getmaxy()/2+152))
    {
        drawButton(getmaxx()/2+60, getmaxy()/2+100, 6, itsYellow, itsBg, hardTxt);
    }
    else
    {
        drawButton(getmaxx()/2+60, getmaxy()/2+100, 6, itsBg, itsYellow, hardTxt);
    }
}

void moveFirstTxt()
{
    if (lang == 1)
    {
        strcpy(startText, "Choose the player who will move first");
        strcpy(choiceText, "Button 1 for ");
        strcat(choiceText, player1);
        strcat(choiceText, " or 2 for ");
        strcat(choiceText, player2);
    }
    else if (lang == 2)
    {
        strcpy(startText, "Alegeti jucatorul care va muta primul");
        strcpy(choiceText, "Tasta 1 pentru ");
        strcat(choiceText, player1);
        strcat(choiceText, " sau 2 pentru ");
        strcat(choiceText, player2);
    }
    else if (lang == 3)
    {
        strcpy(startText, "Choisissez le joueur qui commencera le jeu");
        strcpy(choiceText, "Bouton 1 pour ");
        strcat(choiceText, player1);
        strcat(choiceText, " ou 2 pour ");
        strcat(choiceText, player2);
    }
    setcolor(WHITE);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
    outtextxy(getmaxx()/2-150, 100, startText);
    outtextxy(getmaxx()/2-150, 140, choiceText);
}

void doesntExistTxt()
{
    if (lang == 1)
    {
        strcpy(startText, "This player doesn't exist. Please type 1 or 2");
    }
    else if (lang == 2)
    {
        strcpy(startText, "Acest jucator nu exista. Va rugam tastati 1 sau 2");
    }
    else if (lang == 3)
    {
        strcpy(startText, "Ce joueur n'existe pas. Veuillez saisir 1 ou 2");
    }
    setcolor(WHITE);
    outtextxy(getmaxx()/2-150, 100, startText);
}

void playerTurnTxt(char player[20])
{
    if (lang == 1)
    {
        strcpy(pTurnText, "It's ");
        strcat(pTurnText, player);
        strcat(pTurnText, "'s turn");
    }
    else if (lang == 2)
    {
        strcpy(pTurnText, "Este randul lui ");
        strcat(pTurnText, player);
    }
    else if (lang == 3)
    {
        strcpy(pTurnText, "C'est au tour de ");
        strcat(pTurnText, player);
    }
    setcolor(WHITE);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
    outtextxy(getmaxx()/2-150, 100, pTurnText);
}

void possibleMovesTxt(char player[20], char moves[3])
{
    if (lang == 1)
    {
        strcpy(pMovesText, player);
        strcat(pMovesText, " has ");
        strcat(pMovesText, moves);
        strcat(pMovesText, " possible moves");
    }
    else if (lang == 2)
    {
        strcpy(pMovesText, player);
        strcat(pMovesText, " are ");
        strcat(pMovesText, moves);
        strcat(pMovesText, " mutari posibile");
    }
    else if (lang == 3)
    {
        strcpy(pMovesText, player);
        strcat(pMovesText, " a ");
        strcat(pMovesText, moves);
        strcat(pMovesText, " mouvements possibles");
    }
    setcolor(WHITE);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
    outtextxy(getmaxx()/2-150, 140, pMovesText);
}

void hasWonTxt(char player[20], int playerColor)
{
    strcpy(pWonTxt1, player);
    setcolor(playerColor);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 7);
    outtextxy(getmaxx()/2-150, 100, pWonTxt1);
    if (lang == 1)
    {
        strcpy(pWonTxt2, "has won!!!");
        strcpy(goToMenuTxt, "Click on 'Menu' button to go to the menu");
    }
    else if (lang == 2)
    {
        strcpy(pWonTxt2, "a castigat!!!");
        strcpy(goToMenuTxt, "Click pe 'Meniu' pentru a te intoarce in meniu");
    }
    else if (lang == 3)
    {
        strcpy(pWonTxt2, "a gagne!!!");
        strcpy(goToMenuTxt, "Cliquez sur le 'Menu' pour accéder au menu");
    }
    outtextxy(getmaxx()/2-150, 160, pWonTxt2);
    setcolor(WHITE);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
    outtextxy(getmaxx()/2-150, 240, goToMenuTxt);
}

void clickOnSquareTxt()
{
    if (lang == 1)
    {
        strcpy(startText, "Click on 3 squares: 2 ends and the corner of new L");
    }
    else if (lang == 2)
    {
        strcpy(startText, "Click pe 3 patratele: 2 capete si coltul noului L");
    }
    else if (lang == 3)
    {
        strcpy(startText, "Cliquez sur 3 carrés: 2 extrémités et le coin du nouveau L");
    }
    setcolor(WHITE);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
    outtextxy(getmaxx()/2-150, 180, startText);
}

void cantCreateTxt()
{
    if (lang == 1)
    {
        strcpy(startText, "A new L can't be created using selected squares. Select other squares");
    }
    else if (lang == 2)
    {
        strcpy(startText, "Nu se poate forma un nou L cu patratele selectate. Alege alte patrate");
    }
    else if (lang == 3)
    {
        strcpy(startText, "Un nouveau L ne peut pas être créé. Sélectionnez d'autres carrés");
    }
    setcolor(WHITE);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
    outtextxy(getmaxx()/2-150, 220, startText);
}

void movePieceTxt()
{
    if (lang == 1)
    {
        strcpy(readPieceTxt1, "Click on neutral piece which you want to move or use");
        strcpy(readPieceTxt2, "right click to skip this step");
    }
    else if (lang == 2)
    {
        strcpy(readPieceTxt1, "Click pe piesa neutra pe care doresti sa o muti sau");
        strcpy(readPieceTxt2, "click dreapta pentru a sari peste acest pas");
    }
    else if (lang == 3)
    {
        strcpy(readPieceTxt1, "Cliquez sur la pièce neutre que vous souhaitez déplacer");
        strcpy(readPieceTxt2, "ou utilisez le clic droit pour ignorer cette étape");
    }
    setcolor(WHITE);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
    outtextxy(getmaxx()/2-150, 100, readPieceTxt1);
    outtextxy(getmaxx()/2-150, 140, readPieceTxt2);
}

void chooseSquareTxt()
{
    if (lang == 1)
    {
        strcpy(newCoordTxt, "Choose a new free square where do you want to move the piece");
    }
    else if (lang == 2)
    {
        strcpy(newCoordTxt, "Alege un patrat liber unde doresti sa muti piesa neutra");
    }
    else if (lang == 3)
    {
        strcpy(newCoordTxt, "Choisissez un nouveau carré libre où voulez-vous déplacer la pièce");
    }
    setcolor(WHITE);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
    outtextxy(getmaxx()/2-150, 180, newCoordTxt);
}

void posNotFreeTxt()
{
    if (lang == 1)
    {
        strcpy(notFreeTxt, "The position is not free, choose another one");
    }
    else if (lang == 2)
    {
        strcpy(notFreeTxt, "Pozitia este deja ocupata, alege alta pozitie");
    }
    else if (lang == 3)
    {
        strcpy(notFreeTxt, "Le poste n'est pas libre, choisissez-en un autre");
    }
    setcolor(WHITE);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
    outtextxy(getmaxx()/2-150, 220, notFreeTxt);
}

void wantedPlayerTxt()
{
    if (lang == 1)
    {
        strcpy(startText, "Choose the player which you want to be");
        strcpy(choiceText, "Button 1 for ");
        strcat(choiceText, player1);
        strcat(choiceText, " or 2 for ");
        strcat(choiceText, player2);
    }
    else if (lang == 2)
    {
        strcpy(startText, "Alege jucatorul care vrei sa fii");
        strcpy(choiceText, "Tasta 1 pentru ");
        strcat(choiceText, player1);
        strcat(choiceText, " sau 2 pentru ");
        strcat(choiceText, player2);
    }
    else if (lang == 3)
    {
        strcpy(startText, "Choisissez le joueur que vous voulez être");
        strcpy(choiceText, "Bouton 1 pour ");
        strcat(choiceText, player1);
        strcat(choiceText, " ou 2 pour ");
        strcat(choiceText, player2);
    }
    setcolor(WHITE);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
    outtextxy(getmaxx()/2-150, 100, startText);
    outtextxy(getmaxx()/2-150, 140, choiceText);
}

void initPvpGame()
{
    short xx=getmaxx()/10, yy=getmaxy()/10-20, zz=getmaxx()/14;
    short mutari;
    short in, jn, k1, k2;

    // display the game table;

    startConfig();
    startGame(xx, yy, zz, itsBg);

    // display the 'who will move first' text
    moveFirstTxt();

    short player;

    while (1)
    {
        player=getch()-'0';
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
            cleardevice();
            startGame(xx, yy, zz, itsBg);

            // display the 'player does not exist' text
            doesntExistTxt();
        }
    }

    // go to menu button
    goToMenu();

    while(1)
    {
        char pTurnText[50];
        if(player==1)
        {
            cleardevice();

            // display the 'it is player's turn' text
            playerTurnTxt(player1);
        }
        else
        {
            cleardevice();

            // display the 'it is player's turn' text
            playerTurnTxt(player2);
        }
        startGame(xx, yy, zz, itsBg);
        goToMenu();

        mutari=mutariposibile(player);
        char m1[3];
        m1[0] = '0'+mutari/10;
        m1[1] = '0'+mutari%10;
        m1[2] = '\0';

        if(player==1)
        {
            // display player's possible moves text
            possibleMovesTxt(player1, m1);
        }
        else
        {
            // display player's possible moves text
            possibleMovesTxt(player2, m1);
        }

        if(mutari==0)
        {
            cleardevice();
            if(player==1)
            {
                // display the player has won text
                hasWonTxt(player2, player2Color);
            }
            else
            {
                // display the player has won text
                hasWonTxt(player1, player1Color);
            }
            startGame(xx, yy, zz, itsBg);
            goToMenu();

            do
            {
                if (ismouseclick(WM_LBUTTONDOWN) && checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
                {
                    isMenu=1;
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
            setcolor(WHITE);

            // display the click on square txt
            clickOnSquareTxt();

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
                            nr++;

                            if (nr == 1)
                            {
                                i1=(mousey()-yy)/zz+1;
                                j1=(mousex()-xx)/zz+1;

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

                                if (i1!=i2 || j1!=j2)
                                {
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
                                else
                                {
                                    nr--;
                                }
                            }
                            else if (nr == 3)
                            {
                                i3=(mousey()-yy)/zz+1;
                                j3=(mousex()-xx)/zz+1;

                                if ((i3!=i2 && i3!=i1) || (j3!=i2 && j3!=j1))
                                {
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
                                else
                                {
                                    nr--;
                                }
                            }
                        }

                        // go to menu button
                        else if (checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
                        {
                            isMenu=1;
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
                    break;
                }
                else
                {
                    // display the can't create a new L text
                    cantCreateTxt();

                    startGame(xx, yy, zz, itsBg);
                    goToMenu();
                }
            }

            while(1)
            {
                cleardevice();
                startGame(xx, yy, zz, itsBg);
                goToMenu();

                // display the click on neutral piece text
                movePieceTxt();

                clearmouseclick(WM_LBUTTONDOWN);
                clearmouseclick(WM_RBUTTONDOWN);
                do
                {
                    if (ismouseclick(WM_LBUTTONDOWN))
                    {
                        if (checkClick(mousex(), mousey(), xx, xx+4*zz, yy, yy+4*zz))
                        {
                            in=(mousey()-yy)/zz+1;
                            jn=(mousex()-xx)/zz+1;

                            if (M[in][jn] == '*' || M[in][jn] == '#')
                            {
                                drawSquare((in-1)*zz+1, (jn-1)*zz+1, xx, yy, zz, COLOR(151, 143, 2));
                                setcolor(WHITE);
                            }

                            clearmouseclick(WM_LBUTTONDOWN);
                        }

                        // go to menu button
                        else if (checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
                        {
                            isMenu=1;
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
                        // display the choose free square for piece text
                        chooseSquareTxt();

                        do
                        {
                            if (ismouseclick(WM_LBUTTONDOWN))
                            {
                                if (checkClick(mousex(), mousey(), xx, xx+4*zz, yy, yy+4*zz))
                                {
                                    k1=(mousey()-yy)/zz+1;
                                    k2=(mousex()-xx)/zz+1;
                                    clearmouseclick(WM_LBUTTONDOWN);
                                }

                                // go to menu button
                                else if (checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
                                {
                                    isMenu=1;
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
                            // display the not free position text
                            posNotFreeTxt();

                            delay(2000);
                        }
                    }

                    else if(M[in][jn]=='#')
                    {
                        // display the choose free square for piece text
                        chooseSquareTxt();

                        do
                        {
                            if (ismouseclick(WM_LBUTTONDOWN))
                            {
                                if (checkClick(mousex(), mousey(), xx, xx+4*zz, yy, yy+4*zz))
                                {
                                    k1=(mousey()-yy)/zz+1;
                                    k2=(mousex()-xx)/zz+1;
                                    clearmouseclick(WM_LBUTTONDOWN);
                                }

                                // go to menu button
                                else if (checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
                                {
                                    isMenu=1;
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
                            // display the not free position text
                            posNotFreeTxt();

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

void initEasyPvcGame()
{
    short xx=getmaxx()/10, yy=getmaxy()/10-20, zz=getmaxx()/14;
    short mutari;
    short in, jn, k1, k2;

    // display the game table;
    startConfig();
    startGame(xx, yy, zz, itsBg);

    setcolor(WHITE);

    // display the choose the player who you wanna be text
    wantedPlayerTxt();

    short player;

    while (1)
    {
        player=getch()-'0';
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
            cleardevice();
            startGame(xx, yy, zz, itsBg);

            // display the player doesn't exist text
            doesntExistTxt();
        }
    }
    cleardevice();

    // display the game table;
    startConfig();
    startGame(xx, yy, zz, itsBg);

    setcolor(WHITE);

    short computer=3-player;

    // display the player who will move first text
    moveFirstTxt();

    while (1)
    {
        player=getch()-'0';
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
            cleardevice();
            startGame(xx, yy, zz, itsBg);

            // display the player doesn't exist text
            doesntExistTxt();
        }
    }

    // go to menu button
    goToMenu();

    while(1)
    {
        cleardevice();
        if(player==1)
        {
            // display the it's player's turn text
            playerTurnTxt(player1);
        }
        else
        {
            // display the it's player's turn text
            playerTurnTxt(player2);
        }
        startGame(xx, yy, zz, itsBg);
        goToMenu();

        if (computer==player)
        {
            mutari=mutariPosibilePvc(computer);
        }
        else
        {
            mutari=mutariposibile(player);
        }

        char m1[3];
        m1[0] = '0'+mutari/10;
        m1[1] = '0'+mutari%10;
        m1[2] = '\0';

        if(player==1)
        {
            // display the player's possible moves text
            possibleMovesTxt(player1, m1);
        }
        else
        {
            // display the player's possible moves text
            possibleMovesTxt(player2, m1);
        }

        if(mutari==0)
        {
            cleardevice();
            if(player==1)
            {
                // display the player has won text
                hasWonTxt(player2, player2Color);
            }
            else
            {
                // display the player has won text
                hasWonTxt(player1, player1Color);
            }
            startGame(xx, yy, zz, itsBg);
            goToMenu();

            do
            {
                if (ismouseclick(WM_LBUTTONDOWN) && checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
                {
                    isMenu=1;
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
            setcolor(WHITE);
            settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
            if (player==computer)
            {
                short z=1;
                short randNr = rand() % mutari + 1;
                delay(1500);
                mutare(computer, v[randNr].i1, v[randNr].j1, v[randNr].i2, v[randNr].j2, v[randNr].i3, v[randNr].j3);
                short neutralPiece = rand() % 3 + 1;
                for (short i=1; i<=4; i++)
                {
                    for (short j=1; j<=4; j++)
                    {
                        if (M[i][j] == '0')
                        {
                            liber[z].i=i;
                            liber[z].j=j;
                            z++;
                        }
                    }
                }
                z--;
                short randChoice = rand() % z + 1;
                if (neutralPiece == 1)
                {
                    M[liber[randChoice].i][liber[randChoice].j]='*';
                    M[stea.i][stea.j]='0';
                    stea.i=liber[randChoice].i;
                    stea.j=liber[randChoice].j;
                }
                else if (neutralPiece == 2)
                {
                    M[liber[randChoice].i][liber[randChoice].j]='#';
                    M[diez.i][diez.j]='0';
                    diez.i=liber[randChoice].i;
                    diez.j=liber[randChoice].j;
                }
            }
            else
            {
                // display the choose squares for new L text
                clickOnSquareTxt();

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
                                nr++;

                                if (nr == 1)
                                {
                                    i1=(mousey()-yy)/zz+1;
                                    j1=(mousex()-xx)/zz+1;

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

                                    if (i2!=i1 || j2!=j1)
                                    {
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
                                    else
                                    {
                                        nr--;
                                    }
                                }
                                else if (nr == 3)
                                {
                                    i3=(mousey()-yy)/zz+1;
                                    j3=(mousex()-xx)/zz+1;

                                    if ((i3!=i2 && i3!=i1) || (j3!=j2 && j3!=j1))
                                    {
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
                                    else
                                    {
                                        nr--;
                                    }
                                }
                            }

                            // go to menu button
                            else if (checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
                            {
                                isMenu=1;
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
                        break;
                    }
                    else
                    {
                        // display the can't create text
                        cantCreateTxt();

                        startGame(xx, yy, zz, itsBg);
                        goToMenu();
                    }
                }

                while(1)
                {
                    cleardevice();
                    startGame(xx, yy, zz, itsBg);
                    goToMenu();

                    // display the choose piece text
                    movePieceTxt();

                    clearmouseclick(WM_LBUTTONDOWN);
                    clearmouseclick(WM_RBUTTONDOWN);
                    do
                    {
                        if (ismouseclick(WM_LBUTTONDOWN))
                        {
                            if (checkClick(mousex(), mousey(), xx, xx+4*zz, yy, yy+4*zz))
                            {
                                in=(mousey()-yy)/zz+1;
                                jn=(mousex()-xx)/zz+1;

                                if (M[in][jn] == '*' || M[in][jn] == '#')
                                {
                                    drawSquare((in-1)*zz+1, (jn-1)*zz+1, xx, yy, zz, COLOR(151, 143, 2));
                                    setcolor(WHITE);
                                }

                                clearmouseclick(WM_LBUTTONDOWN);
                            }

                            // go to menu button
                            else if (checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
                            {
                                isMenu=1;
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
                            // display the choose square text
                            chooseSquareTxt();

                            do
                            {
                                if (ismouseclick(WM_LBUTTONDOWN))
                                {
                                    if (checkClick(mousex(), mousey(), xx, xx+4*zz, yy, yy+4*zz))
                                    {
                                        k1=(mousey()-yy)/zz+1;
                                        k2=(mousex()-xx)/zz+1;
                                        clearmouseclick(WM_LBUTTONDOWN);
                                    }

                                    // go to menu button
                                    else if (checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
                                    {
                                        isMenu=1;
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
                                // display the position not free text
                                posNotFreeTxt();

                                delay(2000);
                            }
                        }

                        else if(M[in][jn]=='#')
                        {
                            // display the choose new square text
                            chooseSquareTxt();

                            do
                            {
                                if (ismouseclick(WM_LBUTTONDOWN))
                                {
                                    if (checkClick(mousex(), mousey(), xx, xx+4*zz, yy, yy+4*zz))
                                    {
                                        k1=(mousey()-yy)/zz+1;
                                        k2=(mousex()-xx)/zz+1;
                                        clearmouseclick(WM_LBUTTONDOWN);
                                    }

                                    // go to menu button
                                    else if (checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
                                    {
                                        isMenu=1;
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
                                // display the position not free text
                                posNotFreeTxt();

                                delay(2000);
                            }
                        }
                    }
                    else
                        break;
                }
            }
        }
        player=3-player;
    }
}


void initMediumPvcGame()
{
    short xx=getmaxx()/10, yy=getmaxy()/10-20, zz=getmaxx()/14;
    short mutari;
    short in, jn, k1, k2;

    // display the game table;
    startConfig();
    startGame(xx, yy, zz, itsBg);

    setcolor(WHITE);

    // display the choose wanted player text
    wantedPlayerTxt();

    short player;

    while (1)
    {
        player=getch()-'0';
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
            cleardevice();
            startGame(xx, yy, zz, itsBg);

            // display the player doesn't exist text
            doesntExistTxt();
        }
    }
    cleardevice();

    // display the game table;
    startConfig();
    startGame(xx, yy, zz, itsBg);

    short computer=3-player;

    // display the choose the player who will move first text
    moveFirstTxt();

    while (1)
    {
        player=getch()-'0';
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
            cleardevice();
            startGame(xx, yy, zz, itsBg);

            // display the player doesn't exist text
            doesntExistTxt();
        }
    }

    // go to menu button
    goToMenu();

    while(1)
    {
        cleardevice();
        if(player==1)
        {
            // display the it's player's turn text
            playerTurnTxt(player1);
        }
        else
        {
            // display the it's player's turn text
            playerTurnTxt(player2);
        }
        startGame(xx, yy, zz, itsBg);
        goToMenu();

        if (computer==player)
        {
            mutari=mutariPosibileHardPvc(computer);
        }
        else
        {
            mutari=mutariposibile(player);
        }

        char m1[3];
        m1[0] = '0'+mutari/10;
        m1[1] = '0'+mutari%10;
        m1[2] = '\0';

        if(player==1)
        {
            // display the player moves text
            possibleMovesTxt(player1, m1);
        }
        else
        {
            // display the player moves text
            possibleMovesTxt(player2, m1);
        }

        if(mutari==0)
        {
            cleardevice();
            if(player==1)
            {
                // display the player has won text
                hasWonTxt(player2, player2Color);
            }
            else
            {
                // display the player has won text
                hasWonTxt(player1, player1Color);
            }
            startGame(xx, yy, zz, itsBg);
            goToMenu();

            do
            {
                if (ismouseclick(WM_LBUTTONDOWN) && checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
                {
                    isMenu=1;
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
            setcolor(WHITE);
            settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
            if (player==computer)
            {
                short z=1, i, j, minim=100, imin=1;
                for (i=1; i<=mutari; i++)
                {
                    if (v[i].pMoves < minim)
                    {
                        minim=v[i].pMoves;
                        imin=i;
                    }
                }
                delay(1500);
                mutare(computer, v[imin].i1, v[imin].j1, v[imin].i2, v[imin].j2, v[imin].i3, v[imin].j3);
                short neutralPiece = rand() % 3 + 1;
                for (i=1; i<=4; i++)
                {
                    for (j=1; j<=4; j++)
                    {
                        if (M[i][j] == '0')
                        {
                            liber[z].i=i;
                            liber[z].j=j;
                            z++;
                        }
                    }
                }
                z--;
                short randChoice = rand() % z + 1;
                if (neutralPiece == 1)
                {
                    M[liber[randChoice].i][liber[randChoice].j]='*';
                    M[stea.i][stea.j]='0';
                    stea.i=liber[randChoice].i;
                    stea.j=liber[randChoice].j;
                }
                else if (neutralPiece == 2)
                {
                    M[liber[randChoice].i][liber[randChoice].j]='#';
                    M[diez.i][diez.j]='0';
                    diez.i=liber[randChoice].i;
                    diez.j=liber[randChoice].j;
                }
            }
            else
            {
                // display the choose free squares for the new L text
                clickOnSquareTxt();

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
                                nr++;

                                if (nr == 1)
                                {
                                    i1=(mousey()-yy)/zz+1;
                                    j1=(mousex()-xx)/zz+1;

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

                                    if (i2!=i1 || j2!=j1)
                                    {
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
                                    else
                                    {
                                        nr--;
                                    }
                                }
                                else if (nr == 3)
                                {
                                    i3=(mousey()-yy)/zz+1;
                                    j3=(mousex()-xx)/zz+1;

                                    if ((i3!=i2 && i3!=i1) || (j3!=j2 && j3!=j1))
                                    {
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
                                    else
                                    {
                                        nr--;
                                    }
                                }
                            }

                            // go to menu button
                            else if (checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
                            {
                                isMenu=1;
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
                        break;
                    }
                    else
                    {
                        // display the can't create text
                        cantCreateTxt();

                        startGame(xx, yy, zz, itsBg);
                        goToMenu();
                    }
                }

                while(1)
                {
                    cleardevice();
                    startGame(xx, yy, zz, itsBg);
                    goToMenu();

                    // display the click on neutral piece text
                    movePieceTxt();

                    clearmouseclick(WM_LBUTTONDOWN);
                    clearmouseclick(WM_RBUTTONDOWN);
                    do
                    {
                        if (ismouseclick(WM_LBUTTONDOWN))
                        {
                            if (checkClick(mousex(), mousey(), xx, xx+4*zz, yy, yy+4*zz))
                            {
                                in=(mousey()-yy)/zz+1;
                                jn=(mousex()-xx)/zz+1;

                                if (M[in][jn] == '*' || M[in][jn] == '#')
                                {
                                    drawSquare((in-1)*zz+1, (jn-1)*zz+1, xx, yy, zz, COLOR(151, 143, 2));
                                    setcolor(WHITE);
                                }

                                clearmouseclick(WM_LBUTTONDOWN);
                            }

                            // go to menu button
                            else if (checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
                            {
                                isMenu=1;
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
                            // display the choose a new square for the neutral piece text
                            chooseSquareTxt();

                            do
                            {
                                if (ismouseclick(WM_LBUTTONDOWN))
                                {
                                    if (checkClick(mousex(), mousey(), xx, xx+4*zz, yy, yy+4*zz))
                                    {
                                        k1=(mousey()-yy)/zz+1;
                                        k2=(mousex()-xx)/zz+1;
                                        clearmouseclick(WM_LBUTTONDOWN);
                                    }

                                    // go to menu button
                                    else if (checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
                                    {
                                        isMenu=1;
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
                                // display the position not free text
                                posNotFreeTxt();

                                delay(2000);
                            }
                        }

                        else if(M[in][jn]=='#')
                        {
                            // display the choose a free square text
                            chooseSquareTxt();

                            do
                            {
                                if (ismouseclick(WM_LBUTTONDOWN))
                                {
                                    if (checkClick(mousex(), mousey(), xx, xx+4*zz, yy, yy+4*zz))
                                    {
                                        k1=(mousey()-yy)/zz+1;
                                        k2=(mousex()-xx)/zz+1;
                                        clearmouseclick(WM_LBUTTONDOWN);
                                    }

                                    // go to menu button
                                    else if (checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
                                    {
                                        isMenu=1;
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
                                // display the position not free text
                                posNotFreeTxt();

                                delay(2000);
                            }
                        }
                    }
                    else
                        break;
                }
            }
        }
        player=3-player;
    }
}

void initHardPvcGame()
{
    short xx=getmaxx()/10, yy=getmaxy()/10-20, zz=getmaxx()/14;
    short mutari;
    short in, jn, k1, k2;

    // display the game table;
    startConfig();
    startGame(xx, yy, zz, itsBg);

    // display the choose wanted player text
    wantedPlayerTxt();

    short player;

    while (1)
    {
        player=getch()-'0';
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
            cleardevice();
            startGame(xx, yy, zz, itsBg);

            // display the player doesn't exist text
            doesntExistTxt();
        }
    }
    cleardevice();

    // display the game table;
    startConfig();
    startGame(xx, yy, zz, itsBg);

    short computer=3-player;

    // display the choose the player who will move first text
    moveFirstTxt();

    while (1)
    {
        player=getch()-'0';
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
            cleardevice();
            startGame(xx, yy, zz, itsBg);

            // display the player doesn't exist text
            doesntExistTxt();
        }
    }

    // go to menu button
    goToMenu();

    while(1)
    {
        cleardevice();
        if(player==1)
        {
            // display the it's player's turn text
            playerTurnTxt(player1);
        }
        else
        {
            // display the it's player's turn text
            playerTurnTxt(player2);
        }
        startGame(xx, yy, zz, itsBg);
        goToMenu();

        if (computer==player)
        {
            mutari=mutariPosibileHardPvc(computer);
        }
        else
        {
            mutari=mutariposibile(player);
        }

        char m1[3];
        m1[0] = '0'+mutari/10;
        m1[1] = '0'+mutari%10;
        m1[2] = '\0';

        if(player==1)
        {
            // display the player moves text
            possibleMovesTxt(player1, m1);
        }
        else
        {
            // display the player moves text
            possibleMovesTxt(player2, m1);
        }

        if(mutari==0)
        {
            cleardevice();
            if(player==1)
            {
                // display the player has won text
                hasWonTxt(player2, player2Color);
            }
            else
            {
                // display the player has won text
                hasWonTxt(player1, player1Color);
            }
            startGame(xx, yy, zz, itsBg);
            goToMenu();

            do
            {
                if (ismouseclick(WM_LBUTTONDOWN) && checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
                {
                    isMenu=1;
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
            setcolor(WHITE);
            settextstyle(COMPLEX_FONT, HORIZ_DIR, fsize);
            if (player==computer)
            {
                short z=0, i, j, minim=100, imin=1;
                for (i=1; i<=mutari; i++)
                {
                    if (v[i].pMoves < minim)
                    {
                        minim=v[i].pMoves;
                        imin=i;
                    }
                }
                delay(1500);
                mutare(computer, v[imin].i1, v[imin].j1, v[imin].i2, v[imin].j2, v[imin].i3, v[imin].j3);
                short nmin=v[imin].pMoves;
                for (i=1; i<=4; i++)
                {
                    for (j=1; j<=4; j++)
                    {
                        if (M[i][j] == '0')
                        {
                            z++;
                            liberStea[z].i=i;
                            liberStea[z].j=j;
                            liberStea[z].pMoves=mPosNeutPiece(3-computer, i, j, '*');
                        }
                    }
                }
                z=0;
                for (i=1; i<=4; i++)
                {
                    for (j=1; j<=4; j++)
                    {
                        if (M[i][j] == '0')
                        {
                            z++;
                            liberDiez[z].i=i;
                            liberDiez[z].j=j;
                            liberDiez[z].pMoves=mPosNeutPiece(3-computer, i, j, '#');
                        }
                    }
                }
                imin=-1;
                short neutralPiece=3;
                for (i=1; i<=6; i++)
                {
                    if (liberStea[i].pMoves < nmin)
                    {
                        nmin=liberStea[i].pMoves;
                        imin=i;
                        neutralPiece=1;
                    }
                }
                for (i=1; i<=6; i++)
                {
                    if (liberDiez[i].pMoves < nmin)
                    {
                        nmin=liberDiez[i].pMoves;
                        imin=i;
                        neutralPiece=2;
                    }
                }
                if (neutralPiece == 1)
                {
                    M[liberStea[imin].i][liberStea[imin].j]='*';
                    M[stea.i][stea.j]='0';
                    stea.i=liberStea[imin].i;
                    stea.j=liberStea[imin].j;
                }
                else if (neutralPiece == 2)
                {
                    M[liberDiez[imin].i][liberDiez[imin].j]='#';
                    M[diez.i][diez.j]='0';
                    diez.i=liberDiez[imin].i;
                    diez.j=liberDiez[imin].j;
                }
            }
            else
            {
                // display the choose L squares text
                clickOnSquareTxt();

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
                                nr++;

                                if (nr == 1)
                                {
                                    i1=(mousey()-yy)/zz+1;
                                    j1=(mousex()-xx)/zz+1;

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

                                    if (i2!=i1 || j2!=j1)
                                    {
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
                                    else
                                    {
                                        nr--;
                                    }
                                }
                                else if (nr == 3)
                                {
                                    i3=(mousey()-yy)/zz+1;
                                    j3=(mousex()-xx)/zz+1;

                                    if ((i3!=i2 && i3!=i1) || (j3!=j2 && j3!=j1))
                                    {
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
                                    else
                                    {
                                        nr--;
                                    }
                                }
                            }

                            // go to menu button
                            else if (checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
                            {
                                isMenu=1;
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
                        break;
                    }
                    else
                    {
                        // display the can't create text
                        cantCreateTxt();

                        startGame(xx, yy, zz, itsBg);
                        goToMenu();
                    }
                }

                while(1)
                {
                    cleardevice();
                    startGame(xx, yy, zz, itsBg);
                    goToMenu();

                    // display the move piece text
                    movePieceTxt();

                    clearmouseclick(WM_LBUTTONDOWN);
                    clearmouseclick(WM_RBUTTONDOWN);
                    do
                    {
                        if (ismouseclick(WM_LBUTTONDOWN))
                        {
                            if (checkClick(mousex(), mousey(), xx, xx+4*zz, yy, yy+4*zz))
                            {
                                in=(mousey()-yy)/zz+1;
                                jn=(mousex()-xx)/zz+1;

                                if (M[in][jn] == '*' || M[in][jn] == '#')
                                {
                                    drawSquare((in-1)*zz+1, (jn-1)*zz+1, xx, yy, zz, COLOR(151, 143, 2));
                                    setcolor(WHITE);
                                }

                                clearmouseclick(WM_LBUTTONDOWN);
                            }

                            // go to menu button
                            else if (checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
                            {
                                isMenu=1;
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

                    char newCoordTxt[75], notFreeTxt[75];

                    if (in!=-1)
                    {
                        if(M[in][jn]=='*')
                        {
                            // display the choose free square text
                            chooseSquareTxt();

                            do
                            {
                                if (ismouseclick(WM_LBUTTONDOWN))
                                {
                                    if (checkClick(mousex(), mousey(), xx, xx+4*zz, yy, yy+4*zz))
                                    {
                                        k1=(mousey()-yy)/zz+1;
                                        k2=(mousex()-xx)/zz+1;
                                        clearmouseclick(WM_LBUTTONDOWN);
                                    }

                                    // go to menu button
                                    else if (checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
                                    {
                                        isMenu=1;
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
                                // display the position not free text
                                posNotFreeTxt();

                                delay(2000);
                            }
                        }

                        else if(M[in][jn]=='#')
                        {
                            // display the choose free square text
                            chooseSquareTxt();

                            do
                            {
                                if (ismouseclick(WM_LBUTTONDOWN))
                                {
                                    if (checkClick(mousex(), mousey(), xx, xx+4*zz, yy, yy+4*zz))
                                    {
                                        k1=(mousey()-yy)/zz+1;
                                        k2=(mousex()-xx)/zz+1;
                                        clearmouseclick(WM_LBUTTONDOWN);
                                    }

                                    // go to menu button
                                    else if (checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
                                    {
                                        isMenu=1;
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
                                // display the position not free text
                                posNotFreeTxt();

                                delay(2000);
                            }
                        }
                    }
                    else
                        break;
                }
            }
        }
        player=3-player;
    }
}

void initOptions()
{
    char gameOptionsTxt[20], gameLangTxt[20], gameSongTxt[20], pOneTxt[20], pTwoTxt[20];
    // the title
    setcolor(itsYellow);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 5);
    if (lang == 1)
    {
        strcpy(gameOptionsTxt, "Game Options");
    }
    else if (lang == 2)
    {
        strcpy(gameOptionsTxt, "Optiuni Joc");
    }
    else if (lang == 3)
    {
        strcpy(gameOptionsTxt, "Options de Jeu");
    }
    outtextxy(100, 100, gameOptionsTxt);

    // options themselves
    setcolor(WHITE);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 4);

    // change language
    if (lang == 1)
    {
        strcpy(gameLangTxt, "Language:");
    }
    else if (lang == 2)
    {
        strcpy(gameLangTxt, "Limba:");
    }
    else if (lang == 3)
    {
        strcpy(gameLangTxt, "Langue:");
    }
    outtextxy(120, 200, gameLangTxt);

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
    if (lang == 3)
    {
        setcolor(WHITE);
    }
    else
    {
        setcolor(itsYellow);
    }
    char frText[] = "FR";
    outtextxy(580, 200, frText);

    // start/stop music
    setcolor(WHITE);

    // choose song
    if (lang == 1)
    {
        strcpy(gameSongTxt, "Song:");
    }
    else if (lang == 2)
    {
        strcpy(gameSongTxt, "Cantec:");
    }
    else if (lang == 3)
    {
        strcpy(gameSongTxt, "Chanson:");
    }
    outtextxy(120, 260, gameSongTxt);

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
        strcpy(pOneTxt, "Player 1:");
    }
    else if (lang == 2)
    {
        strcpy(pOneTxt, "Jucator 1:");
    }
    else if (lang == 3)
    {
        strcpy(pOneTxt, "Joueur 1:");
    }
    outtextxy(120, 320, pOneTxt);

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

        strcpy(pTwoTxt, "Player 2:");
    }
    else if (lang == 2)
    {
        strcpy(pTwoTxt, "Jucator 2:");
    }
    else if (lang == 3)
    {
        strcpy(pTwoTxt, "Joueur 2:");
    }
    outtextxy(120, 380, pTwoTxt);

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

    // go to menu button
    goToMenu();
}

void initInfo()
{
    char aboutTxt[20];
    // the title
    setcolor(itsYellow);
    settextstyle(COMPLEX_FONT, HORIZ_DIR, 5);
    if (lang == 1)
    {
        strcpy(aboutTxt, "About Bonol");
    }
    else if (lang == 2)
    {
        strcpy(aboutTxt, "Despre Bonol");
    }
    else if (lang == 3)
    {
        strcpy(aboutTxt, "À propos de Bonol");
    }
    outtextxy(100, 100, aboutTxt);

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
    else if (lang == 3)
    {
        char info1Fr[] = "   Le jeu L est un jeu a deux joueurs joue sur une planche de carres 4x4. Chaque joueur a une piece";
        char info2Fr[] = "en forme de L 3x2, et il y a deux pièces neutres 1x1  (carrés jaunes dans le diagramme).";
        char info3Fr[] = "   A chaque tour, un joueur doit d'abord déplacer sa pièce en L, puis peut éventuellement déplacer";
        char info4Fr[] = "l'une des pièces neutres. Le jeu est gagné en laissant l'adversaire incapable de déplacer sa pièce";
        char info5Fr[] = "L vers une nouvelle position.";
        char info6Fr[] = "   Les pièces ne doivent pas se chevaucher ou couvrir d'autres pièces. En déplaçant la pièce en L,";
        char info7Fr[] = "elle est ramassée puis placée dans des cases vides n'importe où sur le plateau. Il peut être tourné";
        char info8Fr[] = "ou même retourné ce faisant. La seule règle est qu'elle doit se terminer dans une position différente";
        char info9Fr[] = "de celle où elle a commencé; couvrant ainsi au moins un carré, il ne couvrait pas auparavant. Pour";
        char info10Fr[] = "déplacer une pièce neutre, un joueur la prend simplement, puis le place sur une case vide n'importe";
        char info11Fr[] = "où sur le plateau.";

        outtextxy(120, 200, info1Fr);
        outtextxy(120, 225, info2Fr);
        outtextxy(120, 275, info3Fr);
        outtextxy(120, 300, info4Fr);
        outtextxy(120, 350, info5Fr);
        outtextxy(120, 375, info6Fr);
        outtextxy(120, 400, info7Fr);
        outtextxy(120, 425, info8Fr);
        outtextxy(120, 450, info9Fr);
        outtextxy(120, 475, info10Fr);
        outtextxy(120, 500, info11Fr);
    }

    // go to menu button
    goToMenu();
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
            isChoosing=1;
            cleardevice();
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
        if (ismouseclick(WM_LBUTTONDOWN) && checkClick(mousex(), mousey(), getmaxx()/10, getmaxx()/10+310, getmaxy()/2+10, getmaxy()/2+50))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            isMenu=0;
            isInfo=1;
            cleardevice();
            showInfo();
        }

        // if player clicks the EXIT button
        if (ismouseclick(WM_LBUTTONDOWN) && checkClick(mousex(), mousey(), getmaxx()/10, getmaxx()/10+232, getmaxy()/2+80, getmaxy()/2+120))
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
                cleardevice();
                initPvpGame();
            }

            // start PVC
            if (checkClick(mousex(), mousey(), getmaxx()/11, getmaxx()/11+470, getmaxy()/2+50, getmaxy()/2+92))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                isChoosing=0;
                isChoosingDifficulty=1;
                cleardevice();
                showGameDiffChoosing();
            }

            // go to menu
            if (checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                isMenu=1;
                isChoosing=0;
                cleardevice();
                showMenu();
            }
        }

        // prevent some click bugs
        clearmouseclick(WM_LBUTTONDOWN);
        delay(5);
    }
    while (isChoosing == 1);
}

void showGameDiffChoosing()
{
    initGameDifficulty();

    do
    {
        initDifficultyButtons();
        setbkcolor(itsBg);

        if (ismouseclick(WM_LBUTTONDOWN))
        {
            // easy button
            if (checkClick(mousex(), mousey(), getmaxx()/2+60, getmaxx()/2+195, getmaxy()/2-60, getmaxy()/2-8))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                isChoosingDifficulty=0;
                cleardevice();
                initEasyPvcGame();
            }

            // medium button
            if (checkClick(mousex(), mousey(), getmaxx()/2+60, getmaxx()/2+260, getmaxy()/2+20, getmaxy()/2+72))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                isChoosingDifficulty=0;
                cleardevice();
                initMediumPvcGame();
            }

            // hard button
            if (checkClick(mousex(), mousey(), getmaxx()/2+60, getmaxx()/2+195, getmaxy()/2+100, getmaxy()/2+152))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                isChoosingDifficulty=0;
                cleardevice();
                initHardPvcGame();
            }

            // go to menu
            if (checkClick(mousex(), mousey(), getmaxx()-150, getmaxx()-50, getmaxy()-80, getmaxy()-60))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                isMenu=1;
                isChoosingDifficulty=0;
                cleardevice();
                showMenu();
            }
        }
        // prevent some click bugs
        clearmouseclick(WM_LBUTTONDOWN);
        delay(5);
    }
    while (isChoosingDifficulty == 1);
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

                cleardevice();
                showOptions();
            }

            if (checkClick(mousex(), mousey(), 580, 620, 205, 224))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                lang=3;

                // change players' names
                if (active1Color == 1)
                    strcpy(player1, "Rouge");
                else if (active1Color == 2)
                    strcpy(player1, "Orange Sombre");
                else if (active1Color == 3)
                    strcpy(player1, "Orange");
                else if (active1Color == 4)
                    strcpy(player1, "Violet");
                else if (active1Color == 5)
                    strcpy(player1, "Rose");

                if (active2Color == 1)
                    strcpy(player2, "Bleu");
                else if (active2Color == 2)
                    strcpy(player2, "Bleu Froid");
                else if (active2Color == 3)
                    strcpy(player2, "Aqua");
                else if (active2Color == 4)
                    strcpy(player2, "Vert");
                else if (active2Color == 5)
                    strcpy(player2, "Verdure");

                cleardevice();
                showOptions();
            }

            // change song
            if (checkClick(mousex(), mousey(), 460, 480, 268, 284))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                if (songNr > 1 && songNr <= 3)
                {
                    songNr-=1;
                }
                if (songNr == 1)
                {
                    strcpy(songName, "Miyagi - Badabum");
                    PlaySound(TEXT("songs/miyagi_badabum.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
                }
                else if (songNr == 2)
                {
                    strcpy(songName, "Post Malone - Better Now");
                    PlaySound(TEXT("songs/post_malone_better_now.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
                }
                else if (songNr == 3)
                {
                    strcpy(songName, "Clandestina - Cocaina");
                    PlaySound(TEXT("songs/clandestina_cocaina.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
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
                if (songNr == 1)
                {
                    strcpy(songName, "MiyaGi - Badabum");
                    PlaySound(TEXT("songs/miyagi_badabum.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
                }
                else if (songNr == 2)
                {
                    strcpy(songName, "Post Malone - Better Now");
                    PlaySound(TEXT("songs/post_malone_better_now.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
                }
                else if (songNr == 3)
                {
                    strcpy(songName, "Clandestina - Cocaina");
                    PlaySound(TEXT("songs/clandestina_cocaina.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
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
                else if (lang == 3)
                {
                    strcpy(player1, "Rouge");
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
                else if (lang == 3)
                {
                    strcpy(player1, "Orange Sombre");
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
                if (lang == 1 || lang == 3)
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
                else if (lang == 3)
                {
                    strcpy(player1, "Rose");
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
                else if (lang == 3)
                {
                    strcpy(player2, "Bleu");
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
                else if (lang == 3)
                {
                    strcpy(player2, "Bleu Froid");
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
                else if (lang == 3)
                {
                    strcpy(player2, "Vert");
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
                if (lang == 1 || lang == 2)
                {
                    strcpy(player2, "Greenery");
                }
                else if (lang == 3)
                {
                    strcpy(player2, "Verdure");
                }
                player2Color=COLOR(162, 193, 113);
                player2ClickColor=COLOR(111, 142, 62);
                cleardevice();
                showOptions();
            }

            // go to menu button
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
        // go to menu button
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
        PlaySound(TEXT("songs/miyagi_badabum.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
    }
    initwindow(screenWidth, screenHeight, "BONOL", -3, -3);
    setbkcolor(itsBg);
    cleardevice();

    if (getmaxx() >= 1600)
        fsize=3;
    else
        fsize=2;

    showMenu();

    getch();
    return 0;
}
