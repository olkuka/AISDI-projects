#include <iostream>
#include <chrono>
#include <utility>
#include <algorithm>
#include <fstream>
#include <map>
#include <string>
#include <vector>

using namespace std;

/*!
 *  Narzedzie do zliczania czasu
 *
 *  Sposob uzycia:
 *
 *  {
 *    Benchmark<std::chrono::nanoseconds> b;
 *    // kod do zbadania
 *    size_t elapsed = b.elapsed();
 *  }    }
 *
 *  lub
 *
 *  {
 *    Benchmark<std::chrono::milliseconds> b(true);
 *    // kod do zbadania
 *  } // obiekt wypisze wartosc czasu w podanych jednostkach na stderr
 */
template<typename D = std::chrono::microseconds>
class Benchmark {
public:

    Benchmark(bool printOnExit = false) : m_print(printOnExit) {
        start = std::chrono::high_resolution_clock::now();
    }
    typename D::rep elapsed() const {
        auto end = std::chrono::high_resolution_clock::now();
        auto result = std::chrono::duration_cast<D>(end-start);
        return result.count();
    }
    ~Benchmark() {
        auto result = elapsed();
        if (m_print)
        {
            std::cerr << "czas: " << result << "\n";
        }
    }
private:
    std::chrono::high_resolution_clock::time_point start;
    bool m_print = true;
};

/*
 * Slownik
 *
 */
template<typename KeyType, typename ValueType>
class TreeMap
{
  public:
    using key_type = KeyType;
    using mapped_type = ValueType;
    using value_type = std::pair<const key_type, mapped_type>;

    struct Node {
        Node* left;
        Node* right;
        KeyType key;
        ValueType value;
        int height;
};

Node *root;

    void deleteTree(Node *node) {
        if (node != nullptr) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

    TreeMap() {
        root = nullptr;
    }    // konstruktor trywialny

    ~TreeMap() {
        deleteTree(root);
    }   // destruktor trywialny

    /*!
     * true jezeli slownik jest pusty
     */
    bool isEmpty() const {
        if (root == nullptr)
            return true;
        return false;

    }

    // wysokosc
     int height(Node *node) {  
        if (node == nullptr)  
            return 0;  
        return node -> height;  
    }  
  
    // funkcja max
    int max(int a, int b) {  
        return (a > b)? a : b;  
    }  

    //rotacja - prawa
    Node* rightRotate(Node *y)  {  
        Node *x = y -> left;  
        Node *T2 = x -> right;  
    
        // rotacja  
        x -> right = y;  
        y -> left = T2;  
    
        // aktualizacja wysokosci  
        y -> height = max(height(y -> left), height(y -> right)) + 1;  
        x -> height = max(height(x -> left), height(x -> right)) + 1;  
    
        // zwroc nowy root  
        return x;  
    }  
    
    //rotacja - lewa
    Node* leftRotate(Node *x)  {  
        Node *y = x -> right;  
        Node *T2 = y -> left;  
    
        // rotacja  
        y -> left = x;  
        x -> right = T2;  
    
        // aktualizacja wysokosci  
        x -> height = max(height(x -> left), height(x -> right)) + 1;  
        y -> height = max(height(y -> left),  height(y -> right)) + 1;  
    
        // zwroc nowy root  
        return y;  
    }  



    // funkcja zwracajaca balance  
    int getBalance(Node *node)  {  
        if (node == nullptr)  
            return 0;  
        return height(node -> left) - height(node -> right);  
    }  
    
    Node* insert(Node* node, const key_type &key, const mapped_type &value)  {  
        if (node == nullptr) //gdy drzewo jest jeszcze puste
        {
            node = new Node;
            node -> key = key;
            node -> value = value;
            node -> height = 1;
            node -> left = nullptr;
            node -> right = nullptr;
            if (isEmpty()) root = node;
            return node;
        }

        /* 1. Normalne dodawanie BST */
        if (key < node -> key)
        {  
            node -> left = insert(node -> left, key, value);
        }  
        else if (key > node -> key)
        {
            node -> right = insert(node -> right, key, value);
        }  
        else
        { 
            ++(node -> value); 
            return node;
        }  
    
        /* 2. Aktualizacja wysokosci */
        node -> height = 1 + max(height(node -> left), height(node -> right));  
    
        /* 3. Balance dla node */
        int balance = getBalance(node);  
    
        // Jesli ten node jest nie zbalansowany, to  
        // sa 4 przypadki  
    
        // Left Left przypadek  
        if (balance > 1 && key < node -> left -> key)  
            node = rightRotate(node);  
    
        // Right Right przypadek  
        if (balance < -1 && key > node -> right -> key)  
            node = leftRotate(node);  
    
        // Left Right przypadek 
        if (balance > 1 && key > node -> left -> key)  
        {  
            node -> left = leftRotate(node -> left);  
            node = rightRotate(node);  
        }  
    
        // Right Left przypadek
        if (balance < -1 && key < node -> right -> key)  
        {  
            node -> right = rightRotate(node -> right);  
            node = leftRotate(node);  
        }  
    
        root = node;
        /* zwracanie node bez zmian */
        return node; 
    }  

    /*!
     * dodaje wpis do slownika
     */
    void insert(const key_type& key, const mapped_type &value)
    {
        insert(root, key, value);
    }

    /*!
     * dodaje wpis do slownika przez podanie pary klucz-wartosc
     */
    void insert(const value_type &key_value)
    {
        insert(root, key_value.first, key_value.second);
    }

    /*!
     * zwraca referencje na wartosc dla podanego klucza
     *
     * jezeli elementu nie ma w slowniku, dodaje go
     */

    mapped_type& value(const key_type& key)
    {
        Node* cur = root;
        while(cur != nullptr)
        {
            if(cur->key == key)
                return cur->value;
            if(cur->key > key)
                cur = cur->left;
            else
                cur = cur->right;
        }
    }

    /*!
     * zwraca wartosc dla podanego klucza
     */

    mapped_type& operator[](const key_type& key)
    {
        Node* cur = root;
        while(cur != nullptr)
        {
            if(cur->key == key)
                return cur->value;
            if(cur->key > key)
                cur = cur->left;
            else
                cur = cur->right;
        }
        insert(root, key, 0);

        return value(key);
    }

    /*!
     * zwraca informacje, czy istnieje w slowniku podany klucz
     */
    bool contains(const key_type& key) const {
        Node* node = root;
        while (node != nullptr) {
            if (node->key < key)
                node = node->right;
            else if (node->key > key)
                node = node->left;
            else 
                return true;
        }
        return false;
    }

    /*!
     * zwraca liczbe wpisow w slowniku
     */

    size_t sizet(Node *node) {
        if (node != nullptr) {
            return 1 + sizet(node->left) + sizet(node -> right);
        }
        return 0;
    }

    size_t size() {
        return sizet(root);
    }

    void preOrder(Node *root) {
        if (root != nullptr) {
            std::cout << root->key << "\n";
            preOrder(root->left);
            preOrder(root->right);
        }
    }

    void print() {
        preOrder(root);
    }

};


#include "tests.h"

int main() {
    unit_test();

    vector <string> slownik;
    string slowo;
    fstream in, out1, out2;
    in.open("pan-tadeusz.txt");
    int i = 0;

    while(in >> slowo and i < 100000) {
        slownik.push_back(slowo);
        i++;
    }

    std::cout << "wstawianie" << std::endl;    

    for (int i = 10000; i <= 100000; i+= 10000) {
        TreeMap<string, int> slownikDrzewo;
        Benchmark<> ben = new Benchmark<>(true);
        for(int j = 0; j<i; ++j)
            slownikDrzewo.insert(slownik[i], 1);

        map<string, int> slownikMapa;
        Benchmark<> ben2 = new Benchmark<>(true);
        for(int j = 0; j<i; ++j)
            slownikMapa.insert(make_pair(slownik[i], 1));


        std::cout << std::endl;
    }

    std::cout << "wyszukiwanie" << std::endl; 

    for (int i = 10000; i <= 100000; i+= 10000) {
        TreeMap<string, int> slownikDrzewo;
        Benchmark<> ben = new Benchmark<>(true);
        for(int j = 0; j<i; ++j)
            slownikDrzewo.contains(slownik[i]);

        map<string, int> slownikMapa;
        Benchmark<> ben2 = new Benchmark<>(true);
        for(int j = 0; j<i; ++j)
            slownikMapa.find(slownik[i]);


        std::cout << std::endl;
    }


    in.close();

    return 0;
}

 
