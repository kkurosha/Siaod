#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace std;

//линейный поиск первого вхождения подстроки в строку
int lin_search(string& str, string& substr, int& s) { 
    int c = s;
    int len = str.length();
    int s_len = substr.length();
    int start = clock();
    for (int i = 0; i <= len - s_len; i++) {
        if (str.substr(i, s_len) == substr) { //совпадает ли подстрока с i длиной s_len
            //c += s_len;
            //cout << "Сравнения: " << c << endl;
            cout << "Время выполнения линейного алгоритма: " << clock() - start << " мс\n";
            return i; //возвращаем индекс первого вхождения
        }
    }
    //cout << "Сравнения: " << c << endl;
    return -1; //если не найдено
}

//алгоритм Бойера-Мура-Хорспула для поиска последнего вхождения подстроки
//last_ind - таблица с последними индексами символом строки
void preprocess(const string& substr, vector<int>& last_ind) { //ф. для обработки подстроки и заполнения таблицы
    int n = substr.length(); //длина подстроки
    for (int i = 0; i < 256; i++) last_ind[i] = -1; //инициализация
    for (int i = 0; i < n; i++) {
        last_ind[(int)substr[i]] = i; //заполнение таблицы
    }
}

int bhm_search(const string& str, const string& substr, int& q) {
    int len = str.length();
    int s_len = substr.length();
    int c = q;

    int start = clock();
    if (s_len > len) {
        cout << "Подстрока длиннее строки\n";
        return -1; //подстрока длиннее строки
    }

    vector<int> last_ind(256); //таблица, в которой хранятся последние индексы символов строки
    preprocess(substr, last_ind); 
    int i = s_len - 1; //стартуем с конца подстроки
    int j = s_len - 1; //индекс символа в подстроке
    int index = -1; //для хранения последнего найденного индекса

    while (i < len) {
        if (str[i] == substr[j]) { //совпадают ли символы в текущих индексах строки и подстроки
            if (j == 0) { //значит нашли совпадение всей подстроки
                //c += s_len;
                index = i; // обновляем последний найденный индекс
                i += s_len; // сдвигаем индекс для продолжения поиска
                j = s_len - 1; // обнуляем j, чтобы искать снова
            }
            else {  //если совпадений нет
                i--; //смещаемся влево
                j--;
            }
        }
        else { //если символы не совпадают, используем таблицу, 
            i += s_len - min(j, 1 + last_ind[(int)str[i]]); //основываясь на последнем индексе текущего символа
            j = s_len - 1; //обнуляем j
        }
    }
    //cout << "Сравнения: " << c << endl;
    cout << "Время выполнения БМХ-алгоритма: " << clock() - start << " мс\n";
    return index; // возвращаем последний найденный индекс
}


int main() {
    int s = 0;
    int q = 0;
    setlocale(LC_ALL, "rus");
    string str;
    cout << "Введите строку: ";
    getline(cin, str);
    string substr;
    cout << "Введите подстроку, которую нужно найти: ";
    getline(cin, substr);

    int index1 = lin_search(str, substr, s);
    cout << "Первое вхождение подстроки начинается с индекса: " << index1 << endl;
    int index2 = bhm_search(str, substr, q);
    cout << "Последнее вхождение подстроки начинается с индекса: " << index2 << endl;
    
}

// asdfghjkl asdfghsad asdjkl fgh fgh asdfgh a

//Since almost the beginning of cinema, we have had scary films. Of all the genres that exist, horror is perhaps one of the most conventional. Many horror films rely on specific plot devices, also called tropes, to make their audience frightened. When a trope is used too much, it can become a cliché. But when used well, it can really make us jump out of our skin. Here are some of the most used, and perhaps abused, clichés in horror films. No matter what kind of house it is, the basement is a scary place in horror films. That's usually where something is hiding or where the evil psychopath has hidden their tools. Basements are always dark and often damp. You can only reach them by a narrow staircase. And basements are always creepy, even when there isn't anything down there. In older horror films, when protagonists were in desperation, it was difficult or impossible for them to call for help or call the police. Mobile phones have made that situation a bit less believable now. What's the solution to maintain suspense? No phone coverage! If you're a hero in a horror film, it's almost certain that at a key moment, just when you absolutely need to call for help, you will not have any coverage at all. Or your phone battery will die just as you are making the call. Or both.
