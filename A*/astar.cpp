#include <iostream>
#include <queue>
#include <vector>
#include <cmath>
#include <ctime>

using namespace std;

vector<int> board;
vector<int> prev_board;
vector<int> h;

class element{
public:
    int val;    //funkcja kosztu, wartość pola tabeli
    int goal;   //goal to funkcja celu
    int index;
    int prev;   //indeks poprzedniego
    
    element(int v, int g, int i, int p) : val(v), goal(v + g), index(i), prev(p) {} //g to funkcja heurystyczna - rzeczywista odległość pola tabeli do końca (0,6)
    
    bool operator<(const element& r) const  //niezbędne do poprawnego wypisywania, priority queue ma dawac najmniejszy element na poczatku
    {
        return goal > r.goal;
    }
};

void randomize(int x, int y) {
    for (int i = 0; i < x * y; i++) 
        if (i != 6 && i != 42) board.push_back(1 + rand()%9);
        else board.push_back(0);
}

int astar(int x, int y) {   //x,y - wymiary planszy
    
    bool start = true;  //to zmienna pomocnicza, przeciwdziałająca wyjściu z pętli, gdy zaczynamy w komórce o wartości 0 (odróżnienie początku od końca)
    priority_queue< element > Q;
    Q.push(element(0, 0, 6, -1));  //bo poczatek (6,0); -1 w polu prev oznacza, że el. był startem
    
    vector<bool> visited; //tablica, zapisująca inf., czy juz wierzchołek byl rozwazany
    visited.resize(x * y, false);
    
    while(!Q.empty()) { 
        element p = Q.top(); //wierzcholek z aktualnie najmniejszym kosztem dojscia - rozwazany
        Q.pop(); // bo już jest pobrany do rozważania i zapisany w p
        if(visited[p.index]) //spr., czy juz byl rozwazany, jesli juz ten wierzcholek byl rozwazany, to bierzemy kolejny
            continue;

        visited[p.index] = true; //rozwazamy go teraz
        prev_board[p.index] = p.prev; //zapisujemy wczesniejszy elemnet sciezki
        if(board[p.index] == 0 && !start) //spr. czy dotarlismy do konca sciezki (cel)
            return p.index; //mozna na twardo (0,6), bo to cel
        else
            start = false; //tu ustawiamy false, wiec jak jeszcze raz dojdziemy do pola, ktore ma wartosc 0, to koniec wyszukiwania (dla odroznienia konca od poczatku)

        if(p.index % x != x-1) //prawa sciana ograniczenie
            Q.push(element(p.val + board[p.index+1], h[p.index+1], p.index+1, p.index));
        if(p.index / x != y-1) //dolna sciana ograniczenie
            Q.push(element(p.val + board[p.index+x], h[p.index+x], p.index+x, p.index));
        if(p.index % x != 0) //lewa sciana
            Q.push(element(p.val + board[p.index-1], h[p.index-1], p.index-1, p.index));
        if(p.index / x != 0) //gorna sciana 
            Q.push(element(p.val + board[p.index-x], h[p.index-x], p.index-x, p.index));
    }
}

void print(int a, int x, int y) {
    int i = a;
    while(i != -1) { //wartosc dla oznaczenia poczatku
        int w = i;
        i = prev_board[i];
        prev_board[w] = -2; //w tablicy parent wartosci -2 wyznacza sciezke
    }
    
    for(i = 0; i < y; ++i) { //przejscie po tab. i dla kazdego indexu wypisuje wartość, gdy el. nalezy do scieżki lub " ", gdy nie.
        for(int j=0; j<x; ++j)
            if(prev_board[i*x + j] == -2) cout << board[i*x + j] << " ";
            else cout << "  ";
        cout << "\n";
    }
}

void printBoard(int x, int y) {
    for (int i = 0; i < y; i++) {
        for (int j = 0; j < x; j++)
            cout << board[i * x + j] << " ";
        cout << "\n";
    }
} 

int main() {
    int x = 7, y = 7;
    
    /*cout << "Enter values: \n";
    int w;
    for(int i=0; i<x*y; ++i) {
        cin >> w;
        board.push_back(w);
    }*/

    srand(time(nullptr));
    randomize(x, y);
    printBoard(x, y);

    prev_board.resize(x * y);

    for(int i = 0; i < x*y; i++) //wartosci funkcji heurystycznej
        h.push_back(sqrt( (0 - i%x)*(0 - i%x) + (6 - i/x)*(6 - i/x) )); //odleglosc w linii prostej od obecnego wierzcholka do mety 

    int a = astar(x, y); // indeks konca 7*6
    
    cout << "\n";
    print(a, x, y);

    
    return 0;
}
