#include <iostream>
#include <string.h>
#include "winbgim.h"
#include <ctime>
#include <MMsystem.h>
#include <fstream>
#define MAX 11



/// variabilele necesare pentru joc
int latura=100;///latura unui patrat de pe tabla de joc, devine 75 cand tabla este mai mare de 6x6
int l=-1,c=-1;///numarul de linii si de coloane pentru desenara tablei de joc
int la=-1;/// linia pe care se afla jetonul anterior pus
int ca=-1;/// coloana pe care se aflat jetonul anterior pus
int lr=-1; ///
int cr=-1; ///
int cant=-1; ///
int lant=-1; ///
int player; /// playerul care pune pe tabla jetonul intr un anumit moment
int vsplayer=-1; /// variabila pentru alegerea modului de joc
int vsbot=-1; /// variabila pentru alegerea modului de joc
int col=-1, lin=-1; ///liniile si coloanele pentru verificaredrum, actualizare drum, castigpriniesire si castigincolt
int tema=-1; ///variabila pentru alegerea temei de joc
int ok=0; ///contor folosit la mai multe functii
bool gataselectie=0; /// folosita la selectia unui jeton care urmeaza a fi pus pe tabla
int cont=0;///contorul folosit pentru jetoane
int i=0;
int winner;///castigatorul unei runde, folosit pentru leaderboard
int player1=0;///numar de runde castigare ale primului jucator
int player2=0;///numar de runde castigare ale celui de al doilea jucator
int rundadejoc=1;
bool sound=0;

///structuri
struct tablajoc ///pe tabla de joc putem avea jetoane, motiv pentru care folosim o variabila booleana
{
    int jeton;
    int esteJeton;
} tabla[MAX][MAX]; ///tabla este o matrice pe nr de laturi x nr de coloana

struct IO ///pentru a putea construii drumul ne folosim de niste "iesiri" care determina pe unde continua drumul
{
    bool left;
    bool right;
    bool down;
    bool up;

} jetAnt; ///aceste iesiri sunt ale jetonului anterior pus

struct HighScores
{
    int jucator;
    int scor;
} v[3];

///functii necesare
void butoaneMeniu();//butoane meniu 1
void backbutton(int x,int y); //buton de back care duce playerul inapoi la meniul principal
void butonStart(int x,int y); //buton care duce playerul la meniul 2 unde isi alege stilul de joc dorit
void butonClose(int x,int y); //buton care inchide jocul
void butonSet(int x,int y); //buton care duce playerul la reguli
void butonrestartjoc(int x,int y); //buton care restarteaza jocul
void muzica(int x,int y);//pentru sunet
void meniu(); //meniul principal
void reguli(); //reguli
void meniu2(); //al doilea meniu
void meniu2Butoane();//butoane meniu 2
void buton4x4(int x,int y);//butoane alegere marime tabla, tabla va fi de 4x4
void buton5x5(int x,int y); // tabla va fi de 5x5
void buton6x6(int x,int y); // tabla va fi de 6x6
void buton7x7(int x,int y); // tabla va fi de 7x7
void buton8x8(int x,int y); // tabla va fi de 8x8
void buton9x9(int x,int y); // tabla va fi de 9x9
void butontema1(int x, int y);//buton pentru prima tema, t==1
void butontema2(int x, int y); //buton pentru a doua tema, t==2
void butonvsplayer(int x,int y);//buton ce va face jocul de 2 persoane
void butonvsbot(int x,int y); //buton  ce va face jocul de o singura persoana
void initializaretablaJoc();//initializare matrice goala
void drawTabla();///deseneaza tabla de joc
void drawboard();//desenam un tabel
void restartgame(int x,int y);//restarteaza jocul
void MeniuJetoane();//locatia pe ecran a jetoanelor
void playing();//loop-ul necesar pentru joc
void AlegereJeton();///alegere jeton din partea de sus a ferestrei
void selectiepatrat(int x,int y,int cont);//alegere locatie pe tabla
void playerMove(int lin,int col);//care player joaca in ce moment
bool verificareDrum(int &lc,int &cc,int cont);//verificare daca locatia in care se incearca a se pune jetonul este corecta
void actualizareDrum(int &lin,int &col,int cont);//actualizare in cazul in care se aflat deja jetoane pe tabla care se leaga cu jetonul abia pus
bool castigColt(int col, int lin);//verificare daca sa ajuns la coltul negru
bool castigarepriniesire(int lin,int col);//verificare daca sa iesit din tabla
void butonrestartrunda();
void butoncloseb(int x,int y);//buton pentru inchiderea jocului dupa 5 runde de joc
void butoaneWinWindow();//butoanele de pe fereastra de win finala
bool inMatrice(int x, int y);///verificare daca clickul este inaintrul tablei de joc
void randomMove();//miscare bot random
void bordingTable(); // "ingradirea" tablei de joc ca sa nu se poata bot-ul sa puna jeton inafara ei
void returnFreeSpaces(int &jetRand); //functie ce returneaza spatiile libere pe care poate sa puna botul
void AlegereJetonRandom(int &jetRand);//alege un jeton pentru bot urmarind regula de a nu iesi din tabla de joc pe cat posibil
void punereJeton(int cont); //functie care pune jetonul pe tabla pentru bot
void leaderBoard();//retinerea rundelor castigate in timpul in care jocul ruleaza

using namespace std;

ofstream fout("file.out");

int main()
{
    fout<<"               Locul  Jucatorul  Scor"<<endl;
    fout<<"-----------------------------------------------"<<endl;
    initwindow(1000,1000,"Coltul Negru");
    meniu();
    bool jocactiv=1;
    do
    {
        playing();
    }
    while(!jocactiv);
    getch();
    closegraph();

    return 0;
}

void meniu() //desenam meniul principal cu butoanele sale
{
    readimagefile("bg1.gif",0,0,1000,1000);//imaginea de fundal
    readimagefile("title.jpg",250,100,750,200);//titlul jocului
    setfillstyle(SOLID_FILL,WHITE);
    setcolor(WHITE);
    bar(390,290,610,410);
    bar(340,440,660,560);
    bar(340,590,660,710);
    readimagefile("playbuttonb.jpg",400,300,600,400);//buton de joc
    readimagefile("howtobuttonb.jpg",350,450,650,550);//buton reguli
    readimagefile("closebuttonb.jpg",350,600,650,700);//buton close
    setcolor(COLOR(90,22,10));
    setlinestyle(DOTTED_LINE,USERBIT_LINE,THICK_WIDTH);
    rectangle(400,300,600,400);
    rectangle(350,450,650,550);
    rectangle(350,600,650,700);
    setlinestyle(SOLID_LINE,USERBIT_LINE,NORM_WIDTH);
    rectangle(800,560,900,660);
    setfillstyle(SOLID_FILL,5);
    setcolor(5);
    bar(800,560,900,660);
    readimagefile("soundoff.gif",800,560,900,660);
    butoaneMeniu();
    getch();
}

void muzica(int x,int y)// jocul incepe fara muzica, astfel incat daca se apasa in cea zona, in functie devariabla sound,va da drumul la muzica sau o va inchide
{
    if(x>=800 && x<=900 && y>=560 && y<=660 && sound==0)
    {
        PlaySound("Lucky One (Instrumental).wav",NULL,SND_FILENAME|SND_LOOP|SND_ASYNC);
        bar(800,560,900,660);
        readimagefile("soundon.gif",800,560,900,660);
        sound=1;
    }
    else if(x>=800 && x<=900 && y>=560 && y<=660 && sound==1)
    {
        PlaySound(NULL,NULL,0);
        bar(800,560,900,660);
        readimagefile("soundoff.gif",800,560,900,660);
        sound=0;
    }
}

void reguli() // window pentru regurile jocului, contine si buton de back
{
    cleardevice();
    readimagefile("reguli.jpg",0,0,1000,1000);
    int x1,y1;
    bool click=false;
    do
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN,x1,y1);
            backbutton(x1,y1);
        }
    }
    while(!click);
    click=false;

}

void butonClose(int x,int y) //inchide jocul
{
    if(x>=350 && x<=650 && y>=600 && y<=700)
    {
        closegraph();
    }
}

void butonSet(int x,int y) // buton ce contine coordonatele pentru butonul care daca e apasat duce la reguli
{

    if(x>=350 && x<=650 && y>=480 && y<=560)
    {
        cleardevice();
        reguli();
    }
}

void butonStart(int x,int y) // buton ce contine coordonatele pentru butonul care daca e apasat duce la urmatorul meniu
{

    if(x>=400 && x<=600 && y>=300 && y<=400)
    {
        cleardevice();
        meniu2();
    }
}

void butoaneMeniu() // functia care contiine toate coordonatele butoanelor si construieste clickuirle in sine
{
    int x,y;
    bool click=false;
    do
    {

        if(ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN,x,y);
            butonStart(x,y);
            butonSet(x,y);
            butonClose(x,y);
            muzica(x,y);
        }

    }
    while(!click);
    click=false;
}

void backbutton(int x,int y) //buton care duce playerul inapot la meniul principal si reface jocul
{
    if(x>=0 && x<=50 && y>=0 && y<=90)
    {
        cleardevice();
        meniu();
        bool jocactiv=1;
        do
        {
            playing();
        }
        while(!jocactiv);
    }

}

void meniu2() // meniul doi unde jucatorul poate alege daca vrea sa joace vsbot sau vsplayer, cat de mare sa fie tabla de joc, si tema de pe fundal
{
    cleardevice();
    l=-1; //in momentul in care este deschis meniul, variabilele sunt resetate astfel incat sa se aleaga din nou
    c=-1; //nr linii si coloana
    vsbot=-1; // pentru joc vsbot
    vsplayer=-1; //pt joc vsplayer
    tema=-1; //pentru tema de fundal
    rundadejoc=1;
    latura=100;
    player1=0;
    player2=0;
    readimagefile("meniu2.jpg",0,0,1000,1000); //imaginea de fundal
    setcolor(7);
    rectangle(95,195,530,250);//vsplayer
    rectangle(600,185,885,250);//vsbot
    rectangle(100,435,220,495);//buton 4x4
    rectangle(240,435,350,495);///buton 5x5
    rectangle(375,435,485,495);///buton 6x6
    rectangle(500,435,610,495);///buton 7x7
    rectangle(620,435,740,495);///buton 8x8
    rectangle(752,435,878,495);///buton 9x9
    rectangle(315,635,415,735);///tema 1
    rectangle(605,635,705,735);///tema 2
    rectangle(370,770,670,870);///buton joc
    setcolor(1);
    meniu2Butoane(); //butoanele meniului
}

void butonvsplayer(int x,int y) //buton ce actualizeaza variabilele astfel incat vsplayer sa fie 1
{
    if(x>=95 && x<=530 && y>=195 && y<=250)
    {
        vsplayer=1;
        vsbot=-1;
        cout<<"vsplayer"<<vsplayer<<endl;
    }
}

void butonvsbot(int x,int y) //buton ce actualizeaza variabilele astfel incat vsbot sa fie 1
{
    if(x>=600 && x<=885 && y>=195 && y<=250)
    {
        vsbot=1;
        vsplayer=-1;
        cout<<"vsbot "<<vsbot<<endl;
    }

}

void buton4x4(int x, int y) //tabla va fi de 4x4
{
    if(x>=100 && x<=220 && y>=435 && y<=495)
    {
        c=4;
        l=4;
        cout<<l<<" "<<c<<endl;
    }

}
void buton5x5(int x,int y) //tabla va fi de 5x5
{
    if(x>=240 && x<=350 && y>=435 && y<=495)
    {
        l=5;
        c=5;
        cout<<l<<" "<<c<<endl;
    }
}
void buton6x6(int x,int y) //tabla va fi de 6x6
{
    if(x>=375 && x<=485 && y>=435 && y<=495)
    {
        l=6;
        c=6;
        cout<<l<<" "<<c<<endl;
    }
}
void buton7x7(int x,int y) //tabla va fi de 7x7
{
    if(x>=500 && x<=610 && y>=435 && y<=495)
    {
        l=7;
        c=7;
        cout<<l<<" "<<c<<endl;
    }
}
void buton8x8(int x,int y) //tabla va fi de 8x8
{
    if(x>=620 && x<=740 && y>=435 && y<=495)
    {
        l=8;
        c=8;
        cout<<l<<" "<<c<<endl;
    }
}
void buton9x9(int x,int y) //tabla va fi de 9x9
{
    if(x>=752 && x<=878 && y>=435 && y<=495)
    {
        l=9;
        c=9;
        cout<<l<<" "<<c<<endl;
    }
}

void butontema1(int x, int y) //tema va fi 1
{
    if(x>=315 && x<=415 && y>=635 && y<=735)
    {
        tema=1;
        cout<<tema<<endl;
    }

}

void butontema2(int x, int y) //tema va fi 2
{
    if(x>=605 && x<=705 && y>=635 && y<=735)
    {
        tema=2;
        cout<<tema<<endl;
    }

}

void butonStartjoc(int x,int y) // daca este apasat click in aceste coordonate se va incepe jocul
{
    if(x>=370 && x<=670 && y>=770 && y<=870 && l>=4 && c>=4 && tema!=-1 && (vsbot!=-1 || vsplayer!=-1))
    {
        cleardevice();
        playing();
    }
}

void meniu2Butoane() //butoanele meniului 2
{
    int x1,y1;
    bool click=false;
    do
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN,x1,y1);

            butonvsbot(x1,y1);
            butonvsplayer(x1,y1);
            buton4x4(x1,y1);
            buton5x5(x1,y1);
            buton6x6(x1,y1);
            buton7x7(x1,y1);
            buton8x8(x1,y1);
            buton9x9(x1,y1);
            butontema1(x1,y1);
            butontema2(x1,y1);
            butonStartjoc(x1,y1);
            backbutton(x1,y1);
        }
    }
    while(!click);
    click=false;

}

void butoaneWinWindow()
{
    int x,y;
    bool click=false;
    do
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN,x,y);
            click=1;
            butonrestartjoc(x,y);
            butoncloseb(x,y);
        }
    }
    while(!click);
    click=0;
}

void butonrestartjoc(int x,int y) //butonul de restart
{
    if(x>=865 && x<=965 && y>=100 && y<=150)
        rundadejoc=1;
    {
        cleardevice();
        meniu();
        bool jocactiv=1;
        do
        {
            playing();
        }
        while(!jocactiv);
    }
}

void butoncloseb(int x,int y)
{
    if(x>=755 && x<=855 && y>=100 && y<=150)
    {
        closegraph();
    }
}

void initializaretablaJoc() //initializarea matricei goale(0 peste tot, nu se afla jetoane pe ea)
{
    tabla[MAX][MAX];
    int i,j;
    for(i=1; i<=9; i++)
        for(j=1; j<=9; j++)
        {
            tabla[i][j].esteJeton=0;
            tabla[i][j].jeton=0;
        }
    for(i=1; i<=c; i++)
    {
        tabla[-1][i].esteJeton=2;
        tabla[i][-1].esteJeton=2;
        tabla[c+1][i].esteJeton=2;
        tabla[i][c+1].esteJeton=2;
    }
    /// iesirile jetoanelor anterioare sunt 0 deoarece nu exista jeton anterior
    jetAnt.down=0;
    jetAnt.up=0;
    jetAnt.right=0;
    jetAnt.left=0;

}

void butonrestartrunda()
{
    int x,y;
    bool click=false;
    do
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN,x,y);

            if(x>=800 && x<=900 && y>=650 && y<=690)
            {
                click=1;
                cleardevice();
                rundadejoc+=1;
                bool jocactiv=1;
                do
                {
                    playing();
                }
                while(!jocactiv);

            }
        }

    }
    while(!click);
}
void playing() // functia care construieste loop-ul necesar ca jocul sa continue
{

    la=-1; //variabilele sunt reactualizate
    ca=-1;
    lr=-1;
    cr=-1;
    col=-1, lin=-1;
    ok=-1;
    cout<<"runda "<<rundadejoc<<endl;
    initializaretablaJoc(); //este initilizata matricea
    drawTabla(); //este desenata fereastra si tabla de joc
    MeniuJetoane(); //este desenat meniul jetoanelor de unde se alege jetonul dorit
    do
    {
        AlegereJeton();
    }
    while(castigColt(col,lin)==0 && castigColt(cr,lr)==0 && castigarepriniesire(lin,col)==0 && castigarepriniesire(lr,cr)==0 && rundadejoc<=6); //jocul continua cat timp nu se ajunge la coltul negru/marginea tablei de joc
    if((castigColt(col,lin)==1 || castigColt(cr,lr)==1) && rundadejoc!=6)
    {
        bar(750,500,950,700);
        setcolor(5);
        rectangle(800,650,900,690);
        if(player==1)
        {
            outtextxy(800,550,"A castigat jucatorul 1!");
            player1++;
        }

        if(player==2)
        {
            outtextxy(800,550,"A castigat jucatorul 2!");
            player2++;
        }
        winner=player;
        leaderBoard();
        butonrestartrunda();

    }
    if((castigarepriniesire(lin,col)==1 || castigarepriniesire(lr,cr)==1) && rundadejoc!=6)
    {
        bar(750,500,950,700);
        setcolor(5);
        rectangle(800,650,900,690);
        if(player==2)
        {
            player1++;
            outtextxy(800,550,"A castigat jucatorul 1!");
        }
        if(player==1)
        {
            player2++;
            outtextxy(800,550,"A castigat jucatorul 2!");
        }
        winner=3-player;
        leaderBoard();
        butonrestartrunda();

    }

    if((castigColt(col,lin)==1 || castigColt(cr,lr)==1) &&rundadejoc==6 ) //daca s-au jucat cele 5 runde si sa catigat ajungand cu drumul la contul negru atunci va fi deschia o alta fereastra
    {
        if(player==1) //in cazul in care playerul care a pus ultimul este 1, atunci el castiga
        {
            winner=player;
            leaderBoard();
        }
        if(player==2) //in cazul in care playerul care a pus ultimul este 2, atunci el castiga
        {
            winner=player;
            leaderBoard();
        }

        readimagefile("gameover.gif",750,50,975,150);
        if(player1>player2)
            outtextxy(760,60,"Player 1 a castigat");
        else
            outtextxy(760,60,"Player 2 a castigat");
        bar(755,100,855,150);
        bar(865,100,965,150);
        outtextxy(880,125,"Restart?");
        outtextxy(770,125,"Inchide?");
        butoaneWinWindow();
        cout<<"Player "<<player<<" a castigat.";
        butoaneWinWindow(); //in terminal apare ce player a castigat
    }
    //daca s-au jucat cele 6 runde si sa catigat ajungand cu drumul la marginea tablei atunci va aparea o noua "zona" pe ecran de unde putem restarta jocul complet sau putem sa iesim din joc
    else if ((castigarepriniesire(lin,col)==1 || castigarepriniesire(lr,cr)==1) && rundadejoc==6)
    {

        if(player==1) //in cazul in care playerul care a pus ultimul este 1, atunci a castigat player 2
        {
            winner=3-player;
            leaderBoard();
        }
        cout<<"Jucatorul 2 a castigat.";
        if(player==2) //in cazul in care playerul care a pus ultimul este 2, atunci a castigat playerul 1
        {
            winner=3-player;
            leaderBoard();
        }
        cout<<"Player 1 a castigat.";
        readimagefile("gameover.gif",750,50,975,150);
        if(player1>player2)
            outtextxy(760,60,"Player 1 a castigat");
        else
            outtextxy(760,60,"Player 2 a castigat");
        bar(755,100,855,150);
        bar(865,100,965,150);
        outtextxy(880,125,"Restart?");
        outtextxy(770,125,"Inchide?");
        butoaneWinWindow();

    }
}

void drawboard() //o fereastra unde se afiseaza de cate ori a castigat un jucator si care dintre ei urmeaza sa puna
{
    setfillstyle(SOLID_FILL,13);
    bar(750,150,975,450);
    setcolor(15);
    outtextxy(830,200,"Jucator 1");
    int x1= 760;
    int x2= 790;
    int y1= 220;
    int y2= 320;
    outtextxy(830,300,"Jucator 2");
    outtextxy(830,170,"Rand: jucatorul 1");
}

void drawTabla() //desenam tabla de joc
{
    int i,j;
    if(tema==1) //imaginea de fundal este in functie de valoare data lui t in meniul 2 de catre player
    {
        readimagefile("bg1.gif", 0,0,1000,1000);
    }
    if(tema==2)
    {
        readimagefile("bg2.gif", 0,0,1000,1000);
    }
    drawboard();
    setcolor(11);
    rectangle(750,500,950,700);//locatie de restart
    rectangle(0,0,40,90);//buton de bac
    setcolor(1);
    int numarColoane=c;
    int numarLinii=l;
    numarColoane+=1;
    numarLinii+=1;
    numarLinii*=100;
    numarColoane*=100;
    if(numarLinii<=700) //in cazul in care tabla de joc este maxim 7x7 atunci latura ramane 100
    {
        for(i=50; i<numarColoane-100; i+=100)
            for(j=100; j<=numarLinii-100; j+=100) //umplem tabla de joc cu culoarea alba
            {
                setcolor(WHITE);
                rectangle(i,j,i+100,j+100);
                setfillstyle(SOLID_FILL,WHITE);
                floodfill(i+50,j+50,WHITE);

                setcolor(BLUE);
                rectangle(i,j,i+100,j+100);
            }
        i=i-100; //coltul nergu este umplut cu culoarea neagra
        j=j-100;
        setcolor(BLACK);
        rectangle(i,j,i+100,j+100);
        setfillstyle(SOLID_FILL,BLACK);
        floodfill(i+50,j+50,BLACK);
    }



    else //in cazul in care este tabla mai mare de 7x7, latura este 75
    {
        int nl=numarLinii-100, nc=numarColoane-100;
        int c1=nl-(nl/100)*75, c2=nc-(nc/100)*75;
        numarLinii-=c1;
        numarColoane-=c2;
        for(i=50; i<numarColoane-75; i+=75)
            for(j=100; j<=numarLinii-75; j+=75)
            {
                setcolor(WHITE); //umplem tabla de joc cu alb
                rectangle(i,j,i+75,j+75);
                setfillstyle(SOLID_FILL,WHITE);
                floodfill(i+50,j+50,WHITE);
                setcolor(BLUE);
                rectangle(i,j,i+75,j+75);
            }
        i=i-75;
        j=j-75;
        setcolor(BLACK); //coltul negru este umplut cu culoarea neagra
        rectangle(i,j,i+75,j+75);
        setfillstyle(SOLID_FILL,BLACK);
        floodfill(i+50,j+50,BLACK);
    }

}

void MeniuJetoane() // jetoanele sunt desenate deasupra tablei de joc
{
    if(tema==1)
    {
        readimagefile("cruce1.jpg",110,10,190,90); //jetonu 1 in cruce
        readimagefile("11.jpg",200,10,280,90); //jetonul 2, jos-stanga, dreapta-sus
        readimagefile("21.jpg",290,10,370,90); //jetonul 2, jos-dreapta, stanga-sus
    }
    if(tema==2)
    {
        readimagefile("cruce2.jpg",110,10,190,90); //jetonu 1 in cruce
        readimagefile("12.jpg",200,10,280,90); //jetonul 2, jos-stanga, dreapta-sus
        readimagefile("22.jpg",290,10,370,90); //jetonul 2, jos-dreapta, stanga-sus
    }
    setlinestyle(SOLID_LINE,USERBIT_LINE,THICK_WIDTH);
    for(int k=110; k<=370; k+=90)
    {
        rectangle(k,10,k+80,90);
    }
    setlinestyle(SOLID_LINE,USERBIT_LINE,NORM_WIDTH);
}

bool inMatrice(int x,int y) //verificam daca click-ul este pe tabla de joc
{
    if(x>=50 && y>=100 && x<=50+latura*c && y<=100+latura*l)
        return 1;
    return 0;
}

void AlegereJeton() //se alege jetonul dorit
{
    int x,y;
    int cont=0; //contorul retine care dintre jetoane este ales
    gataselectie=0; // alegerea are loc cat timp selectia nu este gata
    do
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            clearmouseclick(WM_LBUTTONDOWN);
            x=mousex();
            y=mousey();
            backbutton(x,y); //in orice moment se poate folosi butonul de back
            if(x>=100 && x<=180 && y>=10 &&y<=90) //pentru jetonul 1
            {
                setcolor(1);
                setlinestyle(SOLID_LINE,USERBIT_LINE,THICK_WIDTH);
                rectangle(290,10,370,90);
                rectangle(200,10,280,90);
                setcolor(RED); //jetonul ales are outline rosu
                rectangle(110,10,190,90);
                setlinestyle(SOLID_LINE,USERBIT_LINE,NORM_WIDTH);
                setcolor(1);
                cont=1;
                cout<<cont<<endl;
            }
            else if(x>=190 && x<=270 && y>=10 &&y<=90) //pentru jetonul 2
            {
                setcolor(1);
                setlinestyle(SOLID_LINE,USERBIT_LINE,THICK_WIDTH);
                rectangle(110,10,190,90);
                rectangle(290,10,370,90);
                setcolor(RED);//jetonul ales are outline rosu
                rectangle(200,10,280,90);
                setlinestyle(SOLID_LINE,USERBIT_LINE,NORM_WIDTH);
                setcolor(1);
                cont=2;
                cout<<cont<<endl;
            }
            else if(x>=280 && x<=360 && y>=10 &&y<=90)//pentru jetonul 3
            {
                setcolor(1);
                setlinestyle(SOLID_LINE,USERBIT_LINE,THICK_WIDTH);
                rectangle(110,10,190,90);
                rectangle(200,10,280,90);
                setcolor(RED);//jetonul ales are outline rosu
                rectangle(290,10,370,90);
                setlinestyle(SOLID_LINE,USERBIT_LINE,NORM_WIDTH);
                setcolor(1);
                cont=3;
                cout<<cont<<endl;
            }
            else if(inMatrice(x,y)==1 && cont!=0)
                selectiepatrat(x,y,cont);//daca este dat click in matrice si contorul este diferit de zero incepe selectia patratului in care va fi pus jetonul
        }
    }
    while(gataselectie==0);
}

void selectiepatrat(int x,int y,int cont)
{
    if(c>=7 && l>=7) //daca avem o tabla mai mare de 6x6, jetonale trebuie si ele sa fie mai mici deci latura devine 75
    {
        latura=75;
    }
    col=(x-50)/latura+1;
    lin=(y-100)/latura+1;
    if(x>=50 && y>=100 && x<=50+latura*c && y<=100+latura*l)
    {
        if(verificareDrum(lin,col,cont)==1) //nu putem pune pe tabla jeton decat daca verificarea drumului pentru acea locatie este adevarata
        {
            cout<<lin<<endl;
            gataselectie=1;//selectia este gata
            tabla[lin][col].esteJeton=1; //avem jeton in acea locatie
            if(cont==1) //daca avem contorul 1, atunci este jetonul 1
            {
                tabla[lin][col].jeton=1;//deci in matrice vom aveam acel jeton
                if(tema==1)
                {
                    readimagefile("cruce1.jpg",50+latura*(col-1),100+latura*(lin-1),50+latura*(col),100+latura*(lin));//putem jetonul in acea locatie
                }
                if(tema==2)
                {
                    readimagefile("cruce2.jpg",50+latura*(col-1),100+latura*(lin-1),50+latura*(col),100+latura*(lin));
                }
                setcolor(BLUE);
                rectangle(50+latura*(col-1),100+latura*(lin-1),50+latura*(col),100+latura*(lin));
                playerMove(lin,col);//verificam care player a pus
                actualizareDrum(lin,col,cont);
                castigarepriniesire(lin,col);
                castigColt(col,lin);//verificam posibilitatiile de castig

            }
            else if(cont==2)//daca avem contorul 2, atunci este jetonul 2
            {
                tabla[lin][col].jeton=2;//deci inmatrice vom aveam acel jeton
                if(tema==1)
                {
                    readimagefile("11.jpg",50+latura*(col-1),100+latura*(lin-1),50+latura*(col),100+latura*(lin));
                }
                if(tema==2)
                {
                    readimagefile("12.jpg",50+latura*(col-1),100+latura*(lin-1),50+latura*(col),100+latura*(lin));
                }
                setcolor(BLUE);
                rectangle(50+latura*(col-1),100+latura*(lin-1),50+latura*(col),100+latura*(lin));
                playerMove(lin,col);//verificam care player a pus
                actualizareDrum(lin,col,cont);
                castigColt(col,lin);//verificam posibilitatiile de castig
                castigarepriniesire(lin,col);

            }
            else if(cont==3)//daca avem contorul 3, atunci este jetonul 3
            {
                tabla[lin][col].jeton=3;//pe matrice avem jetonul 3
                if(tema==1)
                {
                    readimagefile("21.jpg",50+latura*(col-1),100+latura*(lin-1),50+latura*(col),100+latura*(lin));
                }
                if(tema==2)
                {
                    readimagefile("22.jpg",50+latura*(col-1),100+latura*(lin-1),50+latura*(col),100+latura*(lin));
                }
                setcolor(BLUE);
                rectangle(50+latura*(col-1),100+latura*(lin-1),50+latura*(col),100+latura*(lin));
                playerMove(lin,col);//verificam care player a pus
                actualizareDrum(lin,col,cont);
                castigColt(col,lin);//verificam posibilitatiile de castig
                castigarepriniesire( lin,col);

            }
          if(vsbot==1) //in cazul in care se joaca vsbot, dupa ce pune player, va fi pusa pe tabla o piesa random
        {
            randomMove();
        }
        }

    }

}
/// functii care actualizeaza iesirile dupa ce un jeton care se leaga de drum dintr-o anumita parte devine jeton anterior
void jeton1iesiresus(int lin,int col, int cont)
{
    if(cont==1)
    {
        jetAnt.down=0;
        jetAnt.right=0;
        jetAnt.up=1;
        jetAnt.left=0;
    }
}
void jeton1iesiredreapta(int lin,int col, int cont)
{
    if(cont==1)
    {
        jetAnt.down=0;
        jetAnt.right=1;
        jetAnt.up=0;
        jetAnt.left=0;
    }
}
void jeton1iesirestanga(int lin,int col, int cont)
{
    if(cont==1)
    {

        jetAnt.down=0;
        jetAnt.right=0;
        jetAnt.up=0;
        jetAnt.left=1;
    }
}
void jeton1iesirejos(int lin,int col, int cont)
{
    if(cont==1)
    {
        jetAnt.down=1;
        jetAnt.right=0;
        jetAnt.up=0;
        jetAnt.left=0;
    }
}
void jeton2iesiresus(int lin,int col, int cont)
{
    if(cont==2)
    {

        jetAnt.up=1;
        jetAnt.down=0;
        jetAnt.right=0;
        jetAnt.left=0;
    }

}
void jeton2iesirejos(int lin,int col, int cont)
{
    if(cont==2)
    {
        jetAnt.up=0;
        jetAnt.down=1;
        jetAnt.right=0;
        jetAnt.left=0;
    }
}
void jeton2iesiredreapta(int lin,int col, int cont)
{
    if(cont==2)
    {
        jetAnt.up=0;
        jetAnt.down=0;
        jetAnt.right=1;
        jetAnt.left=0;
    }
}
void jeton2iesirestanga(int lin,int col, int cont)
{
    if(cont==2)
    {
        jetAnt.up=0;
        jetAnt.down=0;
        jetAnt.right=0;
        jetAnt.left=1;
    }
}
void jeton3iesiresus(int lin,int col, int cont)
{
    if(cont==3)
    {

        jetAnt.up=1;
        jetAnt.down=0;
        jetAnt.right=0;
        jetAnt.left=0;
    }
}
void jeton3iesirejos(int lin,int col, int cont)
{
    if(cont==3)
    {
        jetAnt.up=0;
        jetAnt.down=1;
        jetAnt.right=0;
        jetAnt.left=0;
    }
}
void jeton3iesiredreapta(int lin,int col, int cont)
{
    if(cont==3)
    {
        jetAnt.up=0;
        jetAnt.down=0;
        jetAnt.right=1;
        jetAnt.left=0;
    }
}
void jeton3iesirestanga(int lin,int col, int cont)
{
    if(cont==3)
    {
        jetAnt.up=0;
        jetAnt.down=0;
        jetAnt.right=0;
        jetAnt.left=1;
    }
}
bool verificareDrum(int &lc,int &cc,int cont) ///verifiarea drumului ne lasa sa punem pe tabla un jeton doar in cazul in care se poate continua drumul cu el
{
    cout<<"cl ant "<<ca<<" "<<"lin ant "<<la<<endl;
    cout<<"cl curent "<<cc<<" "<<"lin curent "<<lc<<endl;
    cout<<"jeton "<<cont<<endl;


    if(ca==-1 && la==-1 && lc==1 && cc==1 && cont==1)// cum jocul incepe doar cu jetonul 1 intr-o anumita locatie, nu se poate pune decat acel jeton ca rpima piesa
    {
        ca=1,la=1; //odata pusa pe tabla, se actualizeaza coloana si linia anterioara
        jetAnt.down=1; //se actualizeaza si iesirile care sunt in functie de jeton
        jetAnt.right=1;
        jetAnt.up=0;
        jetAnt.left=0;
        cout<<"cl ant "<<ca<<" "<<"lin ant "<<la<<endl;
        return 1;

    }
    if((abs(ca-cc)==1 && la==lc) || (abs(la-lc)==1 && ca==cc))// o piesa nu poate sa fie pusa decat daca este conectata de jetonul anterior, deci fie coloana e aceasi si linia diferita, fie invers
    {
        if(cont==2 && jetAnt.down==1 && lc==la+1 && cc==ca) //luam pe cazuri, daca se pune jetonul doi si jet ant are o anumita iesire
        {
            // atunci linia si coloana curenta (lc, cc) vor diferentia de linia si coloana ant
            la=lc;                                          // in functie de fiecare caz
            jeton2iesiredreapta(lc,cc,cont);
            // cum jet ant are iesire in jos, atunci jetonul 2 are iesire la dreapta
            cout<<"cl ant "<<ca<<" "<<"lin ant "<<la<<endl;
            return 1;
        }
        if(cont==2 && jetAnt.right==1 && lc==la && cc==ca+1)
        {
            ca=cc;//coloana actuala devine coloana curenta
            jeton2iesirejos(lc,cc,cont);
            //jet ant are iesire la dreapta, deci jetonul 2 are iesire in jos
            cout<<"cl ant "<<ca<<" "<<"lin ant "<<la<<endl;
            return 1;
        }
        if(cont==2 && jetAnt.up==1 && lc==la-1 && cc==ca)
        {
            la=lc;//linia actuala devine linia curenta
            jeton2iesirestanga(lc,cc,cont);//jet ant are iesire in sus, deci jetonul 2 are iesire la stanga
            cout<<"cl ant "<<ca<<" "<<"lin ant "<<la<<endl;
            return 1;
        }
        if(cont==2 && jetAnt.left==1 && lc==la && cc==ca-1)
        {
            ca=cc;//coloana actuala devine coloana curenta
            jeton2iesiresus(lc,cc,cont);//jet ant are iesire in stanga, deci jetonul 2 are iesire in sus
            cout<<"cl ant "<<ca<<" "<<"lin ant "<<la<<endl;
            return 1;
        }
        ///jeton 3 este jetonul pe care playerul doreste sa il puna jos, urmareste aceleasi cazuri ca si jetonul 2
        if(cont==3 && jetAnt.down==1 && lc==la+1 && cc==ca)
        {
            la=lc;//linia actuala devine linia curenta
            jeton3iesirestanga(lc,cc,cont);//jet ant are iesire in jos, deci jetonul 2 are iesire in stanga(continua drumul in stanga)
            cout<<ca<<" "<<la<<endl;
            return 1;
        }
        if(cont==3 && jetAnt.right==1 && lc==la && cc==ca+1)
        {
            ca=cc;//coloana actuala devine coloana curenta
            jeton3iesiresus(lc,cc,cont);//drumul este continuat in sus
            cout<<ca<<" "<<la<<endl;
            return 1;
        }
        if(cont==3 && jetAnt.up==1 && lc==la-1 && cc==ca)
        {
            la=lc;//linia actuala devine linia curenta
            jeton3iesiredreapta(lc,cc,cont);//drumul este continuat la dreapta
            cout<<ca<<" "<<la<<endl;
            return 1;
        }
        if(cont==3 && jetAnt.left==1 && lc==la && cc==ca-1)
        {
            ca=cc;//coloana actuala devine coloana curenta
            jeton3iesirejos(lc,cc,cont);//drumul este continuat la stanga
            cout<<ca<<" "<<la<<endl;
            return 1;
        }
        ///jeton cruce urmareste aceleasi regurli
        if(cont==1 && jetAnt.down==1 && abs(la-lc)==1 && cc==ca)
        {
            la=lc;//linia actuala devine linia curenta
            jeton1iesirejos(lin,col,cont);

            cout<<ca<<" "<<la<<endl;
            return 1;
        }
        if(cont==1 && jetAnt.right==1 && lc==la && cc==ca+1)
        {
            ca=cc;//coloana actuala devine coloana curenta
            jeton1iesiredreapta(lin,col,cont);

            cout<<ca<<" "<<la<<endl;
            return 1;
        }
        if(cont==1 && jetAnt.up==1 && lc==la-1 && cc==ca)
        {
            la=lc;//linia actuala devine linia curenta
            jeton1iesiresus(lin,col,cont);

            cout<<ca<<" "<<la<<" ";
            return 1;
        }
        if(cont==1 && jetAnt.left==1 && lc==la && cc==ca-1)
        {
            ca=cc;//coloana actuala devine coloana curenta
            jeton1iesirestanga(lin,col,cont);

            cout<<ca<<" "<<la<<endl;
            return 1;
        }
    }
    return 0;
}
void actualizareDrum(int &lin, int &col,int cont) //functia ne actualizeaza locatia jet ant in cazul in care jetonul pe care playerul la pus pe tabla se leaga de un jeton existent
{

    do
    {
        ok=0; // ne folosim de un contor care creste in momentul in care deja se intalneste o piesa pe tabla
        if(cont==1) //pt piesa 1
        {
            if(jetAnt.down==1 && tabla[lin+1][col].esteJeton==1) //cand are iesire jos si e deja o piesa acolo
            {
                lin=lin+1; //linia creste
                switch(tabla[lin][col].jeton) //actualizam locatia jetonului anterior
                {
                case 1://jeton 1, in functie de care jeton se afla acolo, sunt actualizate si iesirile sale
                {
                    jeton1iesirejos(lin,col,1);
                    la=lin; //linia anterioara devine linia curenta
                    ok++;
                    break;
                }
                case 2: // este jetonul 2 pe tabla, sunt actualizate iesire si locatia
                {
                    jeton2iesiredreapta(lin,col,2);
                    la=lin;//linia anterioara devine linia curenta
                    ok++;
                    break;
                }
                case 3: //este jetonul 3 pe tabla, sunt actualizate iesire si locatia
                {
                    jeton3iesirestanga(lin,col,3);
                    la=lin;//linia anterioara devine linia curenta
                    ok++;
                    break;
                }
                }
            }
            if(jetAnt.up==1 && tabla[lin-1][col].esteJeton==1)//cand are iesire in sus
            {
                lin=lin-1;
                switch(tabla[lin][col].jeton)
                {
                case 1://jeton 1
                {
                    jeton1iesiresus(lin,col,1);
                    la=lin;//linia anterioara devine linia curenta
                    ok++;
                    break;
                }
                case 2: //jeton 2
                {
                    la=lin;//linia anterioara devine linia curenta
                    jeton2iesirestanga(lin,col,2);

                    ok++;
                    break;
                }
                case 3: //jeton 3
                {
                    jeton3iesiredreapta(lin,col,3);
                    la=lin;//linia anterioara devine linia curenta
                    ok++;
                    break;
                }
                }
            }
            if(jetAnt.right==1 && tabla[lin][col+1].esteJeton==1)//cand are iesire in dreapta
            {
                col=col+1; //colona creste cu 1
                switch(tabla[lin][col].jeton)
                {
                case 1:
                {
                    jeton1iesiredreapta(lin,col,1);
                    ca=col;//coloana ant este coloana curenta
                    ok++;
                    break;
                }
                case 2: //jeton 2
                {
                    jeton2iesirejos(lin,col,2);
                    ca=col;//coloana ant este coloana curenta
                    ok++;
                    break;
                }
                case 3: //jeton 3
                {
                    jeton3iesiresus(lin,col,3);
                    ca=col;//coloana ant este coloana curenta
                    ok++;
                    break;
                }
                }
            }
            if(jetAnt.left==1 && tabla[lin][col-1].esteJeton==1)//cand are iesire in stanga
            {
                col=col-1;
                switch(tabla[lin][col].jeton)
                {
                case 1:
                {
                    jeton1iesirestanga(lin,col,1);
                    ca=col;//coloana ant este coloana curenta
                    ok++;
                    break;
                }
                case 2: //jeton 2
                {
                    jeton2iesiresus(lin,col,2);
                    ca=col;//coloana ant este coloana curenta
                    ok++;
                    break;
                }
                case 3: //jeton 3
                {
                    jeton3iesirejos(lin,col,3);
                    ca=col;//coloana ant este coloana curenta
                    ok++;
                    break;
                }
                }
            }
        }
        // se repeta pentru celelalte 2 jetoane si pentru fiecare iesire cele 3 cazuri posibile
        if(cont==2) //pt piesa 2
        {
            if(jetAnt.down==1 && tabla[lin+1][col].esteJeton==1) //cand are iesire jos si e deja o piesa acolo
            {
                lin=lin+1;
                switch(tabla[lin][col].jeton)
                {
                case 1://jeton 1
                {
                    jeton1iesirejos(lin,col,1);
                    la=lin;
                    ok++;
                    break;
                }
                case 2: //jeton 2
                {
                    jeton2iesiredreapta(lin,col,2);
                    la=lin;
                    ok++;
                    break;
                }
                case 3: //jeton 3
                {
                    jeton3iesirestanga(lin,col,3);
                    la=lin;
                    ok++;
                    break;
                }
                }
            }
            if(jetAnt.up==1 && tabla[lin-1][col].esteJeton==1)//cand are iesire in sus
            {
                lin=lin-1;
                switch(tabla[lin][col].jeton)
                {
                case 1://jeton 1
                {
                    jeton1iesiresus(lin,col,1);
                    la=lin;
                    cout<<lin<<" "<<col<<endl;
                    ok++;
                    break;
                }
                case 2: //jeton 2
                {
                    la=lin;
                    jeton2iesirestanga(lin,col,2);

                    ok++;
                    break;
                }
                case 3: //jeton 3
                {
                    jeton3iesiredreapta(lin,col,3);
                    la=lin;
                    ok++;
                    break;
                }
                }
            }
            if(jetAnt.right==1 && tabla[lin][col+1].esteJeton==1)//cand are iesire in dreapta
            {
                col=col+1;
                switch(tabla[lin][col].jeton)
                {
                case 1:
                {
                    jeton1iesiredreapta(lin,col,1);
                    ca=col;
                    ok++;
                    break;
                }
                case 2: //jeton 2
                {
                    jeton2iesirejos(lin,col,2);
                    ca=col;
                    ok++;
                    break;
                }
                case 3: //jeton 3
                {
                    jeton3iesiresus(lin,col,3);
                    ca=col;
                    ok++;
                    break;
                }
                }
            }
            if(jetAnt.left==1 && tabla[lin][col-1].esteJeton==1)//cand are iesire in stanga
            {
                col=col-1;
                switch(tabla[lin][col].jeton)
                {
                case 1:
                {
                    jeton1iesirestanga(lin,col,1);
                    ca=col;
                    ok++;
                    break;
                }
                case 2: //jeton 2
                {
                    jeton2iesiresus(lin,col,2);
                    ca=col;
                    ok++;
                    break;
                }
                case 3: //jeton 3
                {
                    jeton3iesirejos(lin,col,3);
                    ca=col;
                    ok++;
                    break;
                }
                }
            }
        }
        if(cont==3)//pt piesa 3
        {
            if(jetAnt.down==1 && tabla[lin+1][col].esteJeton==1) //cand are iesire jos si e deja o piesa acolo
            {
                lin=lin+1;
                switch(tabla[lin][col].jeton)
                {
                case 1://jeton 1
                {
                    jeton1iesirejos(lin,col,1);
                    la=lin;
                    ok++;
                    break;
                }
                case 2: //jeton 2
                {
                    jeton2iesiredreapta(lin,col,2);
                    la=lin;
                    ok++;
                    break;
                }
                case 3: //jeton 3
                {
                    jeton3iesirestanga(lin,col,3);
                    la=lin;
                    ok++;
                    break;
                }
                }
            }
            if(jetAnt.up==1 && tabla[lin-1][col].esteJeton==1)//cand are iesire in sus
            {
                lin=lin-1;
                switch(tabla[lin][col].jeton)
                {
                case 1://jeton 1
                {
                    jeton1iesiresus(lin,col,1);
                    la=lin;
                    ok++;
                    break;
                }
                case 2: //jeton 2
                {
                    jeton2iesirestanga(lin,col,2);
                    la=lin;
                    ok++;
                    break;
                }
                case 3: //jeton 3
                {
                    jeton3iesiredreapta(lin,col,3);
                    la=lin;
                    ok++;
                    break;
                }
                }
            }
            if(jetAnt.right==1 && tabla[lin][col+1].esteJeton==1)//cand are iesire in dreapta
            {
                col=col+1;
                switch(tabla[lin][col].jeton)
                {
                case 1:
                {
                    jeton1iesiredreapta(lin,col,1);
                    ca=col;
                    ok++;
                    break;
                }
                case 2: //jeton 2
                {
                    jeton2iesirejos(lin,col,2);
                    ca=col;
                    ok++;
                    break;
                }
                case 3: //jeton 3
                {
                    jeton3iesiresus(lin,col,3);
                    ca=col;
                    ok++;
                    break;
                }
                }
            }
            if(jetAnt.left==1 && tabla[lin][col-1].esteJeton==1)//cand are iesire in stanga
            {
                col=col-1;
                switch(tabla[lin][col].jeton)
                {
                case 1:
                {
                    jeton1iesirestanga(lin,col,1);
                    ca=col;
                    ok++;
                    break;
                }
                case 2: //jeton 2
                {
                    jeton2iesiresus(lin,col,2);
                    ca=col;
                    ok++;
                    break;
                }
                case 3: //jeton 3
                {
                    jeton3iesirejos(lin,col,3);
                    ca=col;
                    ok++;
                    break;
                }
                }
            }
        }
    }
    while(ok>0);
    ok=0;
    la=lin;
    ca=col;

}
void playerMove(int lin,int col)//functia verifica ce player a pus pe tabla
{
    if(tabla[lin][col].jeton>=1 && tabla[lin][col].jeton<=3) //cat timp se afla un jeton pe tabla si este unul din cele 3
    {
        if(lin==1 and col==1) //daca este primul jeton pus pe tabla, este primul player
            player=1;
        else
            player=3-player;// daca nu, el se calculeaza in functie de cat era
        cout<<"Player "<<player<<" moved."<<endl;
    }
    setcolor(WHITE);
    if(player==1)
        outtextxy(830,170,"Rand: jucatorul 2");
    else
        outtextxy(830,170,"Rand: jucatorul 1");
}

bool castigColt(int col,int lin)//castigarea cand se ajuneg in colt are doar 2 posibilitati
{
    if(jetAnt.down==1 && col==c && lin==l-1) //daca drumul vine din partea de sus
        return 1;
    if(jetAnt.right==1 && col==c-1 && lin==l) //daca drumul vine din partea stanga a coltului megru
        return 1;
    return 0;
}

bool castigarepriniesire(int lin,int col) // daca drumul este dus la marginea tablei sunt 4 posibilitati
{


    if(jetAnt.up==1 && lin==1 ) //daca drumul iese de pe tabla prin partea de sus
        return 1;
    if(jetAnt.down==1 && lin==l) //daca drumul iese de pe tabla prin partea de jos
        return 1;
    if(jetAnt.right==1 && col==c) //daca drumul iese de pe tabla prin dreapta
        return 1;
    if(jetAnt.left==1 && col==1) // daca drumul iese de pe tabla prin stanga
        return 1;
    return 0;
}

void bordingTable() // ingradim tabla de joc ca botul sa nu puna inafara ei
{
    for(int i=1; i<=c; i++)
    {
        tabla[0][i].esteJeton=2;
        tabla[l+1][i].esteJeton=2;
    }
    for(int i=1; i<=l; i++)
    {
        tabla[i][0].esteJeton=2;
        tabla[i][c+1].esteJeton=2;
    }
}

void returnFreeSpaces(int &jetRand)
{
    cout<<endl<<endl<<endl<<"Miscarea "<<i<<":"<<endl;
    cout<<"coordonatele actuale sunt: LIN="<<lin<<" si  COL="<<col<<endl;
    if(tabla[lin][col].jeton==1)
    {
        cout<<jetAnt.right<<" "<<jetAnt.left<<" "<<jetAnt.up<<" "<<jetAnt.down<<" "<<endl;
        if(lin==1 && col==1)//prima piesa de pe tabla
        {
            srand(time(0));
            jetRand=rand()%2+1;
            cr=col+1;
            lr=lin;
        }
        else if(jetAnt.down==1 and tabla[lin+1][col].esteJeton!=2)//vine de sus
        {
            lr=lin+1;
            cr=col;

            ///alegerea jetonului
            AlegereJetonRandom(jetRand);
            ///

        }
        else if(jetAnt.up==1 and tabla[lin-1][col].esteJeton!=2)//vine de jos  lant==lin+1
        {
            lr=lin-1;
            cr=col;

            ///alegere jeton
            AlegereJetonRandom(jetRand);
            ///
        }
        else if(jetAnt.left==1 and tabla[lin][col-1].esteJeton!=2)//vine din dreapta  cant==col+1
        {
            cr=col-1;
            lr=lin;

            ///alegere jeton
            AlegereJetonRandom(jetRand);
            ///
        }
        else if(jetAnt.right==1 and tabla[lin][col+1].esteJeton!=2)//vine din stanga
        {
            cr=col+1;
            lr=lin;

            ///alegere jeton
            AlegereJetonRandom(jetRand);
            ///
        }
        else
        {
            cr=-1;
            lr=-1;
        }
    }
    else if(tabla[lin][col].jeton==2)
    {
        cout<<jetAnt.right<<" "<<jetAnt.left<<" "<<jetAnt.up<<" "<<jetAnt.down<<endl;
        if( jetAnt.up==1 and tabla[lin-1][col].esteJeton!=2)//vine din dreapta   cant==col+1
        {
            lr=lin-1;
            cr=col;

            ///alegere jeton
            AlegereJetonRandom(jetRand);
            ///
        }
        else if( jetAnt.down==1 and tabla[lin+1][col].esteJeton!=2)//vine din stanga   cant==col-1
        {
            lr=lin+1;
            cr=col;

            ///alegerea jetonului
            AlegereJetonRandom(jetRand);
            ///
        }
        else if(jetAnt.left==1 and tabla[lin][col-1].esteJeton!=2)//vine de jos   lant==lin-1
        {
            lr=lin;
            cr=col-1;

            ///alegere jeton
            AlegereJetonRandom(jetRand);
            ///
        }
        else if(jetAnt.right==1 and tabla[lin][col+1].esteJeton!=2)//vine de sus  lant==lin+1
        {
            lr=lin;
            cr=col+1;

            ///alegere jeton
            AlegereJetonRandom(jetRand);
            ///
        }
        else
        {
            cr=-1;
            lr=-1;
        }
    }
    else if(tabla[lin][col].jeton==3)
    {
        cout<<jetAnt.right<<" "<<jetAnt.left<<" "<<jetAnt.up<<" "<<jetAnt.down<<endl;
        if(jetAnt.right==1 and tabla[lin][col+1].esteJeton!=2)//vine de jos  lant==lin+1
        {
            lr=lin;
            cr=col+1;

            ///alegere jeton
            AlegereJetonRandom(jetRand);
            ///
        }
        else if(jetAnt.left==1 and tabla[lin][col-1].esteJeton!=2)//vine de sus  lant==lin-1
        {
            lr=lin;
            cr=col-1;
            ///alegere jeton
            AlegereJetonRandom(jetRand);
            ///
        }
        else if(jetAnt.up==1 and tabla[lin-1][col].esteJeton!=2)//vine din stanga  cant==col-1
        {
            lr=lin-1;
            cr=col;
            ///alegere jeton
            AlegereJetonRandom(jetRand);
            ///
        }
        else if(jetAnt.down==1 and tabla[lin+1][col].esteJeton!=2)//vine din dreapta  cant==col+1
        {
            lr=lin+1;
            cr=col;

            ///alegerea jetonului
            AlegereJetonRandom(jetRand);
            ///
        }
        else
        {
            cr=-1;
            lr=-1;
        }

    }

    cout<<"lr="<<lr<<" si cr="<<cr<<endl;
    if(tabla[lr][cr].esteJeton==1)
    {
        cout<<endl<<"*"<<endl;
        lin=lr;
        col=cr;
        if(verificareDrum(lin,col,tabla[lin][col].jeton)==1)
            actualizareDrum(lin,col,tabla[lin][col].jeton);
        returnFreeSpaces(jetRand);
    }
}

void AlegereJetonRandom(int &jetRand)
{
    srand(time(0));
    jetRand=rand()%3+1;
    if(jetAnt.up==1)
    {
        if(col==1 and lin==2)
            jetRand=3;
        else if(col==c and lin==2)
            jetRand=2;
        else
        {
            if(col==1)
            {
                srand(time(0));
                jetRand=rand()%2+1;
                if(jetRand==2)
                    jetRand=3;
            }
            else if(col==c)
            {
                srand(time(0));
                jetRand=rand()%2+1;
            }
            if(lin==2)
            {
                srand(time(0));
                jetRand=rand()%2+2;
            }
        }
    }
    else if(jetAnt.down==1)
    {
        if(col==1 and lin==l-1)
            jetRand=2;
        else if(col==c and lin==l-1)
        {
            lr=-1;
            cr=-1;
            return;
        }
        else
        {
            if(col==1)
            {
                srand(time(0));
                jetRand=rand()%2+1;
            }
            else if(col==c)
            {
                srand(time(0));
                jetRand=rand()%2+1;
                if(jetRand==2)
                    jetRand=3;
            }
            if(lin==l-1)
            {
                srand(time(0));
                jetRand=rand()%2+2;
            }
        }
    }
    else if(jetAnt.left==1)
    {
        if(lin==1 and col==2)
            jetRand=3;
        else if(lin==l and col==2)
            jetRand=2;
        else
        {
            if(col==2)
            {
                srand(time(0));
                jetRand=rand()%2+2;
            }
            if(lin==1)
            {
                srand(time(0));
                jetRand=rand()%2+1;
                if(jetRand==2)
                    jetRand=3;
            }
            else if(lin==l)
            {
                srand(time(0));
                jetRand=rand()%2+1;
            }
        }
    }
    else if(jetAnt.right==1)
    {
        if(lin==1 and col==c-1)
            jetRand=2;
        else if(lin==l and col==c-1)
        {
            lr=-1;
            cr=-1;
            return;
        }
        else
        {
            if(lin==1)
            {
                srand(time(0));
                jetRand=rand()%2+1;
            }
            else if(lin==l)
            {
                srand(time(0));
                jetRand=rand()%2+1;
                if(jetRand==2)
                    jetRand=3;
            }
            if(col==c)
            {
                srand(time(0));
                jetRand=rand()%2+2;
            }
        }
    }
}

void punereJeton(int cont)
{

    latura=100;
    if(c>=7 && l>=7)
    {
        latura=75;
    }

    if(verificareDrum(lr,cr,cont)==1)
    {
        tabla[lr][cr].esteJeton=1;
        if(cont==1)
        {
            tabla[lr][cr].jeton=1;
            delay(500);
            if(tema==1)
            {
                readimagefile("cruce1.jpg",50+latura*(cr-1),100+latura*(lr-1),50+latura*(cr),100+latura*(lr));
            }
            if(tema==2)
            {
                readimagefile("cruce2.jpg",50+latura*(cr-1),100+latura*(lr-1),50+latura*(cr),100+latura*(lr));
            }
            setcolor(BLUE);
            rectangle(50+latura*(cr-1),100+latura*(lr-1),50+latura*(cr),100+latura*(lr));
            playerMove(lr,cr);
            castigColt(cr,lr);
            castigarepriniesire(lr,cr);
            actualizareDrum(lr,cr,cont);
        }
        else if(cont==2)
        {
            tabla[lr][cr].jeton=2;
            delay(500);
            if(tema==1)
            {
                readimagefile("11.jpg",50+latura*(cr-1),100+latura*(lr-1),50+latura*(cr),100+latura*(lr));
            }
            if(tema==2)
            {
                readimagefile("12.jpg",50+latura*(cr-1),100+latura*(lr-1),50+latura*(cr),100+latura*(lr));
            }
            setcolor(BLUE);
            rectangle(50+latura*(cr-1),100+latura*(lr-1),50+latura*(cr),100+latura*(lr));
            playerMove(lr,cr);
            castigColt(cr,lr);
            castigarepriniesire(lr,cr);
            actualizareDrum(lr,cr,cont);
        }
        else if(cont==3)
        {
            tabla[lr][cr].jeton=3;
            delay(500);
            if(tema==1)
            {
                readimagefile("21.jpg",50+latura*(cr-1),100+latura*(lr-1),50+latura*(cr),100+latura*(lr));
            }
            if(tema==2)
            {
                readimagefile("22.jpg",50+latura*(cr-1),100+latura*(lr-1),50+latura*(cr),100+latura*(lr));
            }
            setcolor(BLUE);
            rectangle(50+latura*(cr-1),100+latura*(lr-1),50+latura*(cr),100+latura*(lr));
            playerMove(lr,cr);
            castigColt(cr,lr);
            castigarepriniesire( lr,cr);
            actualizareDrum(lr,cr,cont);
        }
    }
    latura=100;
}

void randomMove()
{
    int jetRand;
    bordingTable();
    returnFreeSpaces(jetRand);//returneaza coordonatele unui spatiu liber si care poate fi drum
    if(lr==-1)//daca nu poate fi drum (daca lr=-1 atunci si cr=-1)
        cout<<"Player 1 won!"<<endl;
    else
    {
        punereJeton(jetRand);
        cout<<"Se pune piesa "<<jetRand<<" pe linia "<<lr<<" si coloana "<<cr<<endl;
    }
}

void leaderBoard()
{
    v[1].jucator=1;
    v[2].jucator=2;
    v[winner].scor++;
    if(v[2].scor>v[1].scor)
        swap(v[1],v[2]);
    fout<<endl;
    fout<<"RUNDA:"<<rundadejoc;
    for(int i=1; i<=2; i++)
    {
        fout<<"          "<<i<<"       "<<v[i].jucator<<"       "<<v[i].scor<<endl<<endl;
        if(i==1)
            fout<<"       ";
    }
    fout<<"----------------------------------------------"<<endl<<endl;
}

