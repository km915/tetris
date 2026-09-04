#include <iostream>
#include <windows.h>
#include <vector> 
#include <random>
#include <conio.h>

void resetCursor(){
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void swap(bool& a, bool& b){
    bool tmp=a;
    a=b;
    b=tmp;
}

using namespace std;

int h=30;
int w=16;
int speed=800;

vector<vector<bool>> screen = vector<vector<bool>>(h,vector<bool>(w,false));


class tetromino{
    public:
        vector<vector<bool>> grid;
        bool isIO;

        void tetromino_I_O(){
            grid =  vector<vector<bool>>(4,vector<bool>(4,false));
        }

        void rotate_IO(){
            if(grid[0][2] || grid[2][0])
                for(int idx=0;idx<=3;idx++) swap(grid[idx][2], grid[2][idx]);           //I
            else
                return;                                                                 //O
        }

        void tetromino_odd(){
            grid =  vector<vector<bool>>(3,vector<bool>(3,false));
        }

        void rotate_odd(){
            bool temp = grid[0][0];
            grid[0][0]=grid[2][0];
            grid[2][0]=grid[2][2];
            grid[2][2]=grid[0][2];
            grid[0][2]=temp;

            temp = grid[0][1];
            grid[0][1]=grid[1][0];
            grid[1][0]=grid[2][1];
            grid[2][1]=grid[1][2];
            grid[1][2]=temp;
        }
};

class Oshape : public tetromino{
    public:
        
        Oshape(){
            tetromino_I_O();
            fillingGrid();
            isIO=true;
        }

        void fillingGrid(){
            grid[1][1]=true;
            grid[1][2]=true;
            grid[2][1]=true;
            grid[2][2]=true;
        }
};

class Ishape : public tetromino{
    public:
        Ishape(){ 
            tetromino_I_O();
            fillingGrid();
            isIO=true;
        }

        void fillingGrid(){
            grid[2][0]=true;
            grid[2][1]=true;
            grid[2][2]=true;
            grid[2][3]=true;
        }
};

class Tshape : public tetromino{
    public:
        Tshape() {
            tetromino_odd();
            fillingGrid();
            isIO=false;
        }

        void fillingGrid(){
            grid[0][0]=true;
            grid[1][0]=true;
            grid[1][1]=true;
            grid[2][0]=true;
        }
};

class Lshape : public tetromino{
    public:
        Lshape(){
            tetromino_odd();
            fillingGrid();
            isIO=false;
        }

        void fillingGrid(){
            grid[0][0]=true;
            grid[1][0]=true;
            grid[2][0]=true;
            grid[2][1]=true;
        }
};

class Jshape : public tetromino{
    public:
        Jshape() {
            tetromino_odd();
            fillingGrid();
            isIO=false;

        }

        void fillingGrid(){
            grid[0][1]=true;
            grid[1][1]=true;
            grid[2][0]=true;
            grid[2][1]=true;
        }
};

class Zshape : public tetromino{
    public:
        Zshape(){
            tetromino_odd();
            fillingGrid();
            isIO=false;

        }

        void fillingGrid(){
            grid[0][1]=true;
            grid[1][1]=true;
            grid[1][0]=true;
            grid[2][0]=true;
        }
};

class Sshape : public tetromino{
    public:
        Sshape() {
            tetromino_odd();
            fillingGrid();
            isIO=false;
        }

        void fillingGrid(){
            grid[0][0]=true;
            grid[1][1]=true;
            grid[1][0]=true;
            grid[2][1]=true;
        }
};



tetromino* randomShapeGenerator(){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distrib(1, 7);
    int random_value = distrib(gen);

    switch(random_value){
        case 1:
            return new Oshape();
        case 2:
            return new Ishape();
        case 3:
            return new Tshape();
        case 4:
            return new Lshape();
        case 5:
            return new Jshape();
        case 6:
            return new Zshape();
        case 7:
            return new Sshape();
    }
    return nullptr;
}

void rotateShape(tetromino* tet, int n){
    for(int i=0;i<n;i++){
        if(tet->isIO) tet->rotate_IO();   
        else tet->rotate_odd(); 
    }
}

bool checkCollision(tetromino* tet, int x, int y){
    int n=tet->grid.size();

    for(int j=y; j<y+n; j++){
            for(int i=x; i<x+n; i++){
                if(tet->grid[j-y][i-x] && screen[j][i]) return true;
            }
        }
    return false;
}

void updateScreen(tetromino* tet, int x, int y){
    int n=tet->grid.size();

    for(int j=y; j<y+n; j++){
        for(int i=x; i<x+n; i++){
            if(tet->grid[j-y][i-x]) screen[j][i]=true;
        }
    }
}

void gameoverScreen(int score){
    system("cls");
    cout<<"- - - - - - - - - - - - "<<endl;
    cout<<"GAME OVER"<< endl;
    cout<<"YOUR SCORE: "<< score<< endl;
    cout<<"- - - - - - - - - - - - "<<endl;
}

vector<int> checkForCompletedRows(){
    bool tmp=true;
    vector<int> compRows;
    for(int j=0;j<h-1;j++){
        for(int i=1;i<w-1;i++){
            if(tmp) tmp=tmp && screen[j][i];
            else break;
        }
        if(tmp) compRows.push_back(j);
        tmp=true;
    }
    return compRows;
}

void removeCompletedRows(){
    vector<int> r=checkForCompletedRows();
    if(r.size()==0) return;

    for(int row: r){
        for(int j=row; j>0; j--){
            for(int i=1; i<w-1;i++){
                screen[j][i]=screen[j-1][i];
            }
        }
    }
}

int main(){
    int x,y, tetsize;
    tetromino* tet;
    int score=-1;
    bool gameOver=false;

    for(int i=0;i<w;i++) screen[h-1][i]=true;   //making ground layer true initially
    for(int j=0;j<h;j++) screen[j][0]=true; 
    for(int j=0;j<h;j++) screen[j][w-1]=true; 

    system("cls");

    while(!gameOver){
        score++;
        x=w/2, y=1;
        tet = randomShapeGenerator();
        tetsize=tet->grid.size();

        if(checkCollision(tet, x, y)){
            gameOver = true;
            continue;  
        }

        while(!checkCollision(tet,x,y+1)){
            cout<<"PRESS W/S/A TO ROTATE"<<endl;
            cout<<"PRESS D TO MOVE FASTER"<<endl;
            cout<<"PRESS J/L TO MOVE LEFT/RIGHT"<<endl;
            cout<<"CURRENT SCORE: "<<score<<endl;
            cout<<"\n\n\n";

            speed=200;

            //controls
            if(_kbhit()) {
                char ch = _getch(); 
                // std::cout << "You pressed: " << ch << "\n";
                if(ch=='A'|| ch=='a'){
                    rotateShape(tet, 1);
                    if(checkCollision(tet, x, y)) rotateShape(tet,3);
                }
                else if(ch=='W'||ch=='w'){
                    rotateShape(tet, 2);
                    if(checkCollision(tet, x, y)) rotateShape(tet,2);
                }
                else if(ch=='D'||ch=='d'){
                    rotateShape(tet,3);
                    if(checkCollision(tet, x, y)) rotateShape(tet,1);
                }
                else if(ch=='S'||ch=='s') speed=50;

                else if(ch=='J' || ch=='j'){
                    if(!checkCollision(tet, x-1, y)) x--;
                }
                else if(ch=='L' || ch=='l'){
                    if(!checkCollision(tet, x+1, y)) x++;
                }

                while(_kbhit()) _getch();

                // if(x==0) x++;
                // if(x==w-tetsize) x--;
            }

            //printing screen
            for(int j=0;j<h;j++){
                for(int i=0;i<w;i++){
                    if(i==0 || i==w-1 || j==h-1) cout<<"# ";
                    else if(i>=x && i<x+tetsize && j>=y && j<y+tetsize){
                        if(tet->grid[j-y][i-x]) cout<<"[]";
                        else cout<<". ";
                    }
                    else cout<<". ";
                }
                cout<<endl;
            }

            //tetromino falling each tick
            if(tet->isIO){
                if(y<h-tetsize+1) y++;
            }
            else if(y<h-tetsize) y++;

            // sleep and clear screen
            Sleep(speed);
            // system("cls");
            resetCursor();
        }
        updateScreen(tet, x, y);
        removeCompletedRows();
    }


    gameoverScreen(score);
    return 0;
}
