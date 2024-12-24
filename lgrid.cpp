#include<iostream>
#include<vector>
#include<algorithm>

char boardNow[25][25];
int dx[2][4][4],dy[2][4][4];

struct{
    int playerUse,computerUse;
    int playerFirst;
}setting;
struct{
    int gameEnd,whosTurn,whosWin;
}gameStatus;

struct Operation{
    int type;
    int posX,posY,valK;
    double value;
    friend bool operator<(Operation x,Operation y){
        return x.value<y.value;
    }
};

char boardTiny[4][4];
void generateDelta(){
    for(int i=1;i<=3;i++) for(int j=1;j<=3;j++) boardTiny[i][j]='.';
    boardTiny[1][1]=boardTiny[1][2]=boardTiny[1][3]=boardTiny[2][1]='X';

    std::cout<<"[+] Block 0 generating ..."<<'\n';
    for(int i=0;i<4;i++){
        std::cout<<"[+] boardTiny number"<<' '<<i<<'\n';
        for(int x=1;x<=3;x++){
            for(int y=1;y<=3;y++) std::cout<<boardTiny[x][y];
            std::cout<<'\n';
        }
        int cnt=0;
        for(int x=1;x<=3;x++){
            for(int y=1;y<=3;y++){
                if(boardTiny[x][y]=='.') continue;
                dx[0][i][cnt]=x-2,dy[0][i][cnt]=y-2;
                ++cnt;
            }
        }

        // swap the corner
        std::swap(boardTiny[1][1],boardTiny[1][3]);
        std::swap(boardTiny[3][3],boardTiny[3][1]);
        std::swap(boardTiny[1][1],boardTiny[3][3]);

        // swap the middle
        std::swap(boardTiny[2][1],boardTiny[1][2]);
        std::swap(boardTiny[2][3],boardTiny[3][2]);
        std::swap(boardTiny[2][1],boardTiny[2][3]);
    }

    std::cout<<"[+] Block 1 generating ..."<<'\n';
    std::swap(boardTiny[2][1],boardTiny[2][3]);
    for(int i=0;i<4;i++){
        std::cout<<"[+] boardTiny number"<<' '<<i<<'\n';
        for(int x=1;x<=3;x++){
            for(int y=1;y<=3;y++) std::cout<<boardTiny[x][y];
            std::cout<<'\n';
        }
        int cnt=0;
        for(int x=1;x<=3;x++){
            for(int y=1;y<=3;y++){
                if(boardTiny[x][y]=='.') continue;
                dx[1][i][cnt]=x-2,dy[1][i][cnt]=y-2;
                ++cnt;
            }
        }

        // swap the corner
        std::swap(boardTiny[1][1],boardTiny[1][3]);
        std::swap(boardTiny[3][3],boardTiny[3][1]);
        std::swap(boardTiny[1][1],boardTiny[3][3]);

        // swap the middle
        std::swap(boardTiny[2][1],boardTiny[1][2]);
        std::swap(boardTiny[2][3],boardTiny[3][2]);
        std::swap(boardTiny[2][1],boardTiny[2][3]);
    }

    return;
}

void printBoardNow(){
    for(int i=3;i<=18;i++){
        for(int j=3;j<=18;j++) std::cout<<boardNow[i][j];
        std::cout<<'\n';
    }
    return;
}

void playerMove(){
    std::cout<<'\n';
    std::cout<<"Now is your turn:"<<'\n';
    std::cout<<"Board is as follow ..."<<'\n';
    printBoardNow();
    int nowX,nowY;
    for(int i=0;i<4;i++){
        std::cout<<"Enter position "<<i<<':'<<' ';
        std::cin>>nowX>>nowY;
        boardNow[nowX+2][nowY+2]='X';
    }
    std::cout<<'\n'<<"The board now is as follows:"<<'\n';
    printBoardNow();
    return;
}

bool checkValid(int x,int y,int typ,int k){
    for(int i=0;i<4;i++){
        if(boardNow[x+dx[typ][k][i]][y+dy[typ][k][i]]=='X') return false;
    }
    return true;
}

int blockValidCount[2];
void checkBoard(){
    blockValidCount[0]=blockValidCount[1]=0;
    for(int i=3;i<=18;i++){
        for(int j=3;j<=18;j++){
            for(int k=0;k<4;k++){
                blockValidCount[0]+=checkValid(i,j,0,k);
                blockValidCount[1]+=checkValid(i,j,1,k);
            }
        }
    }
    return;
}

void checkStatus(){
    checkBoard();
    if(!blockValidCount[gameStatus.whosTurn^setting.playerUse^1]){
        gameStatus.gameEnd=1,gameStatus.whosWin=gameStatus.whosTurn^1;
    }
    return;
}

double valueBoard(int whosTurn){
    checkBoard();
    if(!whosTurn){
        if(!blockValidCount[setting.computerUse]) return -1e6;
        else if(!blockValidCount[setting.playerUse]) return 1e6;
    }else{
        if(!blockValidCount[setting.playerUse]) return 1e6;
        else if(!blockValidCount[setting.computerUse]) return -1e6;
    }
    return blockValidCount[setting.computerUse]-blockValidCount[setting.playerUse];
}

std::vector<Operation> getAvaliable(int whosTurn){
    int typeNow=whosTurn?setting.playerUse:setting.computerUse;
    std::vector<Operation> operationVector;
    for(int i=3;i<=18;i++){
        for(int j=3;j<=18;j++){
            for(int k=0;k<4;k++){
                if(checkValid(i,j,typeNow,k)){
                    operationVector.push_back((Operation){typeNow,i,j,k,0});
                }
            }
        }
    }
    return operationVector;
}

void doOperation(Operation x){
    for(int i=0;i<4;i++){
        boardNow[x.posX+dx[x.type][x.valK][i]][x.posY+dy[x.type][x.valK][i]]='X';
    }
    return;
}
void undoOperation(Operation x){
    for(int i=0;i<4;i++){
        boardNow[x.posX+dx[x.type][x.valK][i]][x.posY+dy[x.type][x.valK][i]]='.';
    }
    return;
}

double searchSecondLevel(){
    checkBoard();
    if(!blockValidCount[setting.playerUse]) return 1e6;
    std::vector<Operation> operationVector=getAvaliable(1);
    for(Operation i:operationVector){
        doOperation(i);
        i.value=valueBoard(0);
        undoOperation(i);
    }
    std::sort(operationVector.begin(),operationVector.end());
    return operationVector.front().value;
}
Operation searchFirstLevel(){
    std::vector<Operation> operationVector=getAvaliable(0);
    for(Operation i:operationVector){
        doOperation(i);
        i.value=searchSecondLevel();
        undoOperation(i);
    }
    std::sort(operationVector.begin(),operationVector.end());
    return operationVector.back();
}

void computerMove(){
    std::cout<<"Now is computer's turn:"<<'\n';
    std::cout<<"Please wait ...";
    Operation operationNow=searchFirstLevel();
    doOperation(operationNow);
    std::cout<<"done."<<'\n';
    std::cout<<'\n'<<"The board now is as follows:"<<'\n';
    printBoardNow();
    return;
}

int main(){
    // initialize the board
    for(int i=1;i<=20;i++){
        for(int j=1;j<=20;j++) boardNow[i][j]='X';
    }
    for(int i=3;i<=18;i++){
        for(int j=3;j<=18;j++) boardNow[i][j]='.';
    }
    std::cout<<"[+] Board initialized."<<'\n';

    // initialize the delta of blocks
    generateDelta();
    std::cout<<"[+] Delta array initialized."<<'\n';
    for(int typ=0;typ<2;typ++){
        std::cout<<"[+] Block"<<' '<<typ<<' '<<"as follows:"<<'\n';
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                std::cout<<'('<<dx[typ][i][j]<<','<<dy[typ][i][j]<<')'<<' ';
            }
            std::cout<<'\n';
        }
    }

    std::cout<<'\n'<<"----------------"<<'\n';
    std::cout<<"[+] Do Game Settings"<<'\n';

    std::cout<<"Block 0 is L, and Block 1 is the mirror of L"<<'\n';
    std::cout<<"Enter the one player will use: ";
    std::cin>>setting.playerUse;
    std::cout<<"Computer will use "<<(setting.computerUse=setting.playerUse^1)<<'\n';
    std::cout<<"Enter whether player will play first: ";
    std::cin>>setting.playerFirst;

    std::cout<<"We will assume that:\n1. Player will never cheat.\n2. Player's Choice is always valid."<<'\n';
    
    gameStatus.whosTurn=setting.playerFirst;
    gameStatus.gameEnd=0;
    while(!gameStatus.gameEnd){
        if(gameStatus.whosTurn) playerMove();
        else computerMove();
        gameStatus.whosTurn^=1;
        checkStatus();
        std::cout<<'[+]'<<' '<<"block count as follows: "<<blockValidCount[0]<<' '<<blockValidCount[1]<<'\n';
    }
    std::cout<<"Game ends."<<'\n';
    if(!gameStatus.whosWin) std::cout<<"Computer win this game."<<'\n';
    else std::cout<<"Player win this game."<<'\n';
    return 0;
}