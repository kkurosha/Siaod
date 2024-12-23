#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

int countSequencesDP(int n) { //кол-во после-тей методом дп
    if (n == 1) return 2;
    if (n == 2) return 3;
    vector<int> dp(n + 1);
    dp[1] = 2; //для длины 1
    dp[2] = 3; //2
    //таблица дп
    for (int i = 3; i <= n; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }
    return dp[n];
}

int countSequencesBruteForce(int n) { //перебор всех посл-тей длины n
    int total = 0;
    int combinations = pow(2, n); //всего 2^n последовательностей
    //перебор всех возможных комбинаций
    for (int i = 0; i < combinations; i++) {
        bool valid = true;
        for (int j = 0; j < n - 1; j++) {
            //проверка на наличие двух подряд идущих единиц
            if ((i & (1 << j)) && (i & (1 << (j + 1)))) {
                valid = false;
                break;
            }
        }
        if (valid) total++;
    }
    return total;
}

int main() {
    setlocale(LC_ALL, "rus");
    int n;
    cout << "Введите длину последовательности n: ";
    cin >> n;
    int resultDP = countSequencesDP(n); 
    cout << "\nКоличество последовательностей (метод ДП): " << resultDP << endl;
    int resultBruteForce = countSequencesBruteForce(n);
    cout << "Количество последовательностей (перебор): " << resultBruteForce << endl;
    cout << "\nКоличество переборов для ДП: " << n << endl;
    cout << "Количество переборов для грубой силы: " << pow(2, n) << endl;
}










