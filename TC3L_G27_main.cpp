//*************************************
// Program: TC3L_G27_main.cpp
// Course: CCP6124 OOPDS
// Lecture Class: TC3L
// Tutorial Class: T11L
// Trimester: 2410
// Member_l: 1211112320 I FADZLUL BAQI FAEZ BIN FADZIL I 1211112320@student.mmu.edu.my
// Member_2: 1211112293 I HAZIQ IZZUDDIN BIN AHMAD TARMIDZI I 1211112293@student.mmu.edu.my
// Member_3: 1211112295 I UMAR HAZIQ BIN FAIZIN I 1211112295@student.mmu.edu.my
// Member_4: 1211112289 I WAN AMIRUL AMIR BIN WAN ROMZI I 1211112289@student.mmu.edu.my
// Task Distribution
// Member_l: Simulation and battlefield initialization
// Member_2: Classes base code
// Member_3: Logical computation
// Member_4: Additional robot classes
//**************************************
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <time.h>
#include <iomanip>
#include "Queue.h"
//#include "List.h"

using namespace std;

class Robot;
class RoboCop;
class Terminator;
class TerminatorRoboCop;
class BlueThunder;
class MadBot;
class RoboTank;
class UltimateRobot;
class GigaBot;
class MegaBot;
class KiloBot;
class Simulation;

class RobotPos{
private:
    int xPos;
    int yPos;

public:
    RobotPos(){}
    RobotPos(int xPos,int yPos){
    this->xPos=xPos;
    this->yPos=yPos;
    }
    bool operator==(const RobotPos& p){
        if(this->xPos == p.xPos && this->yPos == p.yPos)
            return true;
        else
            return false;}
    };

class Robot{
private:
    string type;
    string alias;
    string id;
    string name;
    int noRobotFromInput;
    int posX;
    int posY;
    int kill;
    int totalKills;
    int lives;
    bool alive;

public:
    Robot(): posX(0), posY(0), kill(0),totalKills(0), lives(3),alive(true){};
    virtual ~Robot(){};

    string getType(){
        return type;
    }
    string getAlias(){
        return alias;
    }
    string getID(){
        return id;
    }
    string getName(){
        return name;
    }
    int getPosX(){
        return posX;
    }
    int getPosY(){
        return posY;
    }
    int getnoRobotFromInput(){
        return noRobotFromInput;
    }
    int getKills(){
        return kill;
    }
    int getTotalKills(){
        return totalKills;
    }
    int getLives(){
        return lives;
    }

    virtual void setType(string type){
        this->type=type;
    }
    virtual void setAlias(string alias){
        this->alias=alias;
    }
    virtual void setID(string id){
        this->id=id;
    }
    virtual void setName(string name){
        this->name=name;
    }
    virtual void setPosX(int posX){
        this->posX = posX;
    }
    virtual void setPosY(int posY){
        this->posY = posY;
    }
    virtual void setNoRobotFromInput(int noRobotFromInput){
        this->noRobotFromInput=noRobotFromInput;
    }
    virtual void incKill(){
        kill++;
    }
    virtual void incTotalKills(){
        totalKills++;
    }
    virtual void resetKill(){
        kill=0;
    }
    virtual void setLives(int lives){ this->lives=lives;}

    virtual void setTotalKills(int totalKills){ this->totalKills=totalKills;}

    virtual bool isAlive(){ return alive;}

    int checkRemainingAlives(int total, Robot* robotList[]){
    int Alives = 0;
    for(int a=0;a<total;a++)
    {
        if(robotList[a]->isAlive())
            Alives++;
    }
    return Alives;
    }

    virtual void seeing(int total, Robot* robotList[],ofstream& logs){}
    virtual void moving(int x, int y,int total,Robot* robotList[],Queue<Robot*>& rejoinQueue, Queue<Robot*>& noRejoinQueue,ofstream& logs){
        cout << endl << this->getName() << " (" << this->getAlias()+this->getID() << ") CANNOT MOVE" << endl;
        logs << endl << this->getName() << " (" << this->getAlias()+this->getID() << ") CANNOT MOVE" << endl;
    }
    virtual void shooting(int total,Robot* robotList[],Queue<Robot*>& rejoinQueue, Queue<Robot*>& noRejoinQueue ,int cols,int rows, ofstream& logs){
        cout << endl << this->getName() << " (" << this->getAlias()+this->getID() << ") CANNOT SHOOT" << endl;
        logs << endl << this->getName() << " (" << this->getAlias()+this->getID() << ") CANNOT SHOOT" << endl;
    }

    virtual void destroyed(Queue<Robot*>& rejoinQueue,Queue<Robot*>& noRejoinQueue){
        if(this->getLives()>1){
            lives--;
            this->setPosX(-2);
            this->setPosY(-2);
            alive=false;
            rejoinQueue.enqueue(this);
        }
        else
        {
            lives--;
            this->setPosX(-2);
            this->setPosY(-2);
            alive=false;
            noRejoinQueue.enqueue(this);
        }

    }
    virtual void rejoin(int total,Robot* robotList[],int cols, int rows, ofstream& logs){

    alive = true;
    int tempPosX= rand()%cols;
    int tempPosY= rand()%rows;
    int a=0;

    while(a<total){
        if(robotList[a]->getPosX()==tempPosX && robotList[a]->getPosY()==tempPosY){
            tempPosX= rand()%cols;
            tempPosY= rand()%rows;
            a=0;
        }
        else
            a++;
    }
    posX=tempPosX;
    posY=tempPosY;

    cout << "| Reenter Battlefield |\n";
    logs << "| Reenter Battlefield |\n";
    cout << this->getName() << " (" << this->getAlias()+this->getID() << ") Has respawned at (" << posX << "," << posY << ")\n\n";
    logs << this->getName() << " (" << this->getAlias()+this->getID() << ") Has respawned at (" << posX << "," << posY << ")\n\n";
    }
    virtual Robot* upgrade(Robot* robotList[],int totalRobots, ofstream& logs){ return nullptr;}
};

class SeeingRobot:virtual public Robot{
private:
    int nearbyRobotX[8] {-2,-2,-2,-2,-2,-2,-2,-2};
    int nearbyRobotY[8] {-2,-2,-2,-2,-2,-2,-2,-2};
    bool leftTop = false, top = false, rightTop = false, left = false, right = false, leftBelow = false, below = false, rightBelow = false;
    bool nearbyRobot[8] {leftTop,top,rightTop,left,right,leftBelow,below,rightBelow};

    int sorroundPosX[8] {-2,-2,-2,-2,-2,-2,-2,-2};
    int sorroundPosY[8] {-2,-2,-2,-2,-2,-2,-2,-2};
public:
    virtual void seeing(int total, Robot* robotList[], ofstream& logs)=0;

    virtual void setNearbyRobotLeftTop(int x, int y){
        nearbyRobotX[0] = x;
        nearbyRobotY[0] = y;
        this->leftTop=true;
    }
    virtual void setNearbyRobotTop(int x, int y){
        nearbyRobotX[1] = x;
        nearbyRobotY[1] = y;
        this->top=true;
    }
    virtual void setNearbyRobotRightTop(int x, int y){
        nearbyRobotX[2] = x;
        nearbyRobotY[2] = y;
        this->rightTop=true;
    }
    virtual void setNearbyRobotLeft(int x, int y){
        nearbyRobotX[3] = x;
        nearbyRobotY[3] = y;
        this->left=true;
    }
    virtual void setNearbyRobotRight(int x, int y){
        nearbyRobotX[4] = x;
        nearbyRobotY[4] = y;
        this->right=true;
    }
    virtual void setNearbyRobotLeftBelow(int x, int y){
        nearbyRobotX[5] = x;
        nearbyRobotY[5] = y;
        this->leftBelow=true;
    }
    virtual void setNearbyRobotBelow(int x, int y){
        nearbyRobotX[6] = x;
        nearbyRobotY[6] = y;
        this->below=true;
    }
    virtual void setNearbyRobotRightBelow(int x, int y){
        nearbyRobotX[7] = x;
        nearbyRobotY[7] = y;
        this->rightBelow=true;
    }

    virtual void unsetNearbyRobotLeftTop(){
        nearbyRobotX[0] = -2;
        nearbyRobotY[0] = -2;
        this->leftTop=false;
    }
    virtual void unsetNearbyRobotTop(){
        nearbyRobotX[1] = -2;
        nearbyRobotY[1] = -2;
        this->top=false;
    }
    virtual void unsetNearbyRobotRightTop(){
        nearbyRobotX[2] = -2;
        nearbyRobotY[2] = -2;
        this->rightTop=false;
    }
    virtual void unsetNearbyRobotLeft(){
        nearbyRobotX[3] = -2;
        nearbyRobotY[3] = -2;
        this->left=false;
    }
    virtual void unsetNearbyRobotRight(){
        nearbyRobotX[4] = -2;
        nearbyRobotY[4] = -2;
        this->right=false;
    }
    virtual void unsetNearbyRobotLeftBelow(){
        nearbyRobotX[5] = -2;
        nearbyRobotY[5] = -2;
        this->leftBelow=false;
    }
    virtual void unsetNearbyRobotBelow(){
        nearbyRobotX[6] = -2;
        nearbyRobotY[6] = -2;
        this->below=false;
    }
    virtual void unsetNearbyRobotRightBelow(){
        nearbyRobotX[7] = -2;
        nearbyRobotY[7] = -2;
        this->rightBelow=false;
    }
    virtual void setNearbyRobot(bool lt,bool t,bool rt,bool l,bool r,bool lb,bool b,bool rb){
        this->leftTop=lt;
        this->top=t;
        this->rightTop=rt;
        this->left=l;
        this->right=r;
        this->leftBelow=lb;
        this->below=b;
        this->rightBelow=rb;

        nearbyRobot[0] = leftTop;
        nearbyRobot[1] = top;
        nearbyRobot[2] = rightTop;
        nearbyRobot[3] = left;
        nearbyRobot[4] = right;
        nearbyRobot[5] = leftBelow;
        nearbyRobot[6] = below;
        nearbyRobot[7] = rightBelow;
    }

    virtual void setSorroundPosX(int x){
        sorroundPosX[0] = x-1;
        sorroundPosX[1] = x;
        sorroundPosX[2] = x+1;
        sorroundPosX[3] = x-1;
        sorroundPosX[4] = x+1;
        sorroundPosX[5] = x-1;
        sorroundPosX[6] = x;
        sorroundPosX[7] = x+1;
    }
    virtual void setSorroundPosY(int y){
        sorroundPosY[0] = y-1;
        sorroundPosY[1] = y-1;
        sorroundPosY[2] = y-1;
        sorroundPosY[3] = y;
        sorroundPosY[4] = y;
        sorroundPosY[5] = y+1;
        sorroundPosY[6] = y+1;
        sorroundPosY[7] = y+1;
    }

    virtual bool getLeftTop(){return leftTop;}
    virtual bool getTop(){return top;}
    virtual bool getRightTop(){return rightTop;}
    virtual bool getLeft(){return left;}
    virtual bool getRight(){return right;}
    virtual bool getLeftBelow(){return leftBelow;}
    virtual bool getBelow(){return below;}
    virtual bool getRightBelow(){return rightBelow;}

    virtual int getNearbyRobotX(int x){ return nearbyRobotX[x];}
    virtual int getNearbyRobotY(int y){ return nearbyRobotY[y];}
    virtual bool getNearbyRobot(int side){ return nearbyRobot[side];}

    virtual int getSorroundPosX(int x){ return sorroundPosX[x];}
    virtual int getSorroundPosY(int y){ return sorroundPosY[y];}
};

class MovingRobot:virtual public Robot{
private:
    int sorroundPosX[8] {-2,-2,-2,-2,-2,-2,-2,-2};
    int sorroundPosY[8] {-2,-2,-2,-2,-2,-2,-2,-2};
    int lastPosX;
    int lastPosY;
public:
    virtual void moving(int x, int y,int total,Robot* robotList[],Queue<Robot*>& rejoinQueue, Queue<Robot*>& noRejoinQueue,ofstream& logs) = 0;

    virtual void setSorroundPosX(int x){
        sorroundPosX[0] = x-1;
        sorroundPosX[1] = x;
        sorroundPosX[2] = x+1;
        sorroundPosX[3] = x-1;
        sorroundPosX[4] = x+1;
        sorroundPosX[5] = x-1;
        sorroundPosX[6] = x;
        sorroundPosX[7] = x+1;
    }
    virtual void setSorroundPosY(int y){
        sorroundPosY[0] = y-1;
        sorroundPosY[1] = y-1;
        sorroundPosY[2] = y-1;
        sorroundPosY[3] = y;
        sorroundPosY[4] = y;
        sorroundPosY[5] = y+1;
        sorroundPosY[6] = y+1;
        sorroundPosY[7] = y+1;
    }

    virtual int getSorroundPosX(int x){ return sorroundPosX[x];}
    virtual int getSorroundPosY(int y){ return sorroundPosY[y];}

    virtual void setLastPosX(int lastPosX){
        this->lastPosX = lastPosX;
    }
    virtual void setLastPosY(int lastPosY){
        this->lastPosY = lastPosY;
    }

    virtual int getLastPosX(){ return lastPosX;}
    virtual int getLastPosY(){ return lastPosY;}

};

class ShootingRobot:virtual public Robot{
private:

public:
    virtual void shooting(int total,Robot* robotList[],Queue<Robot*>& rejoinQueue, Queue<Robot*>& noRejoinQueue ,int cols,int rows, ofstream& logs) = 0;

    virtual void shooted(){
        incKill();
        incTotalKills();
    }
};

class SteppingRobot:virtual public Robot{

private:

public:
    virtual void stepping(int total, Robot* robotList[]) = 0;

    virtual void stepped(ofstream& logs){

        cout << "\nSteppingRobot actionStep\n";
        logs << "\nSteppingRobot actionStep\n";
        incKill();
        incTotalKills();
    }
};
class GigaBot:virtual public SeeingRobot,virtual public MovingRobot,virtual public ShootingRobot, virtual public SteppingRobot{
private:
    int noRobotSameType;
    int indexTarget = 0;
public:
    GigaBot(string type,string alias,string id, string name,int x, int y,int noRobotFromInput, int initLives, int initKill){
    setType(type);
    setAlias(alias);
    setID(id);
    setName(name);
    setPosX(x);
    setPosY(y);
    setNoRobotFromInput(noRobotFromInput);
    MovingRobot::setSorroundPosX(x);
    MovingRobot::setSorroundPosY(y);
    setLives(initLives);
    setTotalKills(initKill);

    RobotPos p(x,y);
    }
    void seeing(int total, Robot* robotList[],ofstream& logs)override{

        cout << "SeeingRobot actionLook\n\n";
        logs << "SeeingRobot actionLook\n\n";

        unsetNearbyRobotLeftTop();
        unsetNearbyRobotTop();
        unsetNearbyRobotRightTop();
        unsetNearbyRobotLeft();
        unsetNearbyRobotRight();
        unsetNearbyRobotLeftBelow();
        unsetNearbyRobotBelow();
        unsetNearbyRobotRightBelow();

        int x = getPosX();
        int y = getPosY();

        for(int a=0;a<total;a++)
        {
            if(!getLeftTop()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotLeftTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeftTop();
            }
            if(!getTop()){
                if(robotList[a]->getPosX()==(x) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotTop();
            }
            if(!getRightTop()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotRightTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRightTop();
            }
            if(!getLeft()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y)){
                    setNearbyRobotLeft(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeft();
            }
            if(!getRight()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y)){
                    setNearbyRobotRight(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRight();
            }
            if(!getLeftBelow()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotLeftBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeftBelow();
            }
            if(!getBelow()){
                if(robotList[a]->getPosX()==(x) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotBelow();
            }
            if(!getRightBelow()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotRightBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRightBelow();
            }
        }
        setNearbyRobot(getLeftTop(),getTop(),getRightTop(),getLeft(),getRight(),getLeftBelow(),getBelow(),getRightBelow());
    }
    void moving(int cols, int rows,int total,Robot* robotList[],Queue<Robot*>& rejoinQueue, Queue<Robot*>& noRejoinQueue,ofstream& logs)override{

        cout << "MovingRobot actionMove\n";
        logs << "MovingRobot actionMove\n";

        MovingRobot::setSorroundPosX(getPosX());
        MovingRobot::setSorroundPosY(getPosY());
        int x = getPosX();
        int y = getPosY();

        RobotPos lastPos(x,y);
        setLastPosX(x);
        setLastPosY(y);

        int whichSide = rand()%8;

            if(getNearbyRobot(0)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(0) && robotList[a]->getPosY()==getNearbyRobotY(0))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(0));
                setPosY(getNearbyRobotY(0));

            }
            else if(getNearbyRobot(1)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(1) && robotList[a]->getPosY()==getNearbyRobotY(1))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(1));
                setPosY(getNearbyRobotY(1));

            }
            else if(getNearbyRobot(2)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(2) && robotList[a]->getPosY()==getNearbyRobotY(2))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(2));
                setPosY(getNearbyRobotY(2));

            }
            else if(getNearbyRobot(3)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(3) && robotList[a]->getPosY()==getNearbyRobotY(3))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(3));
                setPosY(getNearbyRobotY(3));

            }
            else if(getNearbyRobot(4)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(4) && robotList[a]->getPosY()==getNearbyRobotY(4))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setLastPosX(x);
                setLastPosY(y);
                setPosX(getNearbyRobotX(4));
                setPosY(getNearbyRobotY(4));

            }
            else if(getNearbyRobot(5)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(5) && robotList[a]->getPosY()==getNearbyRobotY(5))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(5));
                setPosY(getNearbyRobotY(5));

            }
            else if(getNearbyRobot(6)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(6) && robotList[a]->getPosY()==getNearbyRobotY(6))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(6));
                setPosY(getNearbyRobotY(6));

            }
            else if(getNearbyRobot(7)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(7) && robotList[a]->getPosY()==getNearbyRobotY(7))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(7));
                setPosY(getNearbyRobotY(7));

            }
            else
            {
                setPosX(MovingRobot::getSorroundPosX(whichSide)<0 || MovingRobot::getSorroundPosX(whichSide) >cols-1 ? x : MovingRobot::getSorroundPosX(whichSide));
                setPosY(MovingRobot::getSorroundPosY(whichSide)<0 || MovingRobot::getSorroundPosY(whichSide)>rows-1 ? y : MovingRobot::getSorroundPosY(whichSide));

                RobotPos nowPos(this->getPosX(),this->getPosY());

                if (nowPos==lastPos){
                    if(MovingRobot::getSorroundPosX(whichSide)<0){
                        setPosX(x+1);
                    }
                    if(MovingRobot::getSorroundPosY(whichSide)<0){
                        setPosY(y+1);
                    }
                    if(MovingRobot::getSorroundPosX(whichSide)>cols-1){
                        setPosX(x-1);
                    }
                    if(MovingRobot::getSorroundPosY(whichSide)>rows-1){
                        setPosY(y-1);
                    }
                }

                cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << (MovingRobot::getSorroundPosX(whichSide)<0 || MovingRobot::getSorroundPosX(whichSide) >cols-1 ? x : MovingRobot::getSorroundPosX(whichSide)) << "," << (MovingRobot::getSorroundPosY(whichSide)<0 || MovingRobot::getSorroundPosY(whichSide)>rows-1 ? y : MovingRobot::getSorroundPosY(whichSide)) << ") FROM (" << getLastPosX() << "," << getLastPosY()<< ")" << endl;
                logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << (MovingRobot::getSorroundPosX(whichSide)<0 || MovingRobot::getSorroundPosX(whichSide) >cols-1 ? x : MovingRobot::getSorroundPosX(whichSide)) << "," << (MovingRobot::getSorroundPosY(whichSide)<0 || MovingRobot::getSorroundPosY(whichSide)>rows-1 ? y : MovingRobot::getSorroundPosY(whichSide)) << ") FROM (" << getLastPosX() << "," << getLastPosY()<< ")" << endl;
            }

    }
    void shooting(int total,Robot* robotList[],Queue<Robot*>& rejoinQueue, Queue<Robot*>& noRejoinQueue ,int cols,int rows,ofstream& logs)override{

        cout << "\nShootingRobot actionFire\n";
        logs << "\nShootingRobot actionFire\n";

        if(checkRemainingAlives(total,robotList)>2){
            while((!robotList[indexTarget]->isAlive() || indexTarget == this->getnoRobotFromInput()) && indexTarget<total){

                indexTarget++;
                if(indexTarget>=total)
                    indexTarget = 0;
        }
        robotList[indexTarget]->destroyed(rejoinQueue,noRejoinQueue);
        cout << robotList[indexTarget]->getName() << " (" << robotList[indexTarget]->getAlias()+robotList[indexTarget]->getID() << ") HAS BEEN SHOT BY " << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << endl;
        logs << robotList[indexTarget]->getName() << " (" << robotList[indexTarget]->getAlias()+robotList[indexTarget]->getID() << ") HAS BEEN SHOT BY " << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << endl;
        this->shooted();

        }
        if(checkRemainingAlives(total,robotList)>1){
            while((!robotList[indexTarget]->isAlive() || indexTarget == this->getnoRobotFromInput()) && indexTarget<total){

                indexTarget++;
                if(indexTarget>=total)
                    indexTarget = 0;
        }

        robotList[indexTarget]->destroyed(rejoinQueue,noRejoinQueue);
        cout << robotList[indexTarget]->getName() << " (" << robotList[indexTarget]->getAlias()+robotList[indexTarget]->getID() << ") HAS BEEN SHOT BY " << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << endl;
        logs << robotList[indexTarget]->getName() << " (" << robotList[indexTarget]->getAlias()+robotList[indexTarget]->getID() << ") HAS BEEN SHOT BY " << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << endl;
        this->shooted();

        }
    }
    void stepping(int total, Robot* robotList[])override{}
};
class MegaBot:virtual public SeeingRobot,virtual public MovingRobot,virtual public ShootingRobot, virtual public SteppingRobot{
private:
    int noRobotSameType;
    int indexTarget = 0;
public:
    MegaBot(string type,string alias,string id, string name,int x, int y,int noRobotFromInput, int initLives, int initKill){
    setType(type);
    setAlias(alias);
    setID(id);
    setName(name);
    setPosX(x);
    setPosY(y);
    setNoRobotFromInput(noRobotFromInput);
    MovingRobot::setSorroundPosX(x);
    MovingRobot::setSorroundPosY(y);
    setLives(initLives);
    setTotalKills(initKill);
    }
    void seeing(int total, Robot* robotList[],ofstream& logs)override{

        cout << "SeeingRobot actionLook\n\n";
        logs << "SeeingRobot actionLook\n\n";

        unsetNearbyRobotLeftTop();
        unsetNearbyRobotTop();
        unsetNearbyRobotRightTop();
        unsetNearbyRobotLeft();
        unsetNearbyRobotRight();
        unsetNearbyRobotLeftBelow();
        unsetNearbyRobotBelow();
        unsetNearbyRobotRightBelow();

        int x = getPosX();
        int y = getPosY();

        for(int a=0;a<total;a++)
        {
            if(!getLeftTop()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotLeftTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeftTop();
            }
            if(!getTop()){
                if(robotList[a]->getPosX()==(x) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotTop();
            }
            if(!getRightTop()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotRightTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRightTop();
            }
            if(!getLeft()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y)){
                    setNearbyRobotLeft(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeft();
            }
            if(!getRight()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y)){
                    setNearbyRobotRight(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRight();
            }
            if(!getLeftBelow()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotLeftBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeftBelow();
            }
            if(!getBelow()){
                if(robotList[a]->getPosX()==(x) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotBelow();
            }
            if(!getRightBelow()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotRightBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRightBelow();
            }
        }
        setNearbyRobot(getLeftTop(),getTop(),getRightTop(),getLeft(),getRight(),getLeftBelow(),getBelow(),getRightBelow());
    }
    void moving(int cols, int rows,int total,Robot* robotList[],Queue<Robot*>& rejoinQueue, Queue<Robot*>& noRejoinQueue,ofstream& logs)override{

        cout << "MovingRobot actionMove\n";
        logs << "MovingRobot actionMove\n";

        MovingRobot::setSorroundPosX(getPosX());
        MovingRobot::setSorroundPosY(getPosY());
        int x = getPosX();
        int y = getPosY();

        RobotPos lastPos(x,y);
        setLastPosX(x);
        setLastPosY(y);

        int whichSide = rand()%8;

            if(getNearbyRobot(0)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(0) && robotList[a]->getPosY()==getNearbyRobotY(0))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(0));
                setPosY(getNearbyRobotY(0));

            }
            else if(getNearbyRobot(1)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(1) && robotList[a]->getPosY()==getNearbyRobotY(1))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(1));
                setPosY(getNearbyRobotY(1));

            }
            else if(getNearbyRobot(2)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(2) && robotList[a]->getPosY()==getNearbyRobotY(2))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(2));
                setPosY(getNearbyRobotY(2));

            }
            else if(getNearbyRobot(3)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(3) && robotList[a]->getPosY()==getNearbyRobotY(3))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(3));
                setPosY(getNearbyRobotY(3));

            }
            else if(getNearbyRobot(4)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(4) && robotList[a]->getPosY()==getNearbyRobotY(4))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setLastPosX(x);
                setLastPosY(y);
                setPosX(getNearbyRobotX(4));
                setPosY(getNearbyRobotY(4));

            }
            else if(getNearbyRobot(5)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(5) && robotList[a]->getPosY()==getNearbyRobotY(5))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(5));
                setPosY(getNearbyRobotY(5));

            }
            else if(getNearbyRobot(6)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(6) && robotList[a]->getPosY()==getNearbyRobotY(6))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(6));
                setPosY(getNearbyRobotY(6));

            }
            else if(getNearbyRobot(7)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(7) && robotList[a]->getPosY()==getNearbyRobotY(7))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(7));
                setPosY(getNearbyRobotY(7));

            }
            else
            {
                setPosX(MovingRobot::getSorroundPosX(whichSide)<0 || MovingRobot::getSorroundPosX(whichSide) >cols-1 ? x : MovingRobot::getSorroundPosX(whichSide));
                setPosY(MovingRobot::getSorroundPosY(whichSide)<0 || MovingRobot::getSorroundPosY(whichSide)>rows-1 ? y : MovingRobot::getSorroundPosY(whichSide));

                RobotPos nowPos(this->getPosX(),this->getPosY());

                if (nowPos==lastPos){
                    if(MovingRobot::getSorroundPosX(whichSide)<0){
                        setPosX(x+1);
                    }
                    if(MovingRobot::getSorroundPosY(whichSide)<0){
                        setPosY(y+1);
                    }
                    if(MovingRobot::getSorroundPosX(whichSide)>cols-1){
                        setPosX(x-1);
                    }
                    if(MovingRobot::getSorroundPosY(whichSide)>rows-1){
                        setPosY(y-1);
                    }
                }

                cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << this->getPosX() << "," << this->getPosY() << ") FROM (" << getLastPosX() << "," << getLastPosY()<< ")" << endl;
                logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << this->getPosX() << "," << this->getPosY() << ") FROM (" << getLastPosX() << "," << getLastPosY()<< ")" << endl;
            }

    }
    void shooting(int total,Robot* robotList[],Queue<Robot*>& rejoinQueue, Queue<Robot*>& noRejoinQueue ,int cols,int rows,ofstream& logs)override{

        cout << "\nShootingRobot actionFire\n";
        logs << "\nShootingRobot actionFire\n";


        if(checkRemainingAlives(total,robotList)>1){
            while((!robotList[indexTarget]->isAlive() || indexTarget == this->getnoRobotFromInput()) && indexTarget<total){

                indexTarget++;
                if(indexTarget>=total)
                    indexTarget = 0;
        }
        robotList[indexTarget]->destroyed(rejoinQueue,noRejoinQueue);
        cout << robotList[indexTarget]->getName() << " (" << robotList[indexTarget]->getAlias()+robotList[indexTarget]->getID() << ") HAS BEEN SHOT BY " << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << endl;
        logs << robotList[indexTarget]->getName() << " (" << robotList[indexTarget]->getAlias()+robotList[indexTarget]->getID() << ") HAS BEEN SHOT BY " << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << endl;
        this->shooted();

        }

    }
    void stepping(int total, Robot* robotList[])override{}

    virtual Robot* upgrade(Robot* robotList[],int totalRobots, ofstream& logs){

        cout << endl << this->getName() << " (" << this->getAlias()+this->getID() << ") HAS BEEN UPGRADED TO (GigaBot) with ID: GB";
        logs << endl << this->getName() << " (" << this->getAlias()+this->getID() << ") HAS BEEN UPGRADED TO (GigaBot) with ID: GB";

        int tempID = 1;
        string newID;
        for(int a=0;a<totalRobots;a++)
        {
            if(robotList[a]->getAlias()=="GB")
            {
                tempID++;
            }
        }
        if(tempID<10)
            newID = "0" + to_string(tempID);
        else
            newID = to_string(tempID);

        cout << newID << endl;
        logs << newID << endl;

        return new GigaBot("GigaBot","GB",newID,this->getName(),this->getPosX(),this->getPosY(),this->getnoRobotFromInput(),this->getLives(),this->getTotalKills());
        }
};
class KiloBot:virtual public SeeingRobot,virtual public MovingRobot,virtual public ShootingRobot{
private:
    int noRobotSameType;
    int Charge =0;
    int indexTarget = 0;
public:
    KiloBot(string type,string alias,string id, string name,int x, int y,int noRobotFromInput, int initLives, int initKill){
    setType(type);
    setAlias(alias);
    setID(id);
    setName(name);
    setPosX(x);
    setPosY(y);
    setNoRobotFromInput(noRobotFromInput);
    MovingRobot::setSorroundPosX(x);
    MovingRobot::setSorroundPosY(y);
    setLives(initLives);
    setTotalKills(initKill);
    }
    void seeing(int total, Robot* robotList[],ofstream& logs)override{

        cout << "SeeingRobot actionLook\n\n";
        logs << "SeeingRobot actionLook\n\n";

        unsetNearbyRobotLeftTop();
        unsetNearbyRobotTop();
        unsetNearbyRobotRightTop();
        unsetNearbyRobotLeft();
        unsetNearbyRobotRight();
        unsetNearbyRobotLeftBelow();
        unsetNearbyRobotBelow();
        unsetNearbyRobotRightBelow();

        int x = getPosX();
        int y = getPosY();

        for(int a=0;a<total;a++)
        {
            if(!getLeftTop()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotLeftTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeftTop();
            }
            if(!getTop()){
                if(robotList[a]->getPosX()==(x) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotTop();
            }
            if(!getRightTop()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotRightTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRightTop();
            }
            if(!getLeft()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y)){
                    setNearbyRobotLeft(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeft();
            }
            if(!getRight()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y)){
                    setNearbyRobotRight(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRight();
            }
            if(!getLeftBelow()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotLeftBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeftBelow();
            }
            if(!getBelow()){
                if(robotList[a]->getPosX()==(x) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotBelow();
            }
            if(!getRightBelow()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotRightBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRightBelow();
            }
        }
        setNearbyRobot(getLeftTop(),getTop(),getRightTop(),getLeft(),getRight(),getLeftBelow(),getBelow(),getRightBelow());
    }
    void moving(int cols, int rows,int total,Robot* robotList[],Queue<Robot*>& rejoinQueue, Queue<Robot*>& noRejoinQueue,ofstream& logs)override{

        cout << "MovingRobot actionMove\n";
        logs << "MovingRobot actionMove\n";

        MovingRobot::setSorroundPosX(getPosX());
        MovingRobot::setSorroundPosY(getPosY());
        int x = getPosX();
        int y = getPosY();

        RobotPos lastPos(x,y);
        setLastPosX(x);
        setLastPosY(y);

        int whichSide = rand()%8;
        if(getLeftTop() && getTop() && getRightTop() && getLeft() && getRight() && getLeftBelow() && getBelow() && getRightBelow()){
        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "CANNOT MOVE" << endl;
        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "CANNOT MOVE" << endl;
        setPosX(x);
        setPosY(y);

        }
        else{
        while(getNearbyRobot(whichSide)==true){
            whichSide = rand()%8;
        }
        setPosX(MovingRobot::getSorroundPosX(whichSide)<0 || MovingRobot::getSorroundPosX(whichSide) >cols-1 ? x : MovingRobot::getSorroundPosX(whichSide));
        setPosY(MovingRobot::getSorroundPosY(whichSide)<0 || MovingRobot::getSorroundPosY(whichSide)>rows-1 ? y : MovingRobot::getSorroundPosY(whichSide));

        RobotPos nowPos(this->getPosX(),this->getPosY());

                if (nowPos==lastPos){
                    if(MovingRobot::getSorroundPosX(whichSide)<0){
                        setPosX(x+1);
                    }
                    if(MovingRobot::getSorroundPosY(whichSide)<0){
                        setPosY(y+1);
                    }
                    if(MovingRobot::getSorroundPosX(whichSide)>cols-1){
                        setPosX(x-1);
                    }
                    if(MovingRobot::getSorroundPosY(whichSide)>rows-1){
                        setPosY(y-1);
                    }
                }

        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << this->getPosX() << "," << this->getPosY() << ") FROM (" << getLastPosX() << "," << getLastPosY()<< ")" << endl;
        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << this->getPosX() << "," << this->getPosY() << ") FROM (" << getLastPosX() << "," << getLastPosY()<< ")" << endl;

        }
    }
    void shooting(int total,Robot* robotList[],Queue<Robot*>& rejoinQueue, Queue<Robot*>& noRejoinQueue ,int cols,int rows,ofstream& logs)override{

        cout << "\nShootingRobot actionFire\n";
        logs << "\nShootingRobot actionFire\n";

        if(Charge == 1){

        if(checkRemainingAlives(total,robotList)>1){
            while((!robotList[indexTarget]->isAlive() || indexTarget == this->getnoRobotFromInput()) && indexTarget<total){

                indexTarget++;
                if(indexTarget>=total)
                    indexTarget = 0;
        }
        robotList[indexTarget]->destroyed(rejoinQueue,noRejoinQueue);
        cout << robotList[indexTarget]->getName() << " (" << robotList[indexTarget]->getAlias()+robotList[indexTarget]->getID() << ") HAS BEEN SHOT BY " << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << endl;
        logs << robotList[indexTarget]->getName() << " (" << robotList[indexTarget]->getAlias()+robotList[indexTarget]->getID() << ") HAS BEEN SHOT BY " << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << endl;
        this->shooted();
        Charge=0;
        }

    }
    else
        {
            Charge++;
            cout << this->getName() << " (" << this->getAlias()+this->getID() << ") IS CHARGING TO SHOOT FOR THE NEXT TURN" <<endl;
            logs << this->getName() << " (" << this->getAlias()+this->getID() << ") IS CHARGING TO SHOOT FOR THE NEXT TURN" <<endl;
        }
    }
    virtual Robot* upgrade(Robot* robotList[],int totalRobots, ofstream& logs){

        cout << endl << this->getName() << " (" << this->getAlias()+this->getID() << ") HAS BEEN UPGRADED TO (MegaBot) with ID: MT";
        logs << endl << this->getName() << " (" << this->getAlias()+this->getID() << ") HAS BEEN UPGRADED TO (MegaBot) with ID: MT";

        int tempID = 1;
        string newID;
        for(int a=0;a<totalRobots;a++)
        {
            if(robotList[a]->getAlias()=="MT")
            {
                tempID++;
            }
        }
        if(tempID<10)
            newID = "0" + to_string(tempID);
        else
            newID = to_string(tempID);

        cout << newID << endl;
        logs << newID << endl;

        return new MegaBot("MegaBot","MT",newID,this->getName(),this->getPosX(),this->getPosY(),this->getnoRobotFromInput(),this->getLives(),this->getTotalKills());
        }
};
class UltimateRobot:virtual public SeeingRobot,virtual public MovingRobot,virtual public ShootingRobot,virtual public SteppingRobot{
private:
    int noRobotSameType;
public:
    UltimateRobot(string type,string alias,string id, string name,int x, int y,int noRobotFromInput, int initLives, int initKill){
    setType(type);
    setAlias(alias);
    setID(id);
    setName(name);
    setPosX(x);
    setPosY(y);
    setNoRobotFromInput(noRobotFromInput);
    MovingRobot::setSorroundPosX(x);
    MovingRobot::setSorroundPosY(y);
    setLives(initLives);
    setTotalKills(initKill);
    }
    void seeing(int total, Robot* robotList[],ofstream& logs)override{

        cout << "SeeingRobot actionLook\n\n";
        logs << "SeeingRobot actionLook\n\n";

        unsetNearbyRobotLeftTop();
        unsetNearbyRobotTop();
        unsetNearbyRobotRightTop();
        unsetNearbyRobotLeft();
        unsetNearbyRobotRight();
        unsetNearbyRobotLeftBelow();
        unsetNearbyRobotBelow();
        unsetNearbyRobotRightBelow();

        int x = getPosX();
        int y = getPosY();

        for(int a=0;a<total;a++)
        {
            if(!getLeftTop()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotLeftTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeftTop();
            }
            if(!getTop()){
                if(robotList[a]->getPosX()==(x) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotTop();
            }
            if(!getRightTop()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotRightTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRightTop();
            }
            if(!getLeft()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y)){
                    setNearbyRobotLeft(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeft();
            }
            if(!getRight()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y)){
                    setNearbyRobotRight(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRight();
            }
            if(!getLeftBelow()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotLeftBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeftBelow();
            }
            if(!getBelow()){
                if(robotList[a]->getPosX()==(x) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotBelow();
            }
            if(!getRightBelow()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotRightBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRightBelow();
            }
        }
        setNearbyRobot(getLeftTop(),getTop(),getRightTop(),getLeft(),getRight(),getLeftBelow(),getBelow(),getRightBelow());
    }
    void moving(int cols, int rows,int total,Robot* robotList[],Queue<Robot*>& rejoinQueue, Queue<Robot*>& noRejoinQueue,ofstream& logs)override{

        cout << "MovingRobot actionMove\n";
        logs << "MovingRobot actionMove\n";

        MovingRobot::setSorroundPosX(getPosX());
        MovingRobot::setSorroundPosY(getPosY());
        int x = getPosX();
        int y = getPosY();

        RobotPos lastPos(x,y);
        setLastPosX(x);
        setLastPosY(y);

        int whichSide = rand()%8;

            if(getNearbyRobot(0)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(0) && robotList[a]->getPosY()==getNearbyRobotY(0))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(0));
                setPosY(getNearbyRobotY(0));

            }
            else if(getNearbyRobot(1)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(1) && robotList[a]->getPosY()==getNearbyRobotY(1))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(1));
                setPosY(getNearbyRobotY(1));

            }
            else if(getNearbyRobot(2)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(2) && robotList[a]->getPosY()==getNearbyRobotY(2))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(2));
                setPosY(getNearbyRobotY(2));

            }
            else if(getNearbyRobot(3)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(3) && robotList[a]->getPosY()==getNearbyRobotY(3))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(3));
                setPosY(getNearbyRobotY(3));

            }
            else if(getNearbyRobot(4)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(4) && robotList[a]->getPosY()==getNearbyRobotY(4))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setLastPosX(x);
                setLastPosY(y);
                setPosX(getNearbyRobotX(4));
                setPosY(getNearbyRobotY(4));

            }
            else if(getNearbyRobot(5)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(5) && robotList[a]->getPosY()==getNearbyRobotY(5))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(5));
                setPosY(getNearbyRobotY(5));

            }
            else if(getNearbyRobot(6)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(6) && robotList[a]->getPosY()==getNearbyRobotY(6))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(6));
                setPosY(getNearbyRobotY(6));

            }
            else if(getNearbyRobot(7)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(7) && robotList[a]->getPosY()==getNearbyRobotY(7))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(7));
                setPosY(getNearbyRobotY(7));

            }
            else
            {
                setPosX(MovingRobot::getSorroundPosX(whichSide)<0 || MovingRobot::getSorroundPosX(whichSide) >cols-1 ? x : MovingRobot::getSorroundPosX(whichSide));
                setPosY(MovingRobot::getSorroundPosY(whichSide)<0 || MovingRobot::getSorroundPosY(whichSide)>rows-1 ? y : MovingRobot::getSorroundPosY(whichSide));

                RobotPos nowPos(this->getPosX(),this->getPosY());

                if (nowPos==lastPos){
                    if(MovingRobot::getSorroundPosX(whichSide)<0){
                        setPosX(x+1);
                    }
                    if(MovingRobot::getSorroundPosY(whichSide)<0){
                        setPosY(y+1);
                    }
                    if(MovingRobot::getSorroundPosX(whichSide)>cols-1){
                        setPosX(x-1);
                    }
                    if(MovingRobot::getSorroundPosY(whichSide)>rows-1){
                        setPosY(y-1);
                    }
                }

                cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << this->getPosX() << "," << this->getPosY() << ") FROM (" << getLastPosX() << "," << getLastPosY()<< ")" << endl;
                logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << this->getPosX() << "," << this->getPosY() << ") FROM (" << getLastPosX() << "," << getLastPosY()<< ")" << endl;
            }

    }
    void shooting(int total,Robot* robotList[],Queue<Robot*>& rejoinQueue, Queue<Robot*>& noRejoinQueue ,int cols,int rows,ofstream& logs)override{

        cout << "\nShootingRobot actionFire\n";
        logs << "\nShootingRobot actionFire\n";

        for(int a=0;a<3;a++){
            int randPosX= rand()%cols;
            int randPosY= rand()%rows;

            while(randPosX==this->getPosX() && randPosY==this->getPosY()){
                randPosX= rand()%cols;
                randPosY= rand()%rows;
            }
            cout << this->getName() << " (" << this->getAlias()+this->getID() << ") SHOOTED AT (" << randPosX << "," << randPosY << ")" << endl;
            logs << this->getName() << " (" << this->getAlias()+this->getID() << ") SHOOTED AT (" << randPosX << "," << randPosY << ")" << endl;

            for(int a=0;a<total;a++){
                if(robotList[a]->getPosX()==randPosX && robotList[a]->getPosY()==randPosY){
                    robotList[a]->destroyed(rejoinQueue,noRejoinQueue);
                    cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN SHOT BY " << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << endl;
                    logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN SHOT BY " << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << endl;
                    this->shooted();
                }
            }
        }
    }
    void stepping(int total, Robot* robotList[])override{}
};
class RoboTank:virtual public SeeingRobot,virtual public ShootingRobot{
private:
    int noRobotSameType;
public:
    RoboTank(string type,string alias,string id, string name,int x, int y,int noRobotFromInput, int initLives, int initKill){
    setType(type);
    setAlias(alias);
    setID(id);
    setName(name);
    setPosX(x);
    setPosY(y);
    setNoRobotFromInput(noRobotFromInput);
    setSorroundPosX(x);
    setSorroundPosY(y);
    setLives(initLives);
    setTotalKills(initKill);
    }
    void seeing(int total, Robot* robotList[],ofstream& logs)override{

        unsetNearbyRobotLeftTop();
        unsetNearbyRobotTop();
        unsetNearbyRobotRightTop();
        unsetNearbyRobotLeft();
        unsetNearbyRobotRight();
        unsetNearbyRobotLeftBelow();
        unsetNearbyRobotBelow();
        unsetNearbyRobotRightBelow();

        int x = getPosX();
        int y = getPosY();

        for(int a=0;a<total;a++)
        {
            if(!getLeftTop()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotLeftTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeftTop();
            }
            if(!getTop()){
                if(robotList[a]->getPosX()==(x) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotTop();
            }
            if(!getRightTop()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotRightTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRightTop();
            }
            if(!getLeft()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y)){
                    setNearbyRobotLeft(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeft();
            }
            if(!getRight()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y)){
                    setNearbyRobotRight(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRight();
            }
            if(!getLeftBelow()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotLeftBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeftBelow();
            }
            if(!getBelow()){
                if(robotList[a]->getPosX()==(x) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotBelow();
            }
            if(!getRightBelow()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotRightBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRightBelow();
            }
        }
        setNearbyRobot(getLeftTop(),getTop(),getRightTop(),getLeft(),getRight(),getLeftBelow(),getBelow(),getRightBelow());
    }
    void shooting(int total,Robot* robotList[],Queue<Robot*>& rejoinQueue, Queue<Robot*>& noRejoinQueue ,int cols,int rows,ofstream& logs)override{

            cout << "\nShootingRobot actionFire\n";
            logs << "\nShootingRobot actionFire\n";

            int randPosX= rand()%cols;
            int randPosY= rand()%rows;

            while(randPosX==this->getPosX() && randPosY==this->getPosY()){
                randPosX= rand()%cols;
                randPosY= rand()%rows;
            }
            cout << this->getName() << " (" << this->getAlias()+this->getID() << ") SHOOTED AT (" << randPosX << "," << randPosY << ")" << endl;
            logs << this->getName() << " (" << this->getAlias()+this->getID() << ") SHOOTED AT (" << randPosX << "," << randPosY << ")" << endl;

            for(int a=0;a<total;a++){
                if(robotList[a]->getPosX()==randPosX && robotList[a]->getPosY()==randPosY){
                    robotList[a]->destroyed(rejoinQueue,noRejoinQueue);
                    cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN SHOT BY " << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << endl;
                    logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN SHOT BY " << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << endl;
                    this->shooted();
                }
            }
    }
    virtual Robot* upgrade(Robot* robotList[],int totalRobots, ofstream& logs){

        cout << endl << this->getName() << " (" << this->getAlias()+this->getID() << ") HAS BEEN UPGRADED TO (UltimateRobot) with ID: UR";
        logs << endl << this->getName() << " (" << this->getAlias()+this->getID() << ") HAS BEEN UPGRADED TO (UltimateRobot) with ID: UR";

        int tempID = 1;
        string newID;
        for(int a=0;a<totalRobots;a++)
        {
            if(robotList[a]->getAlias()=="UR")
            {
                tempID++;
            }
        }
        if(tempID<10)
            newID = "0" + to_string(tempID);
        else
            newID = to_string(tempID);

        cout << newID << endl;
        logs << newID << endl;

        return new UltimateRobot("UltimateRobot","UR",newID,this->getName(),this->getPosX(),this->getPosY(),this->getnoRobotFromInput(),this->getLives(),this->getTotalKills());
        }
};
class MadBot:virtual public SeeingRobot,virtual public ShootingRobot{
private:
    int noRobotSameType;
public:
    MadBot(string type,string alias,string id, string name,int x, int y,int noRobotFromInput, int initLives, int initKill){
    setType(type);
    setAlias(alias);
    setID(id);
    setName(name);
    setPosX(x);
    setPosY(y);
    setNoRobotFromInput(noRobotFromInput);
    setSorroundPosX(x);
    setSorroundPosY(y);
    setLives(initLives);
    setTotalKills(initKill);
    }
    void seeing(int total, Robot* robotList[],ofstream& logs)override{

        unsetNearbyRobotLeftTop();
        unsetNearbyRobotTop();
        unsetNearbyRobotRightTop();
        unsetNearbyRobotLeft();
        unsetNearbyRobotRight();
        unsetNearbyRobotLeftBelow();
        unsetNearbyRobotBelow();
        unsetNearbyRobotRightBelow();

        int x = getPosX();
        int y = getPosY();

        for(int a=0;a<total;a++)
        {
            if(!getLeftTop()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotLeftTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeftTop();
            }
            if(!getTop()){
                if(robotList[a]->getPosX()==(x) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotTop();
            }
            if(!getRightTop()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotRightTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRightTop();
            }
            if(!getLeft()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y)){
                    setNearbyRobotLeft(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeft();
            }
            if(!getRight()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y)){
                    setNearbyRobotRight(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRight();
            }
            if(!getLeftBelow()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotLeftBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeftBelow();
            }
            if(!getBelow()){
                if(robotList[a]->getPosX()==(x) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotBelow();
            }
            if(!getRightBelow()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotRightBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRightBelow();
            }
        }
        setNearbyRobot(getLeftTop(),getTop(),getRightTop(),getLeft(),getRight(),getLeftBelow(),getBelow(),getRightBelow());
    }
    void shooting(int total,Robot* robotList[],Queue<Robot*>& rejoinQueue, Queue<Robot*>& noRejoinQueue ,int cols,int rows,ofstream& logs)override{

        cout << "\nShootingRobot actionFire\n";
        logs << "\nShootingRobot actionFire\n";

        int random = rand()%8;
        int shootX= this->getSorroundPosX(random);
        int shootY= this->getSorroundPosY(random);

        while(shootX <0 || shootY <0 || shootX >(cols-1) || shootY >(rows-1)){
            random = rand()%8;
            shootX= this->getSorroundPosX(random);
            shootY= this->getSorroundPosY(random);
        }

        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") SHOOTED AT (" << shootX << "," << shootY << ")" << endl;
        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") SHOOTED AT (" << shootX << "," << shootY << ")" << endl;
            for(int a=0;a<total;a++){
                if(robotList[a]->getPosX()==shootX && robotList[a]->getPosY()==shootY){
                    robotList[a]->destroyed(rejoinQueue,noRejoinQueue);
                    cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN SHOT BY " << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << endl;
                    this->shooted();
                }
            }
    }

    virtual Robot* upgrade(Robot* robotList[],int totalRobots, ofstream& logs){

        cout << endl << this->getName() << " (" << this->getAlias()+this->getID() << ") HAS BEEN UPGRADED TO (RoboTank) with ID: RT";
        logs << endl << this->getName() << " (" << this->getAlias()+this->getID() << ") HAS BEEN UPGRADED TO (RoboTank) with ID: RT";

        int tempID = 1;
        string newID;
        for(int a=0;a<totalRobots;a++)
        {
            if(robotList[a]->getAlias()=="RT")
            {
                tempID++;
            }
        }
        if(tempID<10)
            newID = "0" + to_string(tempID);
        else
            newID = to_string(tempID);

        cout << newID << endl;
        logs << newID << endl;

        return new RoboTank("RoboTank","RT",newID,this->getName(),this->getPosX(),this->getPosY(),this->getnoRobotFromInput(),this->getLives(),this->getTotalKills());
        }
};
class BlueThunder:virtual public SeeingRobot,virtual public ShootingRobot{
private:
    int noRobotSameType;
    int sideToShoot=1;
public:
    BlueThunder(string type,string alias,string id, string name,int x, int y,int noRobotFromInput, int initLives, int initKill){
    setType(type);
    setAlias(alias);
    setID(id);
    setName(name);
    setPosX(x);
    setPosY(y);
    setNoRobotFromInput(noRobotFromInput);
    setSorroundPosX(x);
    setSorroundPosY(y);
    setLives(initLives);
    setTotalKills(initKill);
    }
    void seeing(int total, Robot* robotList[],ofstream& logs)override{

        unsetNearbyRobotLeftTop();
        unsetNearbyRobotTop();
        unsetNearbyRobotRightTop();
        unsetNearbyRobotLeft();
        unsetNearbyRobotRight();
        unsetNearbyRobotLeftBelow();
        unsetNearbyRobotBelow();
        unsetNearbyRobotRightBelow();

        int x = getPosX();
        int y = getPosY();

        for(int a=0;a<total;a++)
        {
            if(!getLeftTop()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotLeftTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeftTop();
            }
            if(!getTop()){
                if(robotList[a]->getPosX()==(x) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotTop();
            }
            if(!getRightTop()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotRightTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRightTop();
            }
            if(!getLeft()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y)){
                    setNearbyRobotLeft(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeft();
            }
            if(!getRight()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y)){
                    setNearbyRobotRight(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRight();
            }
            if(!getLeftBelow()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotLeftBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeftBelow();
            }
            if(!getBelow()){
                if(robotList[a]->getPosX()==(x) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotBelow();
            }
            if(!getRightBelow()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotRightBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRightBelow();
            }
        }
        setNearbyRobot(getLeftTop(),getTop(),getRightTop(),getLeft(),getRight(),getLeftBelow(),getBelow(),getRightBelow());
    }
    void shooting(int total,Robot* robotList[],Queue<Robot*>& rejoinQueue, Queue<Robot*>& noRejoinQueue ,int cols,int rows,ofstream& logs)override{

        cout << "\nShootingRobot actionFire\n";
        logs << "\nShootingRobot actionFire\n";

        int BTsorroundPosX[8];
        int BTsorroundPosY[8];

        int BTx = this->getPosX();
        int BTy = this->getPosY();

        BTsorroundPosX[0] = BTx-1;
        BTsorroundPosX[1] = BTx;
        BTsorroundPosX[2] = BTx+1;
        BTsorroundPosX[3] = BTx+1;
        BTsorroundPosX[4] = BTx+1;
        BTsorroundPosX[5] = BTx;
        BTsorroundPosX[6] = BTx-1;
        BTsorroundPosX[7] = BTx-1;

        BTsorroundPosY[0] = BTy-1;
        BTsorroundPosY[1] = BTy-1;
        BTsorroundPosY[2] = BTy-1;
        BTsorroundPosY[3] = BTy;
        BTsorroundPosY[4] = BTy+1;
        BTsorroundPosY[5] = BTy+1;
        BTsorroundPosY[6] = BTy+1;
        BTsorroundPosY[7] = BTy;

        int shootX= BTsorroundPosX[sideToShoot];
        int shootY= BTsorroundPosY[sideToShoot];

        while(shootX < 0 || shootY < 0 || shootX >(cols-1) || shootY > (rows-1)){
            sideToShoot++;
            shootX= BTsorroundPosX[sideToShoot];
            shootY= BTsorroundPosY[sideToShoot];
        }

        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") SHOOTED AT (" << shootX << "," << shootY << ")" << endl;
        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") SHOOTED AT (" << shootX << "," << shootY << ")" << endl;

            for(int a=0;a<total;a++){
                if(robotList[a]->getPosX()==shootX && robotList[a]->getPosY()==shootY){
                    robotList[a]->destroyed(rejoinQueue,noRejoinQueue);
                    cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN SHOT BY " << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << endl;
                    this->shooted();
                }
            }
        sideToShoot++;

        if (sideToShoot >7)
            sideToShoot=0;
    }
    virtual Robot* upgrade(Robot* robotList[],int totalRobots, ofstream& logs){

        cout << endl << this->getName() << " (" << this->getAlias()+this->getID() << ") HAS BEEN UPGRADED TO (MadBot) with ID: MB";
        logs << endl << this->getName() << " (" << this->getAlias()+this->getID() << ") HAS BEEN UPGRADED TO (MadBot) with ID: MB";

        int tempID = 1;
        string newID;
        for(int a=0;a<totalRobots;a++)
        {
            if(robotList[a]->getAlias()=="MB")
            {
                tempID++;
            }
        }
        if(tempID<10)
            newID = "0" + to_string(tempID);
        else
            newID = to_string(tempID);

        cout << newID << endl;
        logs << newID << endl;

        return new MadBot("MadBot","MB",newID,this->getName(),this->getPosX(),this->getPosY(),this->getnoRobotFromInput(),this->getLives(),this->getTotalKills());
        }
};
class TerminatorRoboCop:virtual public SeeingRobot,virtual public MovingRobot,virtual public ShootingRobot,virtual public SteppingRobot{
private:
    int noRobotSameType;
public:
    TerminatorRoboCop(string type,string alias,string id, string name,int x, int y,int noRobotFromInput, int initLives, int initKill){
    setType(type);
    setAlias(alias);
    setID(id);
    setName(name);
    setPosX(x);
    setPosY(y);
    setNoRobotFromInput(noRobotFromInput);
    MovingRobot::setSorroundPosX(x);
    MovingRobot::setSorroundPosY(y);
    setLives(initLives);
    setTotalKills(initKill);
    }
    void seeing(int total, Robot* robotList[],ofstream& logs)override{

        cout << "SeeingRobot actionLook\n\n";
        logs << "SeeingRobot actionLook\n\n";

        unsetNearbyRobotLeftTop();
        unsetNearbyRobotTop();
        unsetNearbyRobotRightTop();
        unsetNearbyRobotLeft();
        unsetNearbyRobotRight();
        unsetNearbyRobotLeftBelow();
        unsetNearbyRobotBelow();
        unsetNearbyRobotRightBelow();

        int x = getPosX();
        int y = getPosY();

        for(int a=0;a<total;a++)
        {
            if(!getLeftTop()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotLeftTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeftTop();
            }
            if(!getTop()){
                if(robotList[a]->getPosX()==(x) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotTop();
            }
            if(!getRightTop()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotRightTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRightTop();
            }
            if(!getLeft()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y)){
                    setNearbyRobotLeft(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeft();
            }
            if(!getRight()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y)){
                    setNearbyRobotRight(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRight();
            }
            if(!getLeftBelow()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotLeftBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeftBelow();
            }
            if(!getBelow()){
                if(robotList[a]->getPosX()==(x) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotBelow();
            }
            if(!getRightBelow()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotRightBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRightBelow();
            }
        }
        setNearbyRobot(getLeftTop(),getTop(),getRightTop(),getLeft(),getRight(),getLeftBelow(),getBelow(),getRightBelow());
    }
    void moving(int cols, int rows,int total,Robot* robotList[],Queue<Robot*>& rejoinQueue, Queue<Robot*>& noRejoinQueue,ofstream& logs)override{

        cout << "MovingRobot actionMove\n";
        logs << "MovingRobot actionMove\n";

        MovingRobot::setSorroundPosX(getPosX());
        MovingRobot::setSorroundPosY(getPosY());
        int x = getPosX();
        int y = getPosY();

        RobotPos lastPos(x,y);
        setLastPosX(x);
        setLastPosY(y);

        int whichSide = rand()%8;

            if(getNearbyRobot(0)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(0) && robotList[a]->getPosY()==getNearbyRobotY(0))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(0));
                setPosY(getNearbyRobotY(0));

            }
            else if(getNearbyRobot(1)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(1) && robotList[a]->getPosY()==getNearbyRobotY(1))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(1));
                setPosY(getNearbyRobotY(1));

            }
            else if(getNearbyRobot(2)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(2) && robotList[a]->getPosY()==getNearbyRobotY(2))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(2));
                setPosY(getNearbyRobotY(2));

            }
            else if(getNearbyRobot(3)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(3) && robotList[a]->getPosY()==getNearbyRobotY(3))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(3));
                setPosY(getNearbyRobotY(3));

            }
            else if(getNearbyRobot(4)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(4) && robotList[a]->getPosY()==getNearbyRobotY(4))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(4));
                setPosY(getNearbyRobotY(4));

            }
            else if(getNearbyRobot(5)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(5) && robotList[a]->getPosY()==getNearbyRobotY(5))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(5));
                setPosY(getNearbyRobotY(5));

            }
            else if(getNearbyRobot(6)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(6) && robotList[a]->getPosY()==getNearbyRobotY(6))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(6));
                setPosY(getNearbyRobotY(6));

            }
            else if(getNearbyRobot(7)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(7) && robotList[a]->getPosY()==getNearbyRobotY(7))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(7));
                setPosY(getNearbyRobotY(7));

            }
            else
            {
                setPosX(MovingRobot::getSorroundPosX(whichSide)<0 || MovingRobot::getSorroundPosX(whichSide) >cols-1 ? x : MovingRobot::getSorroundPosX(whichSide));
                setPosY(MovingRobot::getSorroundPosY(whichSide)<0 || MovingRobot::getSorroundPosY(whichSide)>rows-1 ? y : MovingRobot::getSorroundPosY(whichSide));

                RobotPos nowPos(this->getPosX(),this->getPosY());

                if (nowPos==lastPos){
                    if(MovingRobot::getSorroundPosX(whichSide)<0){
                        setPosX(x+1);
                    }
                    if(MovingRobot::getSorroundPosY(whichSide)<0){
                        setPosY(y+1);
                    }
                    if(MovingRobot::getSorroundPosX(whichSide)>cols-1){
                        setPosX(x-1);
                    }
                    if(MovingRobot::getSorroundPosY(whichSide)>rows-1){
                        setPosY(y-1);
                    }
                }

                cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << this->getPosX() << "," << this->getPosY() << ") FROM (" << getLastPosX() << "," << getLastPosY()<< ")" << endl;
                logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << this->getPosX() << "," << this->getPosY() << ") FROM (" << getLastPosX() << "," << getLastPosY()<< ")" << endl;
            }

    }
    void shooting(int total,Robot* robotList[],Queue<Robot*>& rejoinQueue, Queue<Robot*>& noRejoinQueue ,int cols,int rows,ofstream& logs)override{

        cout << "\nShootingRobot actionFire\n";
        logs << "\nShootingRobot actionFire\n";

        for(int a=0;a<3;a++){
            int targetPosX;
            int targetPosY;
            int negativeX= rand()%2;
            int negativeY= rand()%2;
            int randPosX= rand()%11;
            int randPosY= rand()%(11-randPosX);

            while(randPosX==this->getPosX() && randPosY==this->getPosY()){
                negativeX= rand()%2;
                negativeY= rand()%2;
                randPosX= rand()%11;
                randPosY= rand()%(11-randPosX);
            }

            if(negativeX==0 && negativeY==0){
                targetPosX = this->getPosX()+randPosX;
                targetPosY = this->getPosY()+randPosY;
            }
            else if(negativeX==1 && negativeY==0){
                targetPosX = this->getPosX()-randPosX;
                targetPosY = this->getPosY()+randPosY;
            }
            else if(negativeX==0 && negativeY==1){
                targetPosX = this->getPosX()+randPosX;
                targetPosY = this->getPosY()-randPosY;
            }
            else{
                targetPosX = this->getPosX()-randPosX;
                targetPosY = this->getPosY()-randPosY;
            }
            if(targetPosX<0)
                targetPosX=-targetPosX;
            if(targetPosY<0)
                targetPosY=-targetPosY;
            if(targetPosX>(cols-1))
                targetPosX=this->getPosX()-randPosX;
            if(targetPosY>(rows-1))
                targetPosY=this->getPosY()-randPosY;

            if(targetPosX==this->getPosX() && targetPosY==this->getPosY())
            {
                targetPosX++;
                targetPosY++;
            }

                cout << this->getName() << " (" << this->getAlias()+this->getID() << ") SHOOTED AT (" << targetPosX << "," << targetPosY << ")" << endl;
                logs << this->getName() << " (" << this->getAlias()+this->getID() << ") SHOOTED AT (" << targetPosX << "," << targetPosY << ")" << endl;

            for(int a=0;a<total;a++){
                if(robotList[a]->getPosX()==targetPosX && robotList[a]->getPosY()==targetPosY){
                    robotList[a]->destroyed(rejoinQueue,noRejoinQueue);
                    cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN SHOT BY " << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << endl;
                    logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN SHOT BY " << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << endl;
                    this->shooted();
                }
            }
        }
    }
    void stepping(int total, Robot* robotList[])override{}

    virtual Robot* upgrade(Robot* robotList[],int totalRobots, ofstream& logs){

        cout << endl << this->getName() << " (" << this->getAlias()+this->getID() << ") HAS BEEN UPGRADED TO (UltimateRobot) with ID: UR";
        logs << endl << this->getName() << " (" << this->getAlias()+this->getID() << ") HAS BEEN UPGRADED TO (UltimateRobot) with ID: UR";

        int tempID = 1;
        string newID;
        for(int a=0;a<totalRobots;a++)
        {
            if(robotList[a]->getAlias()=="UR")
            {
                tempID++;
            }
        }
        if(tempID<10)
            newID = "0" + to_string(tempID);
        else
            newID = to_string(tempID);

        cout << newID << endl;
        logs << newID << endl;

        return new UltimateRobot("UltimateRobot","UR",newID,this->getName(),this->getPosX(),this->getPosY(),this->getnoRobotFromInput(),this->getLives(),this->getTotalKills());
        }
};
class Terminator:virtual public SeeingRobot,virtual public MovingRobot,virtual public SteppingRobot{
private:
    int noRobotSameType;
public:
    Terminator(string type,string alias,string id, string name,int x, int y,int noRobotFromInput, int initLives, int initKill){
    setType(type);
    setAlias(alias);
    setID(id);
    setName(name);
    setPosX(x);
    setPosY(y);
    setNoRobotFromInput(noRobotFromInput);
    MovingRobot::setSorroundPosX(x);
    MovingRobot::setSorroundPosY(y);
    setLives(initLives);
    setTotalKills(initKill);
    }
    void seeing(int total, Robot* robotList[],ofstream& logs)override{

        cout << "SeeingRobot actionLook\n\n";
        logs << "SeeingRobot actionLook\n\n";

        unsetNearbyRobotLeftTop();
        unsetNearbyRobotTop();
        unsetNearbyRobotRightTop();
        unsetNearbyRobotLeft();
        unsetNearbyRobotRight();
        unsetNearbyRobotLeftBelow();
        unsetNearbyRobotBelow();
        unsetNearbyRobotRightBelow();

        int x = getPosX();
        int y = getPosY();

        for(int a=0;a<total;a++)
        {
            if(!getLeftTop()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotLeftTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeftTop();
            }
            if(!getTop()){
                if(robotList[a]->getPosX()==(x) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotTop();
            }
            if(!getRightTop()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotRightTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRightTop();
            }
            if(!getLeft()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y)){
                    setNearbyRobotLeft(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeft();
            }
            if(!getRight()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y)){
                    setNearbyRobotRight(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRight();
            }
            if(!getLeftBelow()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotLeftBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeftBelow();
            }
            if(!getBelow()){
                if(robotList[a]->getPosX()==(x) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotBelow();
            }
            if(!getRightBelow()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotRightBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRightBelow();
            }
        }
        setNearbyRobot(getLeftTop(),getTop(),getRightTop(),getLeft(),getRight(),getLeftBelow(),getBelow(),getRightBelow());
    }
    void moving(int cols, int rows,int total, Robot* robotList[], Queue<Robot*>& rejoinQueue, Queue<Robot*>& noRejoinQueue,ofstream& logs)override{

        cout << "MovingRobot actionMove\n";
        logs << "MovingRobot actionMove\n";

        MovingRobot::setSorroundPosX(getPosX());
        MovingRobot::setSorroundPosY(getPosY());
        int x = getPosX();
        int y = getPosY();

        RobotPos lastPos(x,y);
        setLastPosX(x);
        setLastPosY(y);

        int whichSide = rand()%8;

            if(getNearbyRobot(0)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(0) && robotList[a]->getPosY()==getNearbyRobotY(0))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(0));
                setPosY(getNearbyRobotY(0));

            }
            else if(getNearbyRobot(1)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(1) && robotList[a]->getPosY()==getNearbyRobotY(1))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(1));
                setPosY(getNearbyRobotY(1));

            }
            else if(getNearbyRobot(2)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(2) && robotList[a]->getPosY()==getNearbyRobotY(2))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(2));
                setPosY(getNearbyRobotY(2));

            }
            else if(getNearbyRobot(3)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(3) && robotList[a]->getPosY()==getNearbyRobotY(3))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(3));
                setPosY(getNearbyRobotY(3));

            }
            else if(getNearbyRobot(4)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(4) && robotList[a]->getPosY()==getNearbyRobotY(4))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;

                    }
                }
                setPosX(getNearbyRobotX(4));
                setPosY(getNearbyRobotY(4));

            }
            else if(getNearbyRobot(5)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(5) && robotList[a]->getPosY()==getNearbyRobotY(5))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;

                    }
                }
                setPosX(getNearbyRobotX(5));
                setPosY(getNearbyRobotY(5));

            }
            else if(getNearbyRobot(6)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(6) && robotList[a]->getPosY()==getNearbyRobotY(6))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(6));
                setPosY(getNearbyRobotY(6));

            }
            else if(getNearbyRobot(7)){
                for(int a=0;a<total;a++){
                    if(robotList[a]->getPosX()==getNearbyRobotX(7) && robotList[a]->getPosY()==getNearbyRobotY(7))
                    {
                        stepped(logs);
                        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() << ") AND STEPPED ON " << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") "  << endl;
                        robotList[a]->destroyed(rejoinQueue,noRejoinQueue);

                        cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                        logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN DESTROYED" << endl;
                    }
                }
                setPosX(getNearbyRobotX(7));
                setPosY(getNearbyRobotY(7));

            }
            else
            {
                setPosX(MovingRobot::getSorroundPosX(whichSide)<0 || MovingRobot::getSorroundPosX(whichSide) >cols-1 ? x : MovingRobot::getSorroundPosX(whichSide));
                setPosY(MovingRobot::getSorroundPosY(whichSide)<0 || MovingRobot::getSorroundPosY(whichSide)>rows-1 ? y : MovingRobot::getSorroundPosY(whichSide));

                RobotPos nowPos(this->getPosX(),this->getPosY());

                if (nowPos==lastPos){
                    if(MovingRobot::getSorroundPosX(whichSide)<0){
                        setPosX(x+1);
                    }
                    if(MovingRobot::getSorroundPosY(whichSide)<0){
                        setPosY(y+1);
                    }
                    if(MovingRobot::getSorroundPosX(whichSide)>cols-1){
                        setPosX(x-1);
                    }
                    if(MovingRobot::getSorroundPosY(whichSide)>rows-1){
                        setPosY(y-1);
                    }
                }

                cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << this->getPosX() << "," << this->getPosY() << ") FROM (" << getLastPosX() << "," << getLastPosY()<< ")" << endl;
                logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << this->getPosX() << "," << this->getPosY() << ") FROM (" << getLastPosX() << "," << getLastPosY()<< ")" << endl;
            }

    }
    void stepping(int total, Robot* robotList[])override{}

    virtual Robot* upgrade(Robot* robotList[],int totalRobots, ofstream& logs){

        cout << endl << this->getName() << " (" << this->getAlias()+this->getID() << ") HAS BEEN UPGRADED TO (TerminatorRoboCop) with ID: TR";
        logs << endl << this->getName() << " (" << this->getAlias()+this->getID() << ") HAS BEEN UPGRADED TO (TerminatorRoboCop) with ID: TR";

        int tempID = 1;
        string newID;
        for(int a=0;a<totalRobots;a++)
        {
            if(robotList[a]->getAlias()=="TR")
            {
                tempID++;
            }
        }
        if(tempID<10)
            newID = "0" + to_string(tempID);
        else
            newID = to_string(tempID);

        cout << newID << endl;
        logs << newID << endl;

        return new TerminatorRoboCop("TerminatorRoboCop","TR",newID,this->getName(),this->getPosX(),this->getPosY(),this->getnoRobotFromInput(),this->getLives(),this->getTotalKills());
        }
};
class RoboCop:virtual public SeeingRobot,virtual public MovingRobot,virtual public ShootingRobot{
private:
    int noRobotSameType;
public:
    RoboCop(string type,string alias,string id, string name,int x, int y,int noRobotFromInput, int initLives, int initKill){
    setType(type);
    setAlias(alias);
    setID(id);
    setName(name);
    setPosX(x);
    setPosY(y);
    setNoRobotFromInput(noRobotFromInput);
    MovingRobot::setSorroundPosX(x);
    MovingRobot::setSorroundPosY(y);
    setLives(initLives);
    setTotalKills(initKill);
    }
    void seeing(int total,Robot* robotList[],ofstream& logs) override{

        cout << "SeeingRobot actionLook\n\n";
        logs << "SeeingRobot actionLook\n\n";

        unsetNearbyRobotLeftTop();
        unsetNearbyRobotTop();
        unsetNearbyRobotRightTop();
        unsetNearbyRobotLeft();
        unsetNearbyRobotRight();
        unsetNearbyRobotLeftBelow();
        unsetNearbyRobotBelow();
        unsetNearbyRobotRightBelow();

        int x = getPosX();
        int y = getPosY();

        for(int a=0;a<total;a++)
        {
            if(!getLeftTop()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotLeftTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeftTop();
            }
            if(!getTop()){
                if(robotList[a]->getPosX()==(x) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotTop();
            }
            if(!getRightTop()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y-1)){
                    setNearbyRobotRightTop(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRightTop();
            }
            if(!getLeft()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y)){
                    setNearbyRobotLeft(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeft();
            }
            if(!getRight()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y)){
                    setNearbyRobotRight(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRight();
            }
            if(!getLeftBelow()){
                if(robotList[a]->getPosX()==(x-1) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotLeftBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotLeftBelow();
            }
            if(!getBelow()){
                if(robotList[a]->getPosX()==(x) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotBelow();
            }
            if(!getRightBelow()){
                if(robotList[a]->getPosX()==(x+1) && robotList[a]->getPosY()==(y+1)){
                    setNearbyRobotRightBelow(robotList[a]->getPosX(),robotList[a]->getPosY());
                }
                else
                    unsetNearbyRobotRightBelow();
            }
        }
        setNearbyRobot(getLeftTop(),getTop(),getRightTop(),getLeft(),getRight(),getLeftBelow(),getBelow(),getRightBelow());
    }
    void moving(int cols, int rows,int total,Robot* robotList[],Queue<Robot*>& rejoinQueue, Queue<Robot*>& noRejoinQueue,ofstream& logs)override{

        cout << "MovingRobot actionMove\n";
        logs << "MovingRobot actionMove\n";

        MovingRobot::setSorroundPosX(getPosX());
        MovingRobot::setSorroundPosY(getPosY());
        int x = getPosX();
        int y = getPosY();

        RobotPos lastPos(x,y);
        setLastPosX(x);
        setLastPosY(y);

        int whichSide = rand()%8;
        if(getLeftTop() && getTop() && getRightTop() && getLeft() && getRight() && getLeftBelow() && getBelow() && getRightBelow()){
        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "CANNOT MOVE" << endl;
        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "CANNOT MOVE" << endl;
        setPosX(x);
        setPosY(y);

        }
        else{
        while(getNearbyRobot(whichSide)==true){
            whichSide = rand()%8;
        }

        setPosX(MovingRobot::getSorroundPosX(whichSide)<0 || MovingRobot::getSorroundPosX(whichSide) >cols-1 ? x : MovingRobot::getSorroundPosX(whichSide));
        setPosY(MovingRobot::getSorroundPosY(whichSide)<0 || MovingRobot::getSorroundPosY(whichSide)>rows-1 ? y : MovingRobot::getSorroundPosY(whichSide));

        RobotPos nowPos(this->getPosX(),this->getPosY());

                if (nowPos==lastPos){
                    if(MovingRobot::getSorroundPosX(whichSide)<0){
                        setPosX(x+1);
                    }
                    if(MovingRobot::getSorroundPosY(whichSide)<0){
                        setPosY(y+1);
                    }
                    if(MovingRobot::getSorroundPosX(whichSide)>cols-1){
                        setPosX(x-1);
                    }
                    if(MovingRobot::getSorroundPosY(whichSide)>rows-1){
                        setPosY(y-1);
                    }
                }

        cout << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << this->getPosX() << "," << this->getPosY() << ") FROM (" << getLastPosX() << "," << getLastPosY()<< ")" << endl;
        logs << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << "MOVED TO (" << this->getPosX() << "," << this->getPosY() << ") FROM (" << getLastPosX() << "," << getLastPosY()<< ")" << endl;

        }
    }
    void shooting(int total,Robot* robotList[],Queue<Robot*>& rejoinQueue, Queue<Robot*>& noRejoinQueue ,int cols,int rows,ofstream& logs)override{

        cout << "\nShootingRobot actionFire\n";
        logs << "\nShootingRobot actionFire\n";

        for(int a=0;a<3;a++){

            int targetPosX;
            int targetPosY;
            int negativeX= rand()%2;
            int negativeY= rand()%2;
            int randPosX= rand()%11;
            int randPosY= rand()%(11-randPosX);

            while(randPosX==0 && randPosY==0){
                negativeX= rand()%2;
                negativeY= rand()%2;
                randPosX= rand()%10;
                randPosY= rand()%(10-randPosX);
            }

            if(negativeX==0 && negativeY==0){
                targetPosX = this->getPosX()+randPosX;
                targetPosY = this->getPosY()+randPosY;
            }
            else if(negativeX==1 && negativeY==0){
                targetPosX = this->getPosX()-randPosX;
                targetPosY = this->getPosY()+randPosY;
            }
            else if(negativeX==0 && negativeY==1){
                targetPosX = this->getPosX()+randPosX;
                targetPosY = this->getPosY()-randPosY;
            }
            else{
                targetPosX = this->getPosX()-randPosX;
                targetPosY = this->getPosY()-randPosY;
            }
            if(targetPosX<0)
                targetPosX= (-targetPosX);
            if(targetPosY<0)
                targetPosY= (-targetPosY);

            if(targetPosX>(cols-1))
                targetPosX=this->getPosX()-randPosX;
            if(targetPosY>(rows-1))
                targetPosY=this->getPosY()-randPosY;

            if(targetPosX==this->getPosX() && targetPosY==this->getPosY())
            {
                targetPosX++;
                targetPosY++;
            }

            cout << this->getName() << " (" << this->getAlias()+this->getID() << ") SHOOTED AT (" << targetPosX << "," << targetPosY << ")" << endl;
            logs << this->getName() << " (" << this->getAlias()+this->getID() << ") SHOOTED AT (" << targetPosX << "," << targetPosY << ")" << endl;

            for(int a=0;a<total;a++){
                if(robotList[a]->getPosX()==targetPosX && robotList[a]->getPosY()==targetPosY){
                    robotList[a]->destroyed(rejoinQueue,noRejoinQueue);
                    cout << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN SHOT BY " << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << endl;
                    logs << robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") HAS BEEN SHOT BY " << this->getName() << " (" << this->getAlias()+this->getID() << ") "  << endl;
                    this->shooted();
                    }
                }
            }
    }
    virtual Robot* upgrade(Robot* robotList[],int totalRobots,ofstream& logs){

        cout << endl << this->getName() << " (" << this->getAlias()+this->getID() << ") HAS BEEN UPGRADED TO (TerminatorRoboCop) with ID: TR";
        logs << endl << this->getName() << " (" << this->getAlias()+this->getID() << ") HAS BEEN UPGRADED TO (TerminatorRoboCop) with ID: TR";

        int tempID = 1;
        string newID;
        for(int a=0;a<totalRobots;a++)
        {
            if(robotList[a]->getAlias()=="TR")
            {
                tempID++;
            }
        }
        if(tempID<10)
            newID = "0" + to_string(tempID);
        else
            newID = to_string(tempID);

        cout << newID << endl;
        logs << newID << endl;

        return new TerminatorRoboCop("TerminatorRoboCop","TR",newID,this->getName(),this->getPosX(),this->getPosY(),this->getnoRobotFromInput(),this->getLives(),this->getTotalKills());
        }
};

class Simulation{
private:
    int cols;
    int rows;
    int turns;
    int robots;
    Queue<Robot*> rejoinQueue;
    Queue<Robot*> noRejoinQueue;

public:
    Simulation(){
    cols = 25;
    rows = 25;
    }
    Simulation(int c, int r){
    cols = c;
    rows = r;
    }

    int getCols(){
        return cols;
    }
    int getRows(){
        return rows;
    }
    int getTurns(){
        return turns;
    }
    int getRobots(){
        return robots;
    }

    void setTurns(int t){
        turns = t;
    }

    void setRobots(int r){
        robots = r;
    }

    void processRejoin(int total,Robot* robotList[],ofstream& logs){
        if(!rejoinQueue.empty()){
            Robot* rejoiningRobot = rejoinQueue.front();
            rejoinQueue.dequeue();
            if (rejoiningRobot->getLives()>0){
                rejoiningRobot->rejoin(total,robotList,getCols(),getRows(),logs);
            }
        }
    }

    void runSimulation(int total, Robot* robotList[],Simulation *field){

        int turn = 0;
        int a=0;
        ofstream logs("fileOutput2a.txt");

            while(a<total && turn <=turns){
                    if(turn==0){
                    cout  << "SIMULATION INITIALIZATION" << endl;
                    logs  << "SIMULATION INITIALIZATION" << endl;

                    cout <<  "All Robots are joining the battlefield" << endl << endl;
                    logs <<  "All Robots are joining the battlefield" << endl << endl;

                    for(int a=0;a<total;a++){
                        cout <<robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") has spawned at (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() <<")" << endl;
                        logs <<robotList[a]->getName() << " (" << robotList[a]->getAlias()+robotList[a]->getID() << ") has spawned at (" << robotList[a]->getPosX() << "," << robotList[a]->getPosY() <<")" << endl;
                    }
                    cout << endl;
                    logs << endl;

                    cout << endl << "Battlefield\n\n";
                    logs << endl << "Battlefield\n\n";

                    int x=0;
                    int y=0;

                    cout << "  ";
                    logs << "  ";

                    for(int i=0;i<field->getCols();i++)
                    {
                        if(x<10){
                        cout << setw(4) << "0" << x;
                        logs << setw(4) << "0" << x;
                        }

                        else{
                        cout << setw(5) << x;
                        logs << setw(5) << x;
                        }
                        x++;
                    }

                    cout << endl;
                    logs << endl;

                    cout << "  ";
                    logs << "  ";

                    for(int i=0; i<field->getCols();i++)
                    {
                        cout << "+----";
                        logs << "+----";
                    }
                    cout << "+\n";
                    logs << "+\n";

                    bool found = false;
                    for(int a=0; a<field->getRows();a++){

                            if(y<10){
                                cout << "0" << y <<"|";
                                logs << "0" << y <<"|";
                            }

                            else{
                            cout << setw(2) << y << "|";
                            logs << setw(2) << y << "|";
                            }
                            y++;

                        for(int b=0; b<field->getCols();b++)
                            {
                            for(int c=0;c<total;c++){

                                if(a==(robotList[c]->getPosY()) && b==(robotList[c]->getPosX())){
                                    cout << setw(5) << robotList[c]->getAlias()+robotList[c]->getID()+"|";
                                    logs << setw(5) << robotList[c]->getAlias()+robotList[c]->getID()+"|";
                                    found=true;
                                    break;
                                }
                                else
                                    found=false;
                            }
                                if(!found){
                                    cout << setw(5) << "|";
                                    logs << setw(5) << "|";
                                }
                            }
                            cout << "\n  ";
                            logs << "\n  ";

                            for(int i=0; i<field->getCols();i++)
                            {
                                cout << "+----";
                                logs << "+----";
                            }
                                cout << "+\n";
                                logs << "+\n";

                        }
                        cout<< endl<< "--------------------------------------------------------------------------------------------------------------------------------" << endl;
                        logs<< endl<< "--------------------------------------------------------------------------------------------------------------------------------" << endl;
                        turn++;
                    }
                    else
                    {
                    if(robotList[a]->isAlive()){

                    cout  << "TURN NOW: " << turn << endl;
                    logs  << "TURN NOW: " << turn << endl;

                    for(int r=0;r<total;r++){
                        cout << endl << robotList[r]->getName() << " (" <<robotList[r]->getAlias()+robotList[r]->getID() << ") | Total Kills: " << robotList[r]->getTotalKills() << " | Lives : " << robotList[r]->getLives();
                        logs << endl << robotList[r]->getName() << " (" <<robotList[r]->getAlias()+robotList[r]->getID() << ") | Total Kills: " << robotList[r]->getTotalKills() << " | Lives : " << robotList[r]->getLives();
                    }

                    cout << "\n\nQueue to reenter the battlefield : ";
                    logs << "\n\nQueue to reenter the battlefield : ";

                    for(auto it=rejoinQueue.begin();it!=rejoinQueue.end();++it){
                    cout << (*it)->getName() << "| ";
                    logs << (*it)->getName() << "| ";
                    }

                    cout << "\n\nQueue of Robots with no life left : ";
                    logs << "\n\nQueue of Robots with no life left : ";

                    for(auto it2=noRejoinQueue.begin();it2!=noRejoinQueue.end();++it2){
                    cout << (*it2)->getName() << "| ";
                    logs << (*it2)->getName() << "| ";
                    }
                    cout << endl << endl;
                    logs << endl << endl;
                    processRejoin(total,robotList,logs);

                    cout<< endl<< "--------------------------------------------------------------------------------------------------------------------------------" << endl;
                    logs<< endl<< "--------------------------------------------------------------------------------------------------------------------------------" << endl;

                    cout << "| " << robotList[a]->getName() << "'s TURN |" <<endl;
                    logs << "| " << robotList[a]->getName() << "'s TURN |" <<endl;

                    robotList[a]->seeing(total,robotList,logs);
                    robotList[a]->moving(field->getCols(),field->getRows(),total,robotList,rejoinQueue, noRejoinQueue,logs);

                    if(robotList[a]->getKills()==3){
                        robotList[a]->resetKill();
                        Robot* upgradedRobot = robotList[a]->upgrade(robotList,total, logs);
                        if(upgradedRobot != nullptr){
                            delete robotList[a];
                            robotList[a] = upgradedRobot;
                        }
                    }

                    robotList[a]->shooting(total,robotList,rejoinQueue,noRejoinQueue,cols,rows,logs);

                    if(robotList[a]->getKills()==3){
                        robotList[a]->resetKill();
                        Robot* upgradedRobot = robotList[a]->upgrade(robotList,total, logs);
                        if(upgradedRobot != nullptr){
                            delete robotList[a];
                            robotList[a] = upgradedRobot;
                        }
                    }
                    cout << endl;
                    logs << endl;

                    int x=0;
                    int y=0;

                    cout << "  ";
                    logs << "  ";

                    for(int i=0;i<field->getCols();i++)
                    {
                        if(x<10){
                            cout << setw(4) << "0" << x;
                            logs << setw(4) << "0" << x;
                        }

                        else{
                            cout << setw(5) << x;
                            logs << setw(5) << x;
                        }

                        x++;
                    }

                    cout << endl;
                    logs << endl;

                    cout << "  ";
                    logs << "  ";

                    for(int i=0; i<field->getCols();i++)
                    {
                        cout << "+----";
                        logs << "+----";
                    }
                    cout << "+\n";
                    logs << "+\n";

                    bool found = false;
                    for(int a=0; a<field->getRows();a++){

                            if(y<10){
                                cout << "0" << y <<"|";
                                logs << "0" << y <<"|";
                            }

                            else{
                                cout << setw(2) << y << "|";
                                logs << setw(2) << y << "|";
                            }

                            y++;

                        for(int b=0; b<field->getCols();b++)
                            {
                            for(int c=0;c<total;c++){

                                if(a==(robotList[c]->getPosY()) && b==(robotList[c]->getPosX())){
                                    cout << setw(5) << robotList[c]->getAlias()+robotList[c]->getID()+"|";
                                    logs << setw(5) << robotList[c]->getAlias()+robotList[c]->getID()+"|";
                                    found=true;
                                    break;
                                }
                                else
                                    found=false;
                            }
                                if(!found){
                                    cout << setw(5) << "|";
                                    logs << setw(5) << "|";
                                }
                            }
                            cout << "\n  ";
                            logs << "\n  ";

                            for(int i=0; i<field->getCols();i++)
                            {
                                cout << "+----";
                                logs << "+----";
                            }
                                cout << "+\n";
                                logs << "+\n";
                        }
                        cout<< endl<< "--------------------------------------------------------------------------------------------------------------------------------" << endl;
                        logs<< endl<< "--------------------------------------------------------------------------------------------------------------------------------" << endl;

                    int aliveCount = 0;
                    for (int i = 0; i < total; i++) {
                        if (robotList[i]->getLives() > 0) {
                            aliveCount++;
                        }
                    }
                    if (aliveCount <= 1) {
                        cout << "Simulation ended early at turn " << turn << " as only one robot is alive." << endl;
                        logs << "Simulation ended early at turn " << turn << " as only one robot is alive." << endl;
                        break;
                        }
                    if (turn==turns){
                        cout << "Simulation ended at matximum turn which is turn " << turn << endl;
                        logs << "Simulation ended at matximum turn which is turn " << turn << endl;
                        break;
                    }

                    turn++;

                    }

                    if(a<(total-1))
                        a++;
                    else
                        a=0;
                    }
                }
        }
};

int main(){

    srand(1211112320);
    const int MAXRobot = 100;
    Simulation *field = nullptr;

    Robot *robotList[MAXRobot];

    int robotCount = 0;
    int initLives = 3;
    int initKill = 0;

    string line;
    int noLine = 1;

    ifstream input("fileInput2a.txt");
    if(input.is_open()){
        while(getline(input,line)){
            if(noLine <=3){
            string valueFromLine = line.substr(7);
            stringstream temp(valueFromLine);
            string First,Second;
            temp >> First >> Second;

            if(noLine ==1)
                field = new Simulation(stoi(First),stoi(Second));
            if(noLine ==2)
                field->setTurns(stoi(First));
            if(noLine ==3)
                field->setRobots(stoi(First));
            noLine++;
            }
            else{
            stringstream temp(line);
            string Type, Name, PosX, PosY;
            temp >> Type >> Name >> PosX >> PosY;

            string Name_type;
            string Name_ID;
            string Name_name;

            string tempName_type = Name.substr(0,2);

            if(tempName_type.substr(0,1)=="T")
            {
                if(tempName_type =="TR")
                    Name_type = "TR";
                else
                    Name_type = "T";
            }
            else
                Name_type = tempName_type;

            Name_ID = Name.substr(Name_type == "T"? 1 : 2 ,2);
            Name_name = Name.substr(Name_type == "T"? 4 : 5);

            if(Type == "RoboCop"){

                robotList[robotCount] = new RoboCop(Type,Name_type,Name_ID,Name_name,(PosX=="random" ? rand()%(field->getCols()):stoi(PosX)),(PosY=="random" ? rand()%(field->getRows()):stoi(PosY)), robotCount,initLives, initKill);
                robotCount++;
                }

            if(Type == "Terminator"){

                robotList[robotCount] = new Terminator(Type,Name_type,Name_ID,Name_name,(PosX=="random" ? rand()%(field->getCols()):stoi(PosX)),(PosY=="random" ? rand()%(field->getRows()):stoi(PosY)), robotCount,initLives,initKill);
                robotCount++;
                }

            if(Type == "TerminatorRoboCop"){

                robotList[robotCount] = new TerminatorRoboCop(Type,Name_type,Name_ID,Name_name,(PosX=="random" ? rand()%(field->getCols()):stoi(PosX)),(PosY=="random" ? rand()%(field->getRows()):stoi(PosY)),robotCount,initLives,initKill);
                robotCount++;
                }

            if(Type == "BlueThunder"){

                robotList[robotCount] = new BlueThunder(Type,Name_type,Name_ID,Name_name,(PosX=="random" ? rand()%(field->getCols()):stoi(PosX)),(PosY=="random" ? rand()%(field->getRows()):stoi(PosY)),robotCount,initLives,initKill);
                robotCount++;
                }

            if(Type == "MadBot"){

                robotList[robotCount] = new MadBot(Type,Name_type,Name_ID,Name_name,(PosX=="random" ? rand()%(field->getCols()):stoi(PosX)),(PosY=="random" ? rand()%(field->getRows()):stoi(PosY)),robotCount,initLives,initKill);
                robotCount++;
                }

            if(Type == "RoboTank"){

                robotList[robotCount] = new RoboTank(Type,Name_type,Name_ID,Name_name,(PosX=="random" ? rand()%(field->getCols()):stoi(PosX)),(PosY=="random" ? rand()%(field->getRows()):stoi(PosY)),robotCount,initLives,initKill);
                robotCount++;
                }

            if(Type == "UltimateRobot"){

                robotList[robotCount] = new UltimateRobot(Type,Name_type,Name_ID,Name_name,(PosX=="random" ? rand()%(field->getCols()):stoi(PosX)),(PosY=="random" ? rand()%(field->getRows()):stoi(PosY)),robotCount,initLives,initKill);
                robotCount++;
                }
            if(Type == "GigaBot"){

                robotList[robotCount] = new GigaBot(Type,Name_type,Name_ID,Name_name,(PosX=="random" ? rand()%(field->getCols()):stoi(PosX)),(PosY=="random" ? rand()%(field->getRows()):stoi(PosY)),robotCount,initLives,initKill);
                robotCount++;
                }
            if(Type == "MegaBot"){

                robotList[robotCount] = new MegaBot(Type,Name_type,Name_ID,Name_name,(PosX=="random" ? rand()%(field->getCols()):stoi(PosX)),(PosY=="random" ? rand()%(field->getRows()):stoi(PosY)),robotCount,initLives,initKill);
                robotCount++;
                }
            if(Type == "KiloBot"){

                robotList[robotCount] = new KiloBot(Type,Name_type,Name_ID,Name_name,(PosX=="random" ? rand()%(field->getCols()):stoi(PosX)),(PosY=="random" ? rand()%(field->getRows()):stoi(PosY)),robotCount,initLives,initKill);
                robotCount++;
                }
            }
            }
        }

    else
        cout << "Unable to open file" << endl;

        field->runSimulation(robotCount,robotList,field);

return 0;
}
