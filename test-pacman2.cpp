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
#include <vector>
#include <list>
#include <math.h>
#include <fstream>
using namespace std;

int death=0;
int rx= 30;
int ry= 30;
int ghostX=200; int ghostY=200;
bool Relaxed;
struct sNode
{
    bool bObstacle = false;
    bool bVisited = false;
    float fGlobalGoal;
    float fLocalGoal;
    int x;
    int y;
    std::vector<sNode*> vecNeighbours;
    sNode* parent;

};

void Solve_AStar();

sNode *nodes = nullptr;
int nMapWidth= 20;
int nMapHeight= 20;

sNode *nodeStart = nullptr;
sNode *nodeEnd = nullptr;


bool OnUserUpdate(float fElapsedTime, int rx, int ry, int ghostX, int ghostY,bool left, bool right, bool up, bool down, int color, bool Relaxed)
{
    int nNodeSize = 20;
    int nNodeBorder = 1;
    
   int nSelectedNodeX = mousex() / nNodeSize;
    int nSelectedNodeY = mousey() / nNodeSize;
    int x=mousex();
    int y=mousey();
    setcolor(WHITE);
    
        if(nSelectedNodeX >= 0 && nSelectedNodeX < nMapWidth)
            if(nSelectedNodeY >=0 && nSelectedNodeY < nMapHeight)
            {
                
                
                if(color==4)
                    nodeStart = &nodes[(ry/nNodeSize) * nMapWidth + rx/nNodeSize];
                if(up==true&&color==5&&ry<300)
                    nodeStart = &nodes[(ry/nNodeSize-2) * nMapWidth + rx/nNodeSize];
                if(down==true&&color==5&&ry<300)
                    nodeStart = &nodes[(ry/nNodeSize+2) * nMapWidth + rx/nNodeSize];
                if(left==true&&color==5&&ry<300)
                    nodeStart = &nodes[ry/nNodeSize * nMapWidth + rx/nNodeSize-2];
                if(right==true&&color==5&&ry<300)
                    nodeStart = &nodes[ry/nNodeSize * nMapWidth + rx/nNodeSize+2];
                    nodeEnd = &nodes[ghostY/nNodeSize * nMapWidth + ghostX/nNodeSize];
                
                
                

                Solve_AStar();
            }
    
    
   


    for(int x =0; x<nMapWidth; x++)
        for(int y =0;y<nMapHeight; y++)
        {
            
            if(getpixel(x*20,y*20)==BLUE)
            nodes[y * nMapWidth + x].bObstacle=true;
            
        }


    if(nodeEnd != nullptr)
        {
            sNode *p = nodeEnd;
            while (p->parent != nullptr)
            {
                setcolor(color);
                setlinestyle(SOLID_LINE,1,3);
                line(10 ,10, 100,100);
                line(p->x*nNodeSize + nNodeSize/2,p->y*nNodeSize + nNodeSize / 2,
                p->parent->x*nNodeSize + nNodeSize / 2, p->parent->y*nNodeSize + nNodeSize / 2 );

                //Set next node to this node's parent
                p = p->parent;
            }
        }
    setcolor(YELLOW);
    setlinestyle(SOLID_LINE,1,1);
    return true;
}
bool OnUserUpdate(float fElapsedTime, int ghostX, int ghostY)
{
    int nNodeSize = 20;
    int nNodeBorder = 1;
    
   int nSelectedNodeX = mousex() / nNodeSize;
    int nSelectedNodeY = mousey() / nNodeSize;
    int x=mousex();
    int y=mousey();
    setcolor(WHITE);
    
        if(nSelectedNodeX >= 0 && nSelectedNodeX < nMapWidth)
            if(nSelectedNodeY >=0 && nSelectedNodeY < nMapHeight)
            {
                
                
                
                    nodeStart = &nodes[ghostY/nNodeSize * nMapWidth + ghostX/nNodeSize];
                    nodeEnd = &nodes[40/nNodeSize * nMapWidth + 360/nNodeSize];
                

                Solve_AStar();
            }
    
    
   


    for(int x =0; x<nMapWidth; x++)
        for(int y =0;y<nMapHeight; y++)
        {
            
            if(getpixel(x*20,y*20)==BLUE)
            nodes[y * nMapWidth + x].bObstacle=true;
            
        }


    if(nodeEnd != nullptr)
        {
            sNode *p = nodeEnd;
            while (p->parent != nullptr)
            {
                setcolor(RED);
                setlinestyle(SOLID_LINE,1,3);
                line(10 ,10, 100,100);
                line(p->x*nNodeSize + nNodeSize/2,p->y*nNodeSize + nNodeSize / 2,
                p->parent->x*nNodeSize + nNodeSize / 2, p->parent->y*nNodeSize + nNodeSize / 2 );

                //Set next node to this node's parent
                p = p->parent;
            }
        }
    setcolor(YELLOW);
    setlinestyle(SOLID_LINE,1,1);
    return true;
}

void Solve_AStar()
{
    // Reset Navigation Graph - default all node states
    for( int x=0;x<nMapWidth; x++)
        for(int y = 0; y<nMapHeight; y++)
        {
            nodes[y*nMapWidth + x].bVisited = false;
            nodes[y*nMapWidth + x].fGlobalGoal= INFINITY ;
            nodes[y*nMapWidth + x].fLocalGoal= INFINITY ;
            nodes[y*nMapWidth + x].parent = nullptr;
        }

    auto distance = [](sNode* a, sNode* b)
    {
        return sqrtf((a->x - b->x)*(a->x - b->x) + (a->y - b->y)*(a->y - b->y));
    };
// So we can experiment with heuristic
    auto heuristic = [distance](sNode* a, sNode* b) 
    {
        return distance(a,b);
    };

    // Setup starting conditions
    sNode *nodeCurrent = nodeStart;
    nodeStart->fLocalGoal = 0.0f;
    nodeStart->fGlobalGoal = heuristic(nodeStart,nodeEnd);

    std::list<sNode*> listNotTestedNodes;
    listNotTestedNodes.push_back(nodeStart);

    while(!listNotTestedNodes.empty() && nodeCurrent != nodeEnd)
    {
        listNotTestedNodes.sort([](const sNode* lhs, const sNode* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

        while(!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
            listNotTestedNodes.pop_front();

            //...or abort because there are no valid nodes left to test
            if (listNotTestedNodes.empty())
                break;
            
            nodeCurrent = listNotTestedNodes.front();
            nodeCurrent->bVisited = true; // We only explore a node once

            // Check each of this node's neighbours...
            for( auto nodeNeighbour : nodeCurrent->vecNeighbours)
            {
                if(!nodeNeighbour->bVisited && nodeNeighbour->bObstacle == 0)
                    listNotTestedNodes.push_back(nodeNeighbour);

                //Calculate the neighbours potential lowest parent distance
                float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbour);

                //If choosing to path through this node is a lower distance than what 
                // the neighbour currently has set, update the neighbour to use this node
                // as the path source, and set its distance scores as necessary
                if( fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)
                {
                    nodeNeighbour->parent = nodeCurrent;
                    nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;

                    //The best path length to the neighbour being tested has changed, so 
                    // update the neighbour's score. The heuristic is used to globally bias 
                    // the oath algorithm, so it knows it its getting better or worse, At some
                    // point the algo will realise this path is worse and abandon it, and then go 
                    // and search along the next best path.
                    nodeNeighbour->fGlobalGoal =  nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, nodeEnd);
                }
            }
    }
}

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
                setlinestyle(SOLID_LINE,1,1);
            circle(x[i]-10,y[j]-10,5);
            floodfill(x[i]-10,y[j]-10,WHITE);
            }
        }
    }
}


void drawGhost(int x,int y);
void drawGhostBlue(int x, int y);
void drawGhostBlack(int x,int y);
void drawGhostRed(int x, int y);
int ghostRedX=0; int ghostRedY=0;
bool ghostflagBlue=false;
int ghostvy=0; int ghostvx=0;
int ghostRedvx=0; int ghostRedvy=0;
int highscore=0;
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
     std::ifstream file("deaths.txt");
    if(file.is_open()){
        std::string line;
        while(std::getline(file,line)){
            cout<<"Deaths: "<<line<<endl;
            death=stoi(line);
        }
        file.close();
    }
    else{
        std::cerr << "Error opening file for reading.\n";
    }
cout<<highscore;
    srand(time(NULL));
    
    int ghostRedX=300;
    int ghostRedY=300;
    nodes= new sNode[nMapWidth * nMapHeight];
    for(int x=0; x<nMapWidth ;x++)
    {
        for(int y=0; y<nMapHeight;y++)
        {
            
            nodes[y * nMapWidth + x].x = x;
            nodes[y * nMapWidth + x].y = y;
            nodes[y * nMapWidth + x].bObstacle = false;
            nodes[y * nMapWidth + x].parent = nullptr;
            nodes[y * nMapWidth + x].bVisited = false;
        }
    }

    //Create connections - in this case nodes are on a regular grid
    for (int x = 0;x< nMapWidth; x++)
        for(int y=0;y<nMapHeight; y++)
        {
            if(y>0)
                nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x + 0)]);
            if(y<nMapHeight - 1)
                nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x + 0)]);
            if(x>0)
                nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMapWidth + (x - 1)]);
            if(x<nMapWidth - 1)
                nodes[y*nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMapWidth + (x + 1)]);
        }
    nodeStart = &nodes[(nMapHeight / 2) * nMapWidth + 1];
    nodeEnd = &nodes[(nMapHeight / 2) * nMapWidth + nMapWidth - 2];

    const int targetFPS = 60;
    const std::chrono::milliseconds frameTime(3500 / targetFPS);

    
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
    int vx=0;int vy=0;
    int score=0; int oldscore=0;
    bool circlepresent=true;
    bool ghostflag=true;
    bool ghostFlagRed=true;
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
         if(ghostX+10>rx-10&&ghostX<rx+10&&ghostY+20>ry-10&&ghostY<ry+10||ghostRedX+10>rx-10&&ghostRedX<rx+10&&ghostRedY+20>ry-10&&ghostRedY<ry+10)
     {
        if(circlepresent==false)
        {
            ghostflag=false;
            ghostflagBlue=false;
        }
        else{
        setvisualpage(page);
        
        for(int i=20;i<360;i++)
        {
            
            setcolor(YELLOW);
            setfillstyle(1,YELLOW);
            if(up&&i<290)
            {
                pieslice(rx,ry,120+i,60,10);
            }
            if(down&&i<290)
            {
                pieslice(rx,ry,300+i,240,10);
            }
            if(right&&30+i<=310)
            {
                pieslice(rx,ry,30+i,330,10);
            }
            if(left&&i<290)
            {
                pieslice(rx,ry,210,150-i,10);
            }               
            
            cleardevice();
             /*,120,60,
                            ,300,240
                            ,30,330,
                            ,210,150*/
            }
        
        
        cleardevice();
        std::ostringstream bgiout;
        setcolor(RED);
        
        outtextxy(300,300,"Ghost has eaten you.");
        std::ofstream outfile("deaths.txt");
        if(outfile.is_open())
        {
            death++;
            outfile<<death<<endl;
            outfile.close();
        }
        else{
            std::cerr<<"Error opening file. /n";
        }
        delay(2000);
        break;
        }
     }
    
    
    
    
    for(int j=0;j<20;j++)
            {
                for(int i=0;i<20;i++)
                {
                    
                    if(ghostX+10>x[i]&&x[i+1]>ghostX+10&&ghostY+10>y[j]&&y[j+1]>ghostY+10)
                    {
                        

                        
                        
                        
                        
                        if(object[j][i+1]==1&&ghostY<y[j])
                        {
                            
                            ghostY=y[j];
                            
                        }
                        if(object[j+2][i+1]==1&&ghostY+20>y[j+1])
                        {
                            
                            ghostY=y[j+1]-20;
                            
                            
                        }
                        if(object[j+1][i]==1&&ghostX<x[i])
                        {
                            
                            ghostX=x[i];
                            
                        }
                        if(object[j+1][i+2]==1&&ghostX+20>x[i+1])
                        {
                            
                            ghostX=x[i+1]-20;
                            
                            
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
            {   setcolor(YELLOW);
                pieslice(x[i]-10,y[j]-10,0,360,5);
            
            }
            
            
            
            }}
        }
    }
            
        
        OnUserUpdate(10,rx,ry,ghostX,ghostY,left,right,up,down,5,Relaxed);
        
        setcolor(YELLOW);
        //OnUserUpdate(float fElapsedTime, int rx, int ry, int ghostX, int ghostY,bool left, bool right, bool up, bool down)
        

    
      if(getpixel(ghostX+22,ghostY+10)==5)
        {ghostvx=5;
         ghostvy=0;}
         
    else if(getpixel(ghostX-3,ghostY+10)==5&&ghostvx!=5)
        {ghostvx=-5;
        ghostvy=0;}
    else if(getpixel(ghostX+10,ghostY+22)==5)
        {ghostvx=0;
         ghostvy=5;}
    else if(getpixel(ghostX+10,ghostY-1)==5&&ghostvy!=5)
        {ghostvx=0;
         ghostvy=-5;}
    
    
    if(oldscore<20||(oldscore>50&&oldscore<60))
       {
            Relaxed=true;
            if(Relaxed==true)
            {
                setcolor(RED);
                setlinestyle(SOLID_LINE,1,3);
                rectangle(210,50,370,90);
                setlinestyle(SOLID_LINE,1,1);
                if(ghostRedY>50||ghostRedX<210)
                OnUserUpdate(10, ghostRedX, ghostRedY);
                setcolor(YELLOW);
            }
       }
    else{
        Relaxed=false;
        OnUserUpdate(10, rx, ry, ghostRedX, ghostRedY, left, right, up, down,4,Relaxed);
    } 

    if(getpixel(ghostRedX-2,ghostRedY+10)==4&&ghostRedvx!=5)
        {ghostRedvx=-5;
        ghostRedvy=0;}
    else if(getpixel(ghostRedX+10,ghostRedY+23)==4&&ghostRedvy!=-5)
        {ghostRedvx=0;
         ghostRedvy=5;}
    else if(getpixel(ghostRedX+10,ghostRedY-1)==4&&ghostRedvy!=5)
        {ghostRedvx=0;
         ghostRedvy=-5;}
    else if(getpixel(ghostRedX+22,ghostRedY+10)==4&&ghostRedvx!=-5)
        {ghostRedvx=5;
         ghostRedvy=0;}
    
    
    else if(ghostRedX>360||ghostRedX<40||ghostRedY>360||ghostRedY<40)
        {
        ghostRedvx=0;
        ghostRedvy=0;
    }

        ghostY+=ghostvy;
        ghostX+=ghostvx;
        ghostRedX+=ghostRedvx;
        ghostRedY+=ghostRedvy;
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
                
                
                setcolor(RED);
                pieslice(330,330,360,0,10);
                setcolor(BLACK);
                
                circle(330,330,10);
                setfillstyle(1,4);
                floodfill(330,335,BLACK);
                 
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
                ghostflag=false;
                
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
            if(ghostflag==true)
            drawGhost(ghostX,ghostY);
            if(ghostFlagRed==true)
            drawGhostRed(ghostRedX,ghostRedY);
            
            if(ghostflagBlue==true)
            drawGhostBlue(ghostX, ghostY);
            if(ghostflagBlue==false&&ghostflag==false)
            drawGhostBlack(ghostX,ghostY);
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
        std::ostringstream onemore;
        onemore<<"Deaths:"<< death;
        outstream(onemore,100,400);

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
        
        
         auto sleepTime = frameTime - frameDuration;
        if (sleepTime > std::chrono::milliseconds(0)) {
           int sleepTimeMs = static_cast<int>(sleepTime.count());
           
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
