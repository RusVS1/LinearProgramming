#include <iostream>
#include <windows.h>

#include "Simplex.h"
#include "TransportProblem.h"

void LinearProgramming() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::cout << "Выберите дейтсвие!\n";
    int key;
    std::cout << "Решить линейную задачу - 1\n";
    std::cout << "Решить транспортную задачу - 2\n";
    std::cout << "Ввод - ";
    std::cin >> key;
    if (key == 1) {
        SimplexMethod();
    }
    else if (key == 2) {
        Transport();
    }
    else {
        std::cout << "Неверный ввод!";
    }
}


int main() {
    LinearProgramming();
}

/*
4
4
1 1 0 0 <= 6
0 0 1 1 <= 6
6 0 13 0 >= 30
0 24 0 13 >= 96
4 47 13 26
min
Y
*/

/*
3
4
120 280 160
130 220 60 70
1 7 9 5
4 2 6 8
3 8 1 2
Y
*/
