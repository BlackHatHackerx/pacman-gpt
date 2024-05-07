#include <graphics.h>
#include <random>
#include <cstdlib>
#include <iostream>
#include<string>
#include<time.h>
#include<sstream>
#include<ostream>
#include <chrono>
#include <thread>
#include <fstream>

using namespace std;

void outstream(ostringstream& out,int x,int y);
void draw_grid(int x[],int y[],int GRID_SIZE)
{
    for(int i=0;i<GRID_SIZE;i++)
    {
        for(int j=0;j<GRID_SIZE;j++)
        {
            if(i==GRID_SIZE-1||j==GRID_SIZE-1)
            {}
            rectangle(x[i],y[j],x[i+1],y[j+1]);
            if(i!=0&&j!=0)
            {
            circle(x[i]-10,y[j]-10,5);
            floodfill(x[i]-10,y[j]-10,WHITE);
            }
        }
    }
}
void drawGhost(int x,int y);
void drawGhostBlue(int x, int y);
void drawGhostBlack(int x,int y);
void drawGhostRed(int x,int y);
void drawGhostWhite(int x,int y);
void drawGhostLightBlue(int x, int y);
void drawGhostGreen(int x, int y);
void drawGhostYellow(int x,int y);
void drawGhostBrown(int x, int y);
bool ghostflagBlue=false;
int ghostvy=5; int ghostvx=0;
int ghostX1=200;int ghostY1=40;
int highscore;

int main()
{
   std::ifstream infile("highscore-list.txt");
    if(infile.is_open()){
        std::string line;
        while(std::getline(infile, line)){
            cout<<"Highscore: "<<line<<endl;
            highscore=stoi(line);
        }
        infile.close();
    }
    else {
    std::cerr << "Error opening file for reading.\n";
}
cout<<highscore;

    const int targetFPS = 60;
    const std::chrono::milliseconds frameTime(3500 / targetFPS);

    std::cout<<0x7ff;
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> dist(20,400);
    
    srand(time(NULL));
    const int GRID_SIZE=20;
    const int GRID_LENGTH=20;
    int ghostX=200; int ghostY=200;
    int rx= 30;
    int ry= 30;
    int counter=0;
    int savedscore=0;
    int vx=0;int vy=0;
    int score=0; int oldscore=0;
    bool circlepresent=true;
    bool ghostflag=true;
    bool ghostflagRed=true;
    bool ghostflagBlue1=false;
    int ghostvx1=5; int ghostvy1=0;
    bool up,down,left,right; int j=0;
    int x[GRID_SIZE]; int y[GRID_SIZE];
    bool circlecoll[100][100];  int no=1;
    int object[20][20]={{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                        {0,0,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,0},
                        {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
                        {0,0,0,1,0,1,1,1,1,0,1,0,1,1,1,1,0,1,0,0},
                        {0,0,0,1,0,1,1,1,1,0,1,0,1,1,1,1,0,1,0,0},
                        {0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0},
                        {0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0},
                        {0,0,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0},
                        {0,0,0,1,1,1,1,0,1,0,0,0,1,0,1,1,1,1,0,0},
                        {0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0},
                        {0,0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,1,1,0,0},
                        {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
                        {0,0,0,1,0,1,1,1,1,0,1,0,1,1,1,1,0,1,0,0},
                        {0,0,0,1,0,1,1,1,1,0,1,0,1,1,1,1,0,1,0,0},
                        {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
                        {0,0,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,0},
                        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
    for(int i=1;i<20;i++)
    {object[1][i]=1;
     object[19][i]=1;
     object[i][1]=1;
     object[i][19]=1;
    }
    


    x[0]=GRID_LENGTH; y[0]=GRID_LENGTH;
    for(int i=1;i<GRID_SIZE;i++)
    {
        for(int j=1;j<GRID_SIZE;j++)
        {
            if(i==3&&j==2)
            {rx=x[i]+10; ry=y[i]+10;}
            x[i]=x[i-1]+GRID_LENGTH;
            y[j]=y[j-1]+GRID_LENGTH;

        }
    }
    initwindow(700,700);
    int page=0;
    setfillstyle(1,14);
    pieslice(rx,ry,30,330,GRID_LENGTH/2);
    draw_grid(x,y,GRID_SIZE);
    getch();
    
    for(;;)
    {
         auto frameStart = std::chrono::high_resolution_clock::now(); 
    j++;
    if(j>10)
    j=0;    
        setactivepage(page);
        setvisualpage(1-page);
    
        cleardevice();
        
   

        if(GetAsyncKeyState(VK_DOWN))
        {
            
            vy=5;
            vx=0;
            setfillstyle(1,14);
            
            
            down=true;
            up=false;
            left=false;
            right=false; 
        }
        else if(GetAsyncKeyState(VK_UP))
        {
            
            vy=-5;
            vx=0;
           
            
            up=true;
            down=false;
            left=false;
            right=false;
            
        }
        else if(GetAsyncKeyState(VK_RIGHT))
        {
            if(left==true)
            vx=0;
            vx=5;
            vy=0;
            for(int i=4;i<18;i++)
            {
                if(object[4][i]==1)
                {
                    object[4][i]=0;
                    if(object[4][i+1]==1)
                        vx=0;
                    object[4][i+1]=1;
                    break;
                }
            }
            
            right=true;
            left=false;
            up=false;
            down=false;
        }
        else if(GetAsyncKeyState(VK_LEFT))
        {
            if(right==true)
            vx=0;
            vx=-5;
            vy=0;
            for(int i=4;i<19;i++)
            {
                if(object[4][i]==1)
                {
                    object[4][i]=0;
                    if(object[4][i-1]==1)
                        vx=0;
                    object[4][i-1]=1;
                    break;
                }
            }
            
            
            left=true;
            right=false;
            up=false;
            down=false;
            
        }
        else{
                rx+=vx;
                ry+=vy;
            int swapx=5; int swapy=5;

            for(int j=0;j<20;j++)
            {
                for(int i=0;i<20;i++)
                {
                    
                    if(rx>x[i]&&x[i+1]>rx&&ry>y[j]&&y[j+1]>ry)
                    {
                        cout<<"x[i] is "<<x[i]<<endl;
                        cout<<"rx is "<<rx<<endl;
                        cout<<"left box is "<<object[j+1][i]<<endl;
                        cout<<"right box is "<<object[j+1][i+2]<<endl;

                        cout<<"y[j] is "<<y[j]<<endl;
                        cout<<"ry is "<<ry<<endl;
                        cout<<"top box is "<<object[j][i+1]<<endl;
                        cout<<"bottom box is "<<object[j+2][i+1]<<endl;
                        if(object[j][i+1]==1&&ry-10<y[j])
                        {
                            ry=y[j]+10;
                            vy=0;
                        }
                        if(object[j+2][i+1]==1&&ry+10>y[j+1])
                        {
                            ry=y[j+1]-10;
                            vy=0;
                        }
                        if(object[j+1][i]==1&&rx-10<x[i])
                        {
                            rx=x[i]+10;
                            vx=0;
                        }
                        if(object[j+1][i+2]==1&&rx+10>x[i+1])
                        {
                            rx=x[i+1]-10;
                            vx=0;
                        }


                        
                       
                       
                    }
                    
                    

                }
            }
        }
        if(ghostX1+10>rx-10&&ghostX1<rx+10&&ghostY1+20>ry-10&&ghostY1<ry+10)
        {
            if(circlepresent==false)
            {
                ghostflagRed=false;
                ghostflagBlue1=false;
            }
            else
            {
                setvisualpage(page);
        cleardevice();
        std::ostringstream bgiout;
        setcolor(RED);
        
        outtextxy(300,300,"Ghost has eaten you.");
        
        delay(2000);
        break;
            }
        }
         if(ghostX+10>rx-10&&ghostX<rx+10&&ghostY+20>ry-10&&ghostY<ry+10)
     {
        if(circlepresent==false)
        {
            ghostflag=false;
            ghostflagBlue=false;
        }
        else{
        setvisualpage(page);
        cleardevice();
        std::ostringstream bgiout;
        setcolor(RED);
        
        outtextxy(300,300,"Ghost has eaten you.");
        
        delay(2000);
        break;}
     }
    ghostY-=ghostvy;
    ghostX+=ghostvx;
  
    for(int j=0;j<20;j++)
            {
                for(int i=0;i<20;i++)
                {
                    
                    if(ghostX+10>x[i]&&x[i+1]>ghostX+10&&ghostY+10>y[j]&&y[j+1]>ghostY+10)
                    {
                        cout<<"x[i] is "<<x[i]<<endl;
                        cout<<"rx is "<<rx<<endl;
                        cout<<"left box is "<<object[j+1][i]<<endl;
                        cout<<"right box is "<<object[j+1][i+2]<<endl;
                        cout<<"Entered ghost collision function"<<endl;
                        cout<<"y[j] is "<<y[j]<<endl;
                        cout<<"ry is "<<ry<<endl;
                        cout<<"top box is "<<object[j][i+1]<<endl;
                        cout<<"bottom box is "<<object[j+2][i+1]<<endl;

                        if(ghostX>150&&ghostX<300&&object[j+2][i+1]!=1&&ghostY-ry<-10)
                        {
                            
                            {ghostvy=-5;
                            ghostvx=0;}
                            
                        }
                        else if(ghostX>100&&ghostX<300&&object[j][i+1]!=1&&ghostY-ry>10)
                        {
                            if(ghostflagBlue==false)
                            {ghostvy=5;
                            ghostvx=0;}
                            
                        }
                        else if(((ghostX>350&&ghostY>100&&ghostY<300)||(ghostX>rx&&ghostY==ry-10))&&object[j+1][i]!=1)
                        {
                            if(ghostflagBlue==false)
                            {ghostvx=-5;
                            ghostvy=0;}
                            cout<<"rx-ghostx is "<<rx-ghostX<<endl;
                        }
                        else if((ghostX<50&&ghostY>100&&ghostY<300)||(ghostX<rx&&ghostY==ry-10)&&object[j+1][i+2]!=1)
                         {
                            if(ghostflagBlue==false)
                            {ghostvx=5;
                            ghostvy=0;}
                         }
                        
                        
                        
                        if(object[j][i+1]==1&&ghostY<y[j])
                        {
                            
                            ghostY=y[j];
                            ghostvy= 0;
                            if(object[j+1][i+2]!=1)
                            ghostvx=5;
                            else if(object[j+1][i]!=1)
                            ghostvx=-5;
                            else{ ghostvy=-5;}
                        }
                        if(object[j+2][i+1]==1&&ghostY+20>y[j+1])
                        {
                            
                            ghostY=y[j+1]-20;
                            ghostvy=0;
                            if(object[j+1][i+2]!=1)
                            ghostvx=5;
                            else if(object[j+1][i]!=1)
                            ghostvx=-5;
                            else {ghostvy=5;}
                            
                        }
                        if(object[j+1][i]==1&&ghostX<x[i])
                        {
                            
                            ghostX=x[i];
                            ghostvx=0;
                            
                            if(object[j][i+1]==1)
                            ghostvy=-5;
                            else if(object[j][i+1]!=1){
                                ghostvy=5;
                            }
                        }
                        if(object[j+1][i+2]==1&&ghostX+20>x[i+1])
                        {
                            if(object[j][i+1]==1)
                            ghostvy=-5;
                            else if(object[j][i+1]!=1){
                                ghostvy=5;
                            }
                            ghostX=x[i+1]-20;
                            ghostvx=0;
                            ghostvy=-5;
                            
                        }
                    //for Red Ghost
                        if(ghostX1+10>x[i]&&x[i+1]>ghostX1+10&&ghostY1+10>y[j]&&y[j+1]>ghostY1+10)
                    {
                        if(object[j][i+1]==1&&ghostY1<y[j])
                        {
                            
                            ghostY1=y[j];
                            ghostvy1= 0;
                        }
                        if(object[j+2][i+1]==1&&ghostY1+20>y[j+1])
                        {
                            
                            ghostY1=y[j+1]-20;
                            ghostvy1=0;
                        }
                        if(object[j+1][i]==1&&ghostX1<x[i])
                        {
                            
                            ghostX1=x[i];
                            ghostvx1=0;
                        }
                        if(object[j+1][i]==1&&ghostX1<x[i])
                        {
                            
                            ghostX1=x[i];
                            ghostvx1=0;
                        }
                    }
                        
                       
                       
                    }
                    
                    

                }
            }
        for(int j=0;j<GRID_SIZE;j++)
    {
        for(int i=0;i<GRID_SIZE;i++)
        {
            if(i==GRID_SIZE-1||j==GRID_SIZE-1)
            {}
            
            
            if(object[j][i]==1)
            {
            setfillstyle(SOLID_FILL,BLUE);
            bar(x[i],y[j],x[i]-20,y[j]-20);
            
            } 
            else{
                setfillstyle(SOLID_FILL,YELLOW);
            if(i!=0&&j!=0)
            {
                if(rx+10>x[i]-10&&x[i]>rx-10&&ry+10>y[j]-10&&y[j]-10>ry-10)
                { circlecoll[i][j]=true;
                  
                }
                
                if(circlecoll[i][j]==false)
            {circle(x[i]-10,y[j]-10,5);
            floodfill(x[i]-10,y[j]-10,WHITE);
            }
            
            
            
            }}
        }
    }
for(int i=0;i<20;i++)
{
    for(int j=0;j<20;j++)
    {
        if(circlecoll[i][j]==true)
        score++;
    }
}


    if(oldscore!=score)
    oldscore=score;
    setfillstyle(SOLID_FILL,YELLOW);
        
            if(up)
             pieslice(rx,ry,120,60,GRID_LENGTH/2);
            else if(down)
             pieslice(rx,ry,300,240,GRID_LENGTH/2);
            else if(right)
             pieslice(rx,ry,30,330,GRID_LENGTH/2);
            else if(left)
             pieslice(rx,ry,210,150,GRID_LENGTH/2);

             if(j<5)
            pieslice(rx,ry,0,0,GRID_LENGTH/2);
            
            if(circlepresent)
            {   
                
                if(counter<=5)
                {setcolor(RED);
                pieslice(330,330,360,0,10);
                setcolor(BLACK);
                
                circle(330,330,10);
                setfillstyle(1,4);
                floodfill(330,335,BLACK);
                
                }
                 else if(counter<=10){
                    
                setcolor(BLACK);
                circle(330,330,6);
                setfillstyle(1,4);
                floodfill(330,335,BLACK);
                setcolor(RED);
                pieslice(330,330,360,0,6);
                setcolor(BLACK);
                circle(330,330,6);
                setfillstyle(1,4);
                floodfill(330,335,BLACK);
                }
                if(counter==10)
                counter=0;
                counter++;
                setcolor(WHITE);
                
                
            }
            if(ghostflagBlue==false&&ghostflag==false)
            {
                if(ghostX<200)
                ghostX+=10;
                else if(ghostY<200)
                ghostY+=10;
                if(ghostX>200)
                ghostX-=10;
                else if(ghostY>200)
                ghostY-=10;
                
                if(ghostX==200&ghostY==200)
                {
                    ghostvx=0;ghostvy=0;
                }
            }
            if(rx>325&&rx<335&&ry>325&&ry<335)
            {
                savedscore=oldscore;
                ghostflag=false;
                ghostflagBlue1=true;
                ghostflagBlue=true;
                circlepresent=false;
                oldscore+=4;
                if(rx>ghostX+20)
                {
                    ghostvx=-5;
                    ghostvy=0;
                }
                else if(ry>ghostY+20)
                {
                    ghostvy=5;
                    ghostvx=0;
                }
            }
            ghostY1-=ghostvy1;
    ghostX1+=ghostvx1;
    if(ghostY1<40)
    {   ghostY1=ghostY1+5;
        
        ghostvy1=-ghostvy1;
    }
     if(ghostY1>360)
    {   ghostY1-=5;
        ghostY1-=ghostvy1;
        ghostvy1=0;
     ghostvx1=-5;}
    if(ghostX1<40)
     {  ghostX1+=5;
        ghostvx1=0;
        ghostvy1=5;
     }
    else if(ghostY1<41&&rx>ghostX1)
     {
        ghostY1+=5;
        ghostvy1=0;
        ghostvx1=5;
     }
    else if(ghostX1>360)
     {  ghostX1-=5;
        ghostvx1=0;
        ghostvy1=-5;
     }
     if(ghostX1+40<rx+20&&ghostX1+40>rx-20&&ghostY1+40<ry+20&&ghostY1+40>ry-20)
     {
        ghostvy1=0;
        ghostvx1=5;
     }
     if(ghostX1-20<rx+20&&ghostX1-20>rx-20&&ghostY1-20<ry+20&&ghostY1-20>ry-20)
     {
        ghostvy1=0;
        ghostvx1=-5;
     }
     if(oldscore==savedscore+20)
     {
        ghostflag=true;
        ghostflagRed=true;
        ghostflagBlue=false;
        ghostflagBlue1=false;
        circlepresent=true;
     }
     
     setrgbpalette(8,0,191,255);
     rectangle(500,500,550,550);
     setfillstyle(1,8);
     floodfill(520,520,WHITE);
            if(ghostflag==true)
            drawGhost(ghostX,ghostY);
            if(ghostflagBlue==true)
            drawGhostBlue(ghostX, ghostY);
            if(ghostflagBlue==false&&ghostflag==false)
            drawGhostBlack(ghostX,ghostY);
            if(ghostflagRed==true)
            drawGhostRed(ghostX1,ghostY1);
            if(ghostflagBlue1==true)
            drawGhostBlue(ghostX1,ghostY1);
            drawGhostLightBlue(200,220);
            drawGhostYellow(180,200);
            //drawGhostBrown(200,200);
            //drawGhostWhite(200,200);
            drawGhostGreen(220,200);
            if(score==100)
            {
                ghostflag=true;
                ghostX=200;
                ghostY=200;
                circlepresent=true;
            }
            if(score==150)
            {
                ghostflag=true;
                for(int i=40;i<80;i+=20)
                {
                    if(getpixel(rx+i,ry+10)!=BLUE)
                {
                    ghostX=rx-10+i;
                    break;
                }
                }
                ghostY=ry-10;
                circlepresent=true;
            }
            if(score==200)
            {
                ghostflag=true;
                ghostX=rx+40;
                ghostY=ry;
                circlepresent=true;
            }  
        std::ostringstream bgiout;
        bgiout<<"Highscore: "<<highscore;
        outstream(bgiout,200,400);
        bgiout.clear();
        std::ostringstream another;
        another<<"Score:"<< oldscore;
        outstream(another,400,400);

        if(oldscore>highscore)
        {
            highscore=oldscore;
            std::ofstream outfile("highscore-list.txt");
            if(outfile.is_open())
            {
                outfile<<oldscore;
                outfile.close();
            }
            else{
                std::cerr << "Error opening file for writing.\n";
            }
        }
        if(oldscore==175)
        {
            setvisualpage(page);
            setcolor(YELLOW);
            outtextxy(400,400,"GAME OVER!!.");
            delay(2000);
            break;
        }
        score=0;

        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);
        auto millisecondsCount= frameDuration.count();
        cout<<millisecondsCount<<endl;
         auto sleepTime = frameTime - frameDuration;
        if (sleepTime > std::chrono::milliseconds(0)) {
           int sleepTimeMs = static_cast<int>(sleepTime.count());
           cout<<sleepTimeMs;
           if(GetAsyncKeyState(VK_SPACE))
                delay(10);
            else{delay(sleepTimeMs);}
        }
        
        
        page=1-page;
        
        
    }

}
void outstream(ostringstream& out,int x, int y) {
    char* textCopy = strdup(out.str().c_str());
    settextstyle(SANS_SERIF_FONT,HORIZ_DIR,2);
    outtextxy(x, y, textCopy);
    free(textCopy); // Free the allocated memory
}

void drawGhost(int x,int y)
{   
    
    
    for(int i=4;i<14;i++)
    putpixel(x+i,y,MAGENTA);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+1,MAGENTA);
    for(int i=2;i<16;i++)
    putpixel(x+i,y+2,MAGENTA);
    for(int i=2;i<16;i++)
    putpixel(x+i,y+3,MAGENTA);
    for(int i=4;i<20;i++)
    putpixel(x,y+i,MAGENTA);
    for(int i=4;i<20;i++)
    putpixel(x+1,y+i,MAGENTA);
    for(int i=4;i<20;i++)
    putpixel(x+2,y+i,MAGENTA);
    for(int i=4;i<20;i++)
    putpixel(x+3,y+i,MAGENTA);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+4,BLACK);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+5,BLACK);
    for(int i=14;i<18;i++)
    putpixel(x+i,y+5,MAGENTA);
    putpixel(x+4,y+6,BLACK);
    putpixel(x+5,y+6,BLACK);
    putpixel(x+6,y+6,LIGHTBLUE);
    putpixel(x+7,y+6,LIGHTBLUE);
    putpixel(x+8,y+6,BLACK);
    putpixel(x+9,y+6,BLACK);
    putpixel(x+10,y+6,LIGHTBLUE);
    putpixel(x+11,y+6,LIGHTBLUE);
    putpixel(x+12,y+6,BLACK);
    putpixel(x+13,y+6,BLACK);
for(int i=14;i<18;i++)
    putpixel(x+i,y+6,MAGENTA);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+7,BLACK);
    for(int i=14;i<18;i++)
    putpixel(x+i,y+7,MAGENTA);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+8,MAGENTA);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+9,MAGENTA);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+10,MAGENTA);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+11,MAGENTA);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+12,MAGENTA);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+13,MAGENTA);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+14,MAGENTA);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+15,MAGENTA);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+16,MAGENTA);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+17,MAGENTA);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+18,MAGENTA);
    for(int i=0;i<5;i++)
    putpixel(x+i,y+19,MAGENTA);
    for(int i=0;i<5;i++)
    putpixel(x+i,y+20,MAGENTA);
    for(int i=8;i<13;i++)
    putpixel(x+i,y+19,MAGENTA);
    for(int i=8;i<13;i++)
    putpixel(x+i,y+20,MAGENTA);
    for(int i=15;i<19;i++)
    putpixel(x+i,y+19,MAGENTA);
    for(int i=15;i<19;i++)
    putpixel(x+i,y+20,MAGENTA);
   
}
void drawGhostBlue(int x,int y)
{
    for(int i=4;i<14;i++)
    putpixel(x+i,y,BLUE);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+1,BLUE);
    for(int i=2;i<16;i++)
    putpixel(x+i,y+2,BLUE);
    for(int i=2;i<16;i++)
    putpixel(x+i,y+3,BLUE);
    for(int i=4;i<20;i++)
    putpixel(x,y+i,BLUE);
    for(int i=4;i<20;i++)
    putpixel(x+1,y+i,BLUE);
    for(int i=4;i<20;i++)
    putpixel(x+2,y+i,BLUE);
    for(int i=4;i<20;i++)
    putpixel(x+3,y+i,BLUE);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+4,BLACK);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+5,BLACK);
    for(int i=14;i<18;i++)
    putpixel(x+i,y+5,BLUE);
    putpixel(x+4,y+6,BLACK);
    putpixel(x+5,y+6,BLACK);
    putpixel(x+6,y+6,LIGHTBLUE);
    putpixel(x+7,y+6,LIGHTBLUE);
    putpixel(x+8,y+6,BLACK);
    putpixel(x+9,y+6,BLACK);
    putpixel(x+10,y+6,LIGHTBLUE);
    putpixel(x+11,y+6,LIGHTBLUE);
    putpixel(x+12,y+6,BLACK);
    putpixel(x+13,y+6,BLACK);
for(int i=14;i<18;i++)
    putpixel(x+i,y+6,BLUE);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+7,BLACK);
    for(int i=14;i<18;i++)
    putpixel(x+i,y+7,BLUE);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+8,BLUE);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+9,BLUE);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+10,BLUE);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+11,BLUE);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+12,BLUE);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+13,BLUE);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+14,BLUE);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+15,BLUE);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+16,BLUE);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+17,BLUE);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+18,BLUE);
    for(int i=0;i<5;i++)
    putpixel(x+i,y+19,BLUE);
    for(int i=0;i<5;i++)
    putpixel(x+i,y+20,BLUE);
    for(int i=8;i<13;i++)
    putpixel(x+i,y+19,BLUE);
    for(int i=8;i<13;i++)
    putpixel(x+i,y+20,BLUE);
    for(int i=15;i<19;i++)
    putpixel(x+i,y+19,BLUE);
    for(int i=15;i<19;i++)
    putpixel(x+i,y+20,BLUE);
}

void drawGhostBlack(int x,int y)
{
   for(int i=4;i<14;i++)
    putpixel(x+i,y,BLACK);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+1,BLACK);
    for(int i=2;i<16;i++)
    putpixel(x+i,y+2,BLACK);
    for(int i=2;i<16;i++)
    putpixel(x+i,y+3,BLACK);
    for(int i=4;i<20;i++)
    putpixel(x,y+i,BLACK);
    for(int i=4;i<20;i++)
    putpixel(x+1,y+i,BLACK);
    for(int i=4;i<20;i++)
    putpixel(x+2,y+i,BLACK);
    for(int i=4;i<20;i++)
    putpixel(x+3,y+i,BLACK);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+4,BLACK);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+5,BLACK);
    for(int i=14;i<18;i++)
    putpixel(x+i,y+5,BLACK);
    putpixel(x+4,y+6,BLACK);
    putpixel(x+5,y+6,BLACK);
    putpixel(x+6,y+6,LIGHTBLUE);
    putpixel(x+7,y+6,LIGHTBLUE);
    putpixel(x+8,y+6,BLACK);
    putpixel(x+9,y+6,BLACK);
    putpixel(x+10,y+6,LIGHTBLUE);
    putpixel(x+11,y+6,LIGHTBLUE);
    putpixel(x+12,y+6,BLACK);
    putpixel(x+13,y+6,BLACK);
for(int i=14;i<18;i++)
    putpixel(x+i,y+6,BLACK);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+7,BLACK);
    for(int i=14;i<18;i++)
    putpixel(x+i,y+7,BLACK);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+8,BLACK);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+9,BLACK);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+10,BLACK);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+11,BLACK);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+12,BLACK);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+13,BLACK);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+14,BLACK);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+15,BLACK);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+16,BLACK);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+17,BLACK);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+18,BLACK);
    for(int i=0;i<5;i++)
    putpixel(x+i,y+19,BLACK);
    for(int i=0;i<5;i++)
    putpixel(x+i,y+20,BLACK);
    for(int i=8;i<13;i++)
    putpixel(x+i,y+19,BLACK);
    for(int i=8;i<13;i++)
    putpixel(x+i,y+20,BLACK);
    for(int i=15;i<19;i++)
    putpixel(x+i,y+19,BLACK);
    for(int i=15;i<19;i++)
    putpixel(x+i,y+20,BLACK);
   
   
}
void drawGhostRed(int x,int y)
{
    for(int i=4;i<14;i++)
    putpixel(x+i,y,RED);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+1,RED);
    for(int i=2;i<16;i++)
    putpixel(x+i,y+2,RED);
    for(int i=2;i<16;i++)
    putpixel(x+i,y+3,RED);
    for(int i=4;i<20;i++)
    putpixel(x,y+i,RED);
    for(int i=4;i<20;i++)
    putpixel(x+1,y+i,RED);
    for(int i=4;i<20;i++)
    putpixel(x+2,y+i,RED);
    for(int i=4;i<20;i++)
    putpixel(x+3,y+i,RED);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+4,BLACK);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+5,BLACK);
    for(int i=14;i<18;i++)
    putpixel(x+i,y+5,RED);
    putpixel(x+4,y+6,BLACK);
    putpixel(x+5,y+6,BLACK);
    putpixel(x+6,y+6,LIGHTBLUE);
    putpixel(x+7,y+6,LIGHTBLUE);
    putpixel(x+8,y+6,BLACK);
    putpixel(x+9,y+6,BLACK);
    putpixel(x+10,y+6,LIGHTBLUE);
    putpixel(x+11,y+6,LIGHTBLUE);
    putpixel(x+12,y+6,BLACK);
    putpixel(x+13,y+6,BLACK);
for(int i=14;i<18;i++)
    putpixel(x+i,y+6,RED);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+7,BLACK);
    for(int i=14;i<18;i++)
    putpixel(x+i,y+7,RED);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+8,RED);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+9,RED);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+10,RED);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+11,RED);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+12,RED);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+13,RED);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+14,RED);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+15,RED);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+16,RED);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+17,RED);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+18,RED);
    for(int i=0;i<5;i++)
    putpixel(x+i,y+19,RED);
    for(int i=0;i<5;i++)
    putpixel(x+i,y+20,RED);
    for(int i=8;i<13;i++)
    putpixel(x+i,y+19,RED);
    for(int i=8;i<13;i++)
    putpixel(x+i,y+20,RED);
    for(int i=15;i<19;i++)
    putpixel(x+i,y+19,RED);
    for(int i=15;i<19;i++)
    putpixel(x+i,y+20,RED);
   
}
void drawGhostLightBlue(int x, int y)
{
    for(int i=4;i<14;i++)
    putpixel(x+i,y,8);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+1,8);
    for(int i=2;i<16;i++)
    putpixel(x+i,y+2,8);
    for(int i=2;i<16;i++)
    putpixel(x+i,y+3,8);
    for(int i=4;i<20;i++)
    putpixel(x,y+i,8);
    for(int i=4;i<20;i++)
    putpixel(x+1,y+i,8);
    for(int i=4;i<20;i++)
    putpixel(x+2,y+i,8);
    for(int i=4;i<20;i++)
    putpixel(x+3,y+i,8);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+4,BLACK);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+5,BLACK);
    for(int i=14;i<18;i++)
    putpixel(x+i,y+5,8);
    putpixel(x+4,y+6,BLACK);
    putpixel(x+5,y+6,BLACK);
    putpixel(x+6,y+6,LIGHTBLUE);
    putpixel(x+7,y+6,LIGHTBLUE);
    putpixel(x+8,y+6,BLACK);
    putpixel(x+9,y+6,BLACK);
    putpixel(x+10,y+6,LIGHTBLUE);
    putpixel(x+11,y+6,LIGHTBLUE);
    putpixel(x+12,y+6,BLACK);
    putpixel(x+13,y+6,BLACK);
for(int i=14;i<18;i++)
    putpixel(x+i,y+6,8);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+7,BLACK);
    for(int i=14;i<18;i++)
    putpixel(x+i,y+7,8);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+8,8);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+9,8);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+10,8);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+11,8);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+12,8);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+13,8);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+14,8);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+15,8);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+16,8);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+17,8);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+18,8);
    for(int i=0;i<5;i++)
    putpixel(x+i,y+19,8);
    for(int i=0;i<5;i++)
    putpixel(x+i,y+20,8);
    for(int i=8;i<13;i++)
    putpixel(x+i,y+19,8);
    for(int i=8;i<13;i++)
    putpixel(x+i,y+20,8);
    for(int i=15;i<19;i++)
    putpixel(x+i,y+19,8);
    for(int i=15;i<19;i++)
    putpixel(x+i,y+20,8);
   
   
}
void drawGhostWhite(int x,int y)
{
   for(int i=4;i<14;i++)
    putpixel(x+i,y,WHITE);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+1,WHITE);
    for(int i=2;i<16;i++)
    putpixel(x+i,y+2,WHITE);
    for(int i=2;i<16;i++)
    putpixel(x+i,y+3,WHITE);
    for(int i=4;i<20;i++)
    putpixel(x,y+i,WHITE);
    for(int i=4;i<20;i++)
    putpixel(x+1,y+i,WHITE);
    for(int i=4;i<20;i++)
    putpixel(x+2,y+i,WHITE);
    for(int i=4;i<20;i++)
    putpixel(x+3,y+i,WHITE);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+4,BLACK);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+5,BLACK);
    for(int i=14;i<18;i++)
    putpixel(x+i,y+5,WHITE);
    putpixel(x+4,y+6,BLACK);
    putpixel(x+5,y+6,BLACK);
    putpixel(x+6,y+6,LIGHTBLUE);
    putpixel(x+7,y+6,LIGHTBLUE);
    putpixel(x+8,y+6,BLACK);
    putpixel(x+9,y+6,BLACK);
    putpixel(x+10,y+6,LIGHTBLUE);
    putpixel(x+11,y+6,LIGHTBLUE);
    putpixel(x+12,y+6,BLACK);
    putpixel(x+13,y+6,BLACK);
for(int i=14;i<18;i++)
    putpixel(x+i,y+6,WHITE);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+7,BLACK);
    for(int i=14;i<18;i++)
    putpixel(x+i,y+7,WHITE);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+8,WHITE);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+9,WHITE);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+10,WHITE);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+11,WHITE);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+12,WHITE);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+13,WHITE);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+14,WHITE);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+15,WHITE);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+16,WHITE);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+17,WHITE);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+18,WHITE);
    for(int i=0;i<5;i++)
    putpixel(x+i,y+19,WHITE);
    for(int i=0;i<5;i++)
    putpixel(x+i,y+20,WHITE);
    for(int i=8;i<13;i++)
    putpixel(x+i,y+19,WHITE);
    for(int i=8;i<13;i++)
    putpixel(x+i,y+20,WHITE);
    for(int i=15;i<19;i++)
    putpixel(x+i,y+19,WHITE);
    for(int i=15;i<19;i++)
    putpixel(x+i,y+20,WHITE);
   
   
   
   
}

void drawGhostGreen(int x,int y)
{
    for(int i=4;i<14;i++)
    putpixel(x+i,y,GREEN);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+1,GREEN);
    for(int i=2;i<16;i++)
    putpixel(x+i,y+2,GREEN);
    for(int i=2;i<16;i++)
    putpixel(x+i,y+3,GREEN);
    for(int i=4;i<20;i++)
    putpixel(x,y+i,GREEN);
    for(int i=4;i<20;i++)
    putpixel(x+1,y+i,GREEN);
    for(int i=4;i<20;i++)
    putpixel(x+2,y+i,GREEN);
    for(int i=4;i<20;i++)
    putpixel(x+3,y+i,GREEN);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+4,BLACK);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+5,BLACK);
    for(int i=14;i<18;i++)
    putpixel(x+i,y+5,GREEN);
    putpixel(x+4,y+6,BLACK);
    putpixel(x+5,y+6,BLACK);
    putpixel(x+6,y+6,LIGHTBLUE);
    putpixel(x+7,y+6,LIGHTBLUE);
    putpixel(x+8,y+6,BLACK);
    putpixel(x+9,y+6,BLACK);
    putpixel(x+10,y+6,LIGHTBLUE);
    putpixel(x+11,y+6,LIGHTBLUE);
    putpixel(x+12,y+6,BLACK);
    putpixel(x+13,y+6,BLACK);
for(int i=14;i<18;i++)
    putpixel(x+i,y+6,GREEN);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+7,BLACK);
    for(int i=14;i<18;i++)
    putpixel(x+i,y+7,GREEN);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+8,GREEN);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+9,GREEN);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+10,GREEN);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+11,GREEN);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+12,GREEN);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+13,GREEN);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+14,GREEN);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+15,GREEN);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+16,GREEN);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+17,GREEN);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+18,GREEN);
    for(int i=0;i<5;i++)
    putpixel(x+i,y+19,GREEN);
    for(int i=0;i<5;i++)
    putpixel(x+i,y+20,GREEN);
    for(int i=8;i<13;i++)
    putpixel(x+i,y+19,GREEN);
    for(int i=8;i<13;i++)
    putpixel(x+i,y+20,GREEN);
    for(int i=15;i<19;i++)
    putpixel(x+i,y+19,GREEN);
    for(int i=15;i<19;i++)
    putpixel(x+i,y+20,GREEN);
   
}
void drawGhostYellow(int x,int y)
{
    for(int i=4;i<14;i++)
    putpixel(x+i,y,YELLOW);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+1,YELLOW);
    for(int i=2;i<16;i++)
    putpixel(x+i,y+2,YELLOW);
    for(int i=2;i<16;i++)
    putpixel(x+i,y+3,YELLOW);
    for(int i=4;i<20;i++)
    putpixel(x,y+i,YELLOW);
    for(int i=4;i<20;i++)
    putpixel(x+1,y+i,YELLOW);
    for(int i=4;i<20;i++)
    putpixel(x+2,y+i,YELLOW);
    for(int i=4;i<20;i++)
    putpixel(x+3,y+i,YELLOW);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+4,BLACK);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+5,BLACK);
    for(int i=14;i<18;i++)
    putpixel(x+i,y+5,YELLOW);
    putpixel(x+4,y+6,BLACK);
    putpixel(x+5,y+6,BLACK);
    putpixel(x+6,y+6,LIGHTBLUE);
    putpixel(x+7,y+6,LIGHTBLUE);
    putpixel(x+8,y+6,BLACK);
    putpixel(x+9,y+6,BLACK);
    putpixel(x+10,y+6,LIGHTBLUE);
    putpixel(x+11,y+6,LIGHTBLUE);
    putpixel(x+12,y+6,BLACK);
    putpixel(x+13,y+6,BLACK);
for(int i=14;i<18;i++)
    putpixel(x+i,y+6,YELLOW);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+7,BLACK);
    for(int i=14;i<18;i++)
    putpixel(x+i,y+7,YELLOW);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+8,YELLOW);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+9,YELLOW);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+10,YELLOW);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+11,YELLOW);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+12,YELLOW);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+13,YELLOW);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+14,YELLOW);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+15,YELLOW);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+16,YELLOW);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+17,YELLOW);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+18,YELLOW);
    for(int i=0;i<5;i++)
    putpixel(x+i,y+19,YELLOW);
    for(int i=0;i<5;i++)
    putpixel(x+i,y+20,YELLOW);
    for(int i=8;i<13;i++)
    putpixel(x+i,y+19,YELLOW);
    for(int i=8;i<13;i++)
    putpixel(x+i,y+20,YELLOW);
    for(int i=15;i<19;i++)
    putpixel(x+i,y+19,YELLOW);
    for(int i=15;i<19;i++)
    putpixel(x+i,y+20,YELLOW);
   
}
void drawGhostBrown(int x,int y)
{
    for(int i=4;i<14;i++)
    putpixel(x+i,y,BROWN);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+1,BROWN);
    for(int i=2;i<16;i++)
    putpixel(x+i,y+2,BROWN);
    for(int i=2;i<16;i++)
    putpixel(x+i,y+3,BROWN);
    for(int i=4;i<20;i++)
    putpixel(x,y+i,BROWN);
    for(int i=4;i<20;i++)
    putpixel(x+1,y+i,BROWN);
    for(int i=4;i<20;i++)
    putpixel(x+2,y+i,BROWN);
    for(int i=4;i<20;i++)
    putpixel(x+3,y+i,BROWN);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+4,BLACK);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+5,BLACK);
    for(int i=14;i<18;i++)
    putpixel(x+i,y+5,BROWN);
    putpixel(x+4,y+6,BLACK);
    putpixel(x+5,y+6,BLACK);
    putpixel(x+6,y+6,LIGHTBLUE);
    putpixel(x+7,y+6,LIGHTBLUE);
    putpixel(x+8,y+6,BLACK);
    putpixel(x+9,y+6,BLACK);
    putpixel(x+10,y+6,LIGHTBLUE);
    putpixel(x+11,y+6,LIGHTBLUE);
    putpixel(x+12,y+6,BLACK);
    putpixel(x+13,y+6,BLACK);
for(int i=14;i<18;i++)
    putpixel(x+i,y+6,BROWN);
    for(int i=4;i<14;i++)
    putpixel(x+i,y+7,BLACK);
    for(int i=14;i<18;i++)
    putpixel(x+i,y+7,BROWN);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+8,BROWN);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+9,BROWN);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+10,BROWN);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+11,BROWN);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+12,BROWN);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+13,BROWN);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+14,BROWN);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+15,BROWN);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+16,BROWN);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+17,BROWN);
    for(int i=0;i<19;i++)
    putpixel(x+i,y+18,BROWN);
    for(int i=0;i<5;i++)
    putpixel(x+i,y+19,BROWN);
    for(int i=0;i<5;i++)
    putpixel(x+i,y+20,BROWN);
    for(int i=8;i<13;i++)
    putpixel(x+i,y+19,BROWN);
    for(int i=8;i<13;i++)
    putpixel(x+i,y+20,BROWN);
    for(int i=15;i<19;i++)
    putpixel(x+i,y+19,BROWN);
    for(int i=15;i<19;i++)
    putpixel(x+i,y+20,BROWN);
   
}