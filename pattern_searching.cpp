#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <cmath>

#define ALPHABET_SIZE 3

char randomChar() {
    return 'a' + rand() % ALPHABET_SIZE;
}

std::string randomize(int len) {
    std::string s = "";
    for (size_t i = 0; i < len; i++)
        s += randomChar();
    return s;
}

// ALGORYTM NAIWNY 
std::vector<int> naive(std::string const & str, std::string const & text) {
    std::vector<int> found;
    size_t str_len = str.size();
    size_t text_len = text.size();

    if (text_len == 0 || str_len == 0) 
        found.push_back(-1);

    else if(text_len < str_len)
        found.push_back(-1);

    else if (text_len == str_len)
        (str == text) ? found.push_back(0) : found.push_back(-1);

    else {
        for (size_t i = 0; i < text_len - str_len + 1; i++)
            if (text.substr(i, str_len) == str) {
                found.push_back(i);
            }
        if (found.empty()) found.push_back(-1);
    }

    return found;
}

// ALGORYTM KMP
std::vector<int> buildShifts(std::string const & str) {
    std::vector<int> shifts;
    size_t str_len = str.size();
    shifts.resize(str_len + 1, 0);
    size_t pos = 0, i = 1;

    while (i < str_len) {
        if (str[pos] == str[i]) {       // jesli znaki sa takie same zwiekszamy liczbe na danej pozycji
            pos++;
            shifts[i++] = pos;
        }
        else if(pos != 0)               // jesli znaki nie sa takie same, ale aktualna pozycja jest różna od 0, to wracamy do znaku spod pozycji pos - 1
            pos = shifts[pos - 1];
        else                            // w przeciwnym wypadku po prostu idziemy dalej
            shifts[i++] = 0;
    }

    return shifts;
}

std::vector<int> KMP(std::string const & str, std::string const & text) {
    std::vector<int> found;
    std::vector<int> shifts = buildShifts(str);
    size_t pos_in_str {0}, pos_in_text {0};
    size_t str_len = str.size();
    size_t text_len = text.size();

    if (text_len == 0 || str_len == 0) 
        found.push_back(-1);

    else if(text_len < str_len)
        found.push_back(-1);

    else if (text_len == str_len)
        (str == text) ? found.push_back(0) : found.push_back(-1);
    
    else {
        while (pos_in_text < text_len) {
            if (str[pos_in_str] == text[pos_in_text]) {
                pos_in_text++;
                pos_in_str++;
                if (pos_in_str == str_len) {
                    found.push_back(pos_in_text - pos_in_str);
                    pos_in_str = shifts[pos_in_str - 1];
                }
            }
            else {
                if (pos_in_str == 0) pos_in_text++;
                else pos_in_str = shifts[pos_in_str - 1];
            }
        }
        if (found.empty()) found.push_back(-1);
    }
    
    
    
    return found;
}

// ALGORYTM BOYER-MOORE'A - NIE DZIALA

std::vector<int> suffTable(std::string const & str) {
    std::vector<int> suff;
    size_t str_len = str.size();
    suff.resize(str_len, 0);

    suff[str_len - 1] = str_len;
    for(size_t i = str_len - 2; i > -1; --i) {
        size_t j {0};
        while (j <= i && str[i - j] == str[str_len - j - 1]) j++;
        suff[i] = j;
    }
    return suff;
}

std::vector<int> badCharShift(std::string const & str) {
    std::vector<int> bcs;
    size_t str_len = str.size();
    bcs.resize(ALPHABET_SIZE, str_len);
    for (size_t i = 0; i < str_len - 1; ++i)
        bcs[str[i]] = str_len - i - 1;
    return bcs;
}

std::vector<int> goodSuffShift(std::string const & str) {
    std::vector<int> gss;
    size_t str_len = str.size();
    gss.resize(ALPHABET_SIZE, str_len);
    std::vector<int> suff = suffTable(str);

    size_t j {0};
    for (size_t i = str_len - 1; i > -1; --i) {
        if (suff[i] == i + 1)
            while (j < str_len - 1 - i) {
                j++;
                gss[j] = str_len - 1 - i;
            }
    }
    for (size_t i = 0; i <= str_len - 2; ++i)
        gss[str_len - 1 - suff[i]] = str_len - 1 - i;
    
    return gss;
}

std::vector<int> BM(std::string const & str, std::string const & text) {
    std::vector<int> found;
    size_t str_len = str.size();
    size_t text_len = text.size();

    if (text_len == 0 || str_len == 0) 
        found.push_back(-1);

    else if(text_len < str_len)
        found.push_back(-1);

    else if (text_len == str_len)
        (str == text) ? found.push_back(0) : found.push_back(-1);

    else {
        std::vector<int> bcs = badCharShift(str);
        std::vector<int> gss = goodSuffShift(str);

        size_t j = 0;
        while (j <= text_len - str_len) {
            size_t i = str_len - 1;
            while (i > -1 && str[i] == text[i + j]) i--;
            if (i < 0) {
                found.push_back(j);
                j += gss[0];
            }
            else j += std::max((int)gss[i], (int)(bcs[text[i + j]] - str_len + 1 + i));
        }
        if (found.empty()) found.push_back(-1);
    }
    return found;
}

// ALGORYTM KARPA-RABINA
int hash(std::string const & str) {
    size_t str_len = str.size();
    int H = 0;
    for (size_t i = 0; i < str_len; i++)
        H = ALPHABET_SIZE * H + (str[i] - 'a');
    return H;
}

std::vector<int> KR(std::string const & str, std::string const & text) {
    std::vector<int> found;
    size_t str_len = str.size();
    size_t text_len = text.size();
    
    int Hs = hash(str);

    if (text_len == 0 || str_len == 0) 
        found.push_back(-1);

    else if(text_len < str_len)
        found.push_back(-1);

    else if (text_len == str_len)
        (str == text) ? found.push_back(0) : found.push_back(-1);

    else {
        int Hs = hash(str);
        int Ht = hash(text.substr(0, str_len));

        size_t i = 0;
        while ( i < text_len - str_len + 1) {
            if ((Ht == Hs) && (str == text.substr(i, str_len)))
                found.push_back(i);
            i++;
            Ht = (Ht - (text[i - 1] - 'a') * pow(ALPHABET_SIZE, str_len - 1)) * ALPHABET_SIZE + text[i + str_len - 1] - 'a';
        }

        if (found.empty()) found.push_back(-1);
    }

    return found;
}

int main() {
    std::vector<int> found;
    srand(time(0));
    std::string text = randomize(50);
    std::string str = randomize(3);
    std::cout << "Text: " << text;
    std::cout << "\nStr (wzorzec): " << str;

    std::cout << "\nNaiwne wyszukiwanie wzorca: ";
    found = naive(str, text);
    for (auto res : found) 
        std::cout << res << " ";
    std::cout << "\n";

    std::cout << "\nWyszukiwanie wzorca KMP: ";
    found = KMP(str, text);
    for (auto res : found) 
        std::cout << res << " ";
    std::cout << "\n";

    /*std::cout << "\nWyszukiwanie wzorca Boyer - Moore: ";
    found = BM(str, text);
    for (auto res : found) 
        std::cout << res << " ";
    std::cout << "\n";*/

    std::cout << "\nWyszukiwanie wzorca KR: ";
    found = KR(str, text);
    for (auto res : found) 
        std::cout << res << " ";
    std::cout << "\n";

    return 0;
}
