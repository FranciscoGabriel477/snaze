#include<iostream>
#include<vector>
#include <thread>
#include <chrono>
#include <algorithm>
#include <windows.h>
class piece{
private:
    int ci;
    int cj;
    int cifather;
    int cjfather;
    char cc;
public:
    piece(int x,int y,char c){ci=x;cj=y;cifather=-1;cjfather=-1;cc=c;}
    int i()const{return ci;}
    int j()const{return cj;}
    int ifa()const{return cifather;}
    int jfa()const{return cjfather;}
    char block()const{return cc;}
    void makepath(int a, int b){cifather=a;cjfather=b;}
    bool havefather(){return cifather==-1 ||cjfather==-1;}
    bool operator==(piece b){return ci==b.i() && cj==b.j();}
    void clear(){cc=' ';}
    void fix(char c){cc=c;}
    void print(){std::cout<<ci<<','<<cj<<' ';}
    void printc(){std::cout<<cc;}
};
class snake{
    private:
    std::vector<piece> body;
    public:
    snake(std::vector<piece> b){body=b;}
    std::vector<piece> getbody()const{return body;}
    void newpos(std::vector<piece> newpos){body=newpos;}
};
class maze{
    private:
    std::vector<std::vector<piece>> mz;
    public:
    maze(std::vector<std::vector<piece>> mazet){
        mz=mazet;
    }
    void move(piece p,snake & s){
        piece to=p;
        std::vector<piece> aux;
        piece last=p;
        for(piece i:s.getbody()){
            if(!to.havefather()){
            aux.push_back(to);
            to=mz[to.ifa()][to.jfa()];
            last=i;
            }
            else{
            aux.push_back(to);
            to=last;
            last=i;
            }
        }
        s.newpos(aux);
    }
    std::vector<std::vector<piece>>& getmaze(){return mz;}
    bool free(piece& p,snake s){
        piece tempim=s.getbody()[1];
        this->move(p,s);
        return p.havefather() && p.block()==' ' && p.i() >=0 && p.j()>=0 && !(tempim==s.getbody()[0]);
    }
    void search(piece p,std::vector<piece>& path,snake s){
        if(p.i()+1>=0 && p.j()>=0 && p.i()+1<=4 &&p.j()<=5){if(this->free(mz[p.i()+1][p.j()],s)){path.push_back(mz[p.i()+1][p.j()]);mz[p.i()+1][p.j()].makepath(p.i(),p.j());}}
        if(p.i()>=0 && p.j()+1>=0 && p.i()<=4 &&p.j()+1<=5){if(this->free(mz[p.i()][p.j()+1],s)){path.push_back(mz[p.i()][p.j()+1]);mz[p.i()][p.j()+1].makepath(p.i(),p.j());}}
        if(p.i()-1>=0 && p.j()>=0 && p.i()-1<=4 &&p.j()<=5){if(this->free(mz[p.i()-1][p.j()],s)){path.push_back(mz[p.i()-1][p.j()]);mz[p.i()-1][p.j()].makepath(p.i(),p.j());}}
        if(p.i()>=0 && p.j()-1>=0 && p.i()<=4 &&p.j()-1<=5){if(this->free(mz[p.i()][p.j()-1],s)){path.push_back(mz[p.i()][p.j()-1]);mz[p.i()][p.j()-1].makepath(p.i(),p.j());}}
    }
    void printm(){
        for(std::vector<piece> i:mz){
            for(piece j:i){
                j.printc();
            }
        std::cout<<"\n";
        }
    }
    void clearspace(int i,int j){
        mz[i][j].clear();
    }
    void fixspace(int i,int j,char c){
        mz[i][j].fix(c);
    }
};
std::vector<piece> bfs(maze& m,piece& goal,snake& s){
    std::vector<piece> mpath;
    std::vector<piece> path;
    mpath.push_back(s.getbody()[0]);
    while(true){
        if(mpath[0]==goal){
            break;
        }
        m.search(mpath[0],mpath,s);
        mpath.erase(mpath.begin());
    }
    piece printp=m.getmaze()[goal.i()][goal.j()];
    while(!printp.havefather()){
    path.push_back(printp);
    printp=m.getmaze()[printp.ifa()][printp.jfa()];
    }
    std::reverse(path.begin(),path.end());
    for(piece i:path){
        i.print();
    }
    std::cout<<"\n";
    return path;
}
void nicerun(maze &m,std::vector<piece> &path,snake &s){
    for(piece p: path){
        m.printm();
        for(piece i:s.getbody()){
            m.clearspace(i.i(),i.j());
        }
        m.move(p,s);
        bool first=true;
        for(piece i:s.getbody()){
            if(first){
            m.fixspace(i.i(),i.j(),'x');
            first=false;
            }
            else{
            m.fixspace(i.i(),i.j(),'o');
            }
        }
        Sleep(100);
        for(size_t i=0;i<m.getmaze().size();i++){
            std::cout << "\033[A"<< "\033[2K";
        }
        
    }
}
int main(){
    std::vector<std::vector<char>> mat {
    {' ', ' ', ' ',' ',' ','#'},
    {' ', 'x', '#','#',' ',' '},
    {' ', 'o', '#',' ','#',' '},
    {' ', ' ', ' ',' ','#',' '},
    {' ', ' ', ' ','#','#',' '}};
    std::vector<std::vector<piece>> mp;
    for(unsigned int i=0;i<mat.size();i++){
        std::vector<piece> tempv;
        for(unsigned int j=0;j<mat[i].size();j++){
            tempv.emplace_back(i,j,mat[i][j]);
        }
        mp.push_back(tempv);
        tempv.clear();
    }
    maze mz(mp);
    std::vector<piece> tempvp;
    piece goal(4,5,' ');
    tempvp.push_back(mz.getmaze()[1][1]);
    tempvp.push_back(mz.getmaze()[2][1]);
    snake s(tempvp);
    std::vector<piece> bestpath=bfs(mz,goal,s);
    nicerun(mz,bestpath,s);
}