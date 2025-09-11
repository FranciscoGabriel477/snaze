#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <algorithm>
#include <random>
#include <cstddef>
#include <fstream>
#include <cstdlib>
#include <cstring>
class Player{
    private:
        size_t life;
        size_t score;
        size_t score_to_pass;
    public:
        Player(size_t lifet){
            life=lifet;
            score=0;
            score_to_pass=10;
        }
        size_t get_score(){
            return score;
        }
        size_t get_score_to_pass(){
            return score_to_pass;
        }
        size_t get_life(){
            return life;
        }
        void damage(){
            life--;
        }
        void point(){
            score++;
        }
        bool pass_level(){
            if(score>=score_to_pass){
                score=0;
                return true;
            }
            return false;
        }
        void set_score(size_t n){
            score_to_pass=n;
        }
        void set_life(size_t n){
            life=n;
        }
};
class Piece{
    private:
        size_t ci;
        size_t cj;
        Piece* father;
        char cc;

    public:
        Piece(){
            cj=0;
            ci=0;
            cc=' ';
            father=nullptr;
        }
        Piece(size_t i, size_t j,char c){
            ci=i;
            cj=j;
            father=nullptr;
            cc=c;
        }
        size_t &i(){
            return ci;
        }
        size_t &j(){
            return cj;
        }
        char &c(){
            return cc;
        }
        Piece* &yfather(){
            return father;
        }
        void make_path(Piece& p){
            father=&p;
        }
        void printc(){
            std::cout<<cc<<" ";
        }
        void printco(){
            std::cout<<"("<<ci<<","<<cj<<")"<<" ";
        }
        void clearf(){
            father=nullptr;
        }
        bool nohavefather(){
            return father==nullptr;
        }
        bool free(){
            return nohavefather() && cc==' ';
        }
        bool freer(){
            return cc==' ';
        }
        bool operator==(Piece& other){
            return ci==other.i() && cj==other.j();
        }
        bool nocolision(std::vector<Piece> &v){
            for(Piece p: v){
                if(*this==p){
                    return false;
                }
            }
            return true;
        }
        bool no_snake_colision(std::vector<Piece> &snake){
            bool first=false;
            for(Piece p: snake){
                if(first){
                    if(*this==p){
                        return false;
                    }
                }
                first=true;
            }
            return true;
        }
};
class Maze{

    private:
        size_t height;
        size_t width;
        size_t ispawn;
        size_t jspawn;
        std::vector<std::vector<Piece>> maze;
        std::vector<Piece> snake;
        Piece goal;
    public:
        Maze(std::vector<std::vector<Piece>> mz,size_t is,size_t js){
            maze=mz;
            ispawn=is;
            jspawn=js;
            snake.push_back(mz[ispawn][jspawn]);
            height=mz.size();
            width=mz[0].size();
        }
        void newgoal(){
           std::vector<Piece> foods;
           for(std::vector<Piece> v: maze){
                for(Piece p:v){
                    if(p.freer() && p.nocolision(snake)){
                        foods.push_back(p);
                    }
                }
            }
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist(0, foods.size()-1);
            goal= foods[dist(gen)];
        }
        void make_a_path(std::vector<Piece> &path, Piece &end){
            Piece temp=end;
                while(temp.yfather()!=nullptr){
                    path.insert(path.begin(), temp); 
                    temp=*temp.yfather();
                }
        }
        std::vector<Piece> bfs(){
            std::vector<Piece> tosearch;
            std::vector<Piece> path;
            std::vector<Piece> snaket=snake;
            tosearch.push_back(snake[0]);
            while(tosearch.size()>0){
                if(tosearch[0]==maze[goal.i()][goal.j()]){
                    goal=maze[goal.i()][goal.j()];
                    make_a_path(path,goal);
                    return path;
                }
                search(tosearch,tosearch[0],snaket);
                if(tosearch.size()>1){
                    tosearch.erase(tosearch.begin());
                }
                else{
                    make_a_path(path,tosearch[0]);
                    path.push_back(maze[tosearch[0].i()][tosearch[0].j()+1]);
                    return path;
                }
            }
        return path;
        }
        void search(std::vector<Piece> &tosearch,Piece target,std::vector<Piece> snaket){
            movesnaket(snaket,target);
            if(target.i()+1>=0 && target.i()+1<height&& !(maze[target.i()+1][target.j()]==snake[0]) && maze[target.i()+1][target.j()].nocolision(snaket) && maze[target.i()+1][target.j()].free()){maze[target.i()+1][target.j()].make_path(maze[target.i()][target.j()]);tosearch.push_back(maze[target.i()+1][target.j()]);}
            if(target.j()+1>=0 && target.j()+1<width && !(maze[target.i()][target.j()+1]==snake[0]) &&maze[target.i()][target.j()+1].nocolision(snaket) && maze[target.i()][target.j()+1].free()){maze[target.i()][target.j()+1].make_path(maze[target.i()][target.j()]);tosearch.push_back(maze[target.i()][target.j()+1]);}
            if(target.i()-1>=0 && target.i()-1<height&& !(maze[target.i()-1][target.j()]==snake[0]) &&maze[target.i()-1][target.j()].nocolision(snaket) && maze[target.i()-1][target.j()].free()){maze[target.i()-1][target.j()].make_path(maze[target.i()][target.j()]);tosearch.push_back(maze[target.i()-1][target.j()]);}
            if(target.j()-1>=0 && target.j()-1<width && !(maze[target.i()][target.j()-1]==snake[0]) &&maze[target.i()][target.j()-1].nocolision(snaket) && maze[target.i()][target.j()-1].free()){maze[target.i()][target.j()-1].make_path(maze[target.i()][target.j()]);tosearch.push_back(maze[target.i()][target.j()-1]);}
        }
        bool searchr(Piece target,std::vector<Piece> snaket){
            movesnaket(snaket,target);
            if(target.i()+1>=0 && target.i()+1<height&& !(maze[target.i()+1][target.j()]==snake[0]) && maze[target.i()+1][target.j()].nocolision(snaket) && maze[target.i()+1][target.j()].freer()){return true;}
            if(target.j()+1>=0 && target.j()+1<width && !(maze[target.i()][target.j()+1]==snake[0]) && maze[target.i()][target.j()+1].nocolision(snaket) && maze[target.i()][target.j()+1].freer()){return true;}
            if(target.i()-1>=0 && target.i()-1<height&& !(maze[target.i()-1][target.j()]==snake[0]) && maze[target.i()-1][target.j()].nocolision(snaket) && maze[target.i()-1][target.j()].freer()){return true;}
            if(target.j()-1>=0 && target.j()-1<width && !(maze[target.i()][target.j()-1]==snake[0]) && maze[target.i()][target.j()-1].nocolision(snaket) && maze[target.i()][target.j()-1].freer()){return true;}
            return false;
        }
        void movesnaket(std::vector<Piece> &snaket,Piece local){
            if(snaket.size()==1){
                snake[0]=local;
            }
            else if(!(snaket[0]==local)){
                Piece last=snaket[0];
                snaket[0]=local;
                size_t i=1;
                while(!local.nohavefather()){
                    snaket[i]=local;
                    local=*local.yfather();
                    last=local;
                }
                for(size_t j=i;j<snaket.size();j++){
                    Piece tempst=snaket[i];
                    snaket[i]=last;
                    last=tempst;
                }
            }
        }
        void movesnake(Piece local){
            Piece last=snake[0];
            snake[0]=local;
            for(int i=1;i<snake.size();i++){
                Piece temps=snake[i];
                snake[i]=last;
                last=temps;
            }
        }
        void clearscreen() {
        std::cout << "\033[2J\033[1;1H";
            #ifndef _WIN32
                std::system("clear");  
            #else
                std::system("cls");    
            #endif
        }
        void process_events(std::vector<Piece> &path,Player &player,size_t &fps){
            for(Piece p:path){
                movesnake(p);
                std::cout<<"Life: "<<player.get_life()<<" Score: "<<player.get_score()<<" of "<<player.get_score_to_pass()<<"\n";
                print_maze();  
                std::this_thread::sleep_for(std::chrono::milliseconds(1000/fps));
                clearscreen();
                if(!(maze[p.i()][p.j()].freer() && snake[0].no_snake_colision(snake))){
                    reset();
                    clear_maze();
                    player.damage();
                    std::cout << "You took damage, Press <enter> to continue";
                    std::cin.get();
                    return;
                }   
            }
            player.point(); 
            grow();
            newgoal();
            clear_maze();
        }
        void print_maze(){
            for(std::vector<Piece> v:maze){
                for(Piece p:v){
                    if(!p.nocolision(snake)){
                        std::cout << "\033[42m  \033[0m";     
                    }
                    else if(p==goal){
                        std::cout << "\033[41m  \033[0m";;
                    }
                    else if(p.c()==' ' || p.c()=='&'){
                        std::cout <<"  ";
                    }
                    else{
                        std::cout << "\033[47m  \033[0m"; 
                    }
                }
                std::cout<<"\n";
            }
        }
        void grow(){
            snake.push_back(*snake[snake.size()-1].yfather());
        }
        void clear_maze(){
            for(size_t i=0;i<height;i++){
                for(size_t j=0;j<width;j++){
                    maze[i][j].clearf();
                }
            }
        }
        void clear_maze_r(std::vector<Piece> &path){
            for(size_t i=0;i<height;i++){
                for(size_t j=0;j<width;j++){
                    if(maze[i][j].nocolision(path)){
                    maze[i][j].clearf();
                    }
                }
            }
        }
        void reset(){
            snake.clear();
            snake.push_back(maze[ispawn][jspawn]);
        }
};
std::tuple<std::vector<std::vector<Piece>>, size_t, size_t> convert_to_maze(std::vector<std::vector<char>> temp){
    size_t tempi=0;
    std::vector<std::vector<Piece>> tempp;
    size_t is;
    size_t js;
    for(std::vector<char> v: temp){
        size_t tempj=0;
        std::vector<Piece> tempvp;
        for(char c:v){
            if(c=='&'){
                is=tempi;
                js=tempj;
                tempvp.emplace_back(tempi,tempj,c);
                tempj++;
            }
            else{
                tempvp.emplace_back(tempi,tempj,c);
                tempj++;
            }
        }
    tempp.push_back(tempvp);
    tempi++;
    }
    return std::make_tuple(tempp, is, js);
}
size_t to_size_t(const char* s) {
    errno=0;
    unsigned long long v = std::strtoull(s, nullptr, 10);
    if (errno == ERANGE || v > std::numeric_limits<size_t>::max()) {
    }
    return static_cast<size_t>(v);
}
void Running_options(int &argc,char* argv[],std::vector<std::vector<std::vector<char>>> &levels,Player &player,size_t &fps){
        if(argc<2){
            std::cout<<"--help                  Print this help text.\n";
            std::cout<<"--fps <num>             Number of frames (board) presented per second.\n";
            std::cout<<"--lives <num>           Number of lives the snake shall have. Default = 5.\n";
            std::cout<<"--food <num>            Number of food pellets for the entire simulation. Default = 10.\n";
            std::cout<<"--playertype <type>     Type of snake intelligence: random, backtracking. Default = backtracking\n";
            exit(0);
        }
        if(strcmp(argv[1], "--help") == 0){
                std::cout<<"--help                  Print this help text.\n";
                std::cout<<"--fps <num>             Number of frames (board) presented per second.\n";
                std::cout<<"--lives <num>           Number of lives the snake shall have. Default = 5.\n";
                std::cout<<"--food <num>            Number of food pellets for the entire simulation. Default = 10.\n";
                std::cout<<"--playertype <type>     Type of snake intelligence: random, backtracking. Default = backtracking\n";
                exit(0);
        }
        std::ifstream mazes(argv[1]);
        if (!mazes.is_open()) {
        std::cerr << "Error to open file\n";
        exit(0);
        }
        std::string line;
        size_t ic=0;
        std::vector<std::vector<char>> tempv;
        while(getline(mazes,line)){
            if (std::isdigit(static_cast<unsigned char>(line[0]))) {
                if(tempv.size()>0){
                    levels.push_back(tempv);
                }
                tempv.clear();
            }
            else{
                tempv.push_back(std::vector<char>(line.begin(), line.end()));
            }
        }
        for(size_t i=2;i<argc;i++){
            if(strcmp(argv[i], "--food") == 0 && i+1<argc){
                player.set_score(to_size_t(argv[i+1]));
                i++;
            }
            else if(strcmp(argv[i], "--lives") == 0 && i+1<argc){
                player.set_life(to_size_t(argv[i+1]));
                i++;
            }
            else if(strcmp(argv[i], "--fps") == 0 && i+1<argc){
                fps = to_size_t(argv[i+1]);
                i++;
            }
            else{

            }
        }
        levels.push_back(tempv);
}
int main(int argc, char* argv[]){
    Player player(5);
    size_t fps=10;
    std::vector<std::vector<std::vector<char>>> levels;
    Running_options(argc,argv,levels,player,fps);
    for(std::vector<std::vector<char>> level: levels){
        Maze mz(std::get<0>(convert_to_maze(level)),std::get<1>(convert_to_maze(level)),std::get<2>(convert_to_maze(level)));
        mz.newgoal();
        while(true){
            std::vector<Piece> path=mz.bfs();
            mz.process_events(path,player,fps);
            if(player.get_life()<=0){
                std::cout<<"GAME OVER";
                return 1;
            }
            if(player.pass_level()){
                std::cout << "Level completed, Press <enter> to continue";
                std::cin.get();
                break;
            }
        }
    }
    std::cout<<"Victory!";
    return 0;
}