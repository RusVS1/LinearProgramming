#pragma once

#include <windows.h>
#include "Functions.h"
#include "ClassM.h"

class SimplexInputError : public std::runtime_error {
public:
    SimplexInputError() : std::runtime_error("SimplexInputError") {
    }
};

class Simplex {
    int n;
    int m;
    std::string task;
    std::vector<std::vector<M>> simplex_table;
    std::vector<std::vector<std::string>> visual_simplex_table;
    std::vector<int> basis;
    std::vector<M> answer;
public:
    Simplex(int n, int m) : n(n), m(m) {
        simplex_table.resize(m + 2, std::vector<M>(n + m + 2, M(0)));
        visual_simplex_table.resize(m + 3, std::vector<std::string>(n + m + 3, ""));
        basis.resize(m, 0);
        answer.resize(n + 1, M(0));
        for (int i = 0; i < m; i++) {
            basis[i] = n + i + 1;
        }
    }
    void CreateTable() {
        std::cout << "Введите ограничения работы машин (пример ввода: 1 2 3 4 <= 10) :" << '\n';
        for (int i = 1; i < m + 1; i++) {
            std::cout << "Ограничение " << i << ": ";
            for (int j = 2; j < n + 2; j++) {
                std::cin >> simplex_table[i][j];
            }
            std::string sign;
            std::cin >> sign;
            std::cin >> simplex_table[i][1];
            if (simplex_table[i][1] < M(0)) {
                for (int j = 2; j < n + 2; j++) {
                    simplex_table[i][j] = -simplex_table[i][j];
                }
                simplex_table[i][1] = -simplex_table[i][1];
                if (sign == "<=") {
                    sign = ">=";
                }
                else {
                    sign = "<=";
                }
            }
            if (sign == "<=") {
                simplex_table[i][n + i + 1] = M(1);
            }
            else if (sign == ">=") {
                simplex_table[i][n + i + 1] = M(-1);
                basis[i - 1] = simplex_table[0].size() - 1;
                for (int j = 0; j < m + 2; j++) {
                    if (j == 0) {
                        simplex_table[j].push_back(M(0, -1));
                        visual_simplex_table[j].push_back("");
                    }
                    else if (j == i) {
                        simplex_table[j].push_back(M(1));
                        visual_simplex_table[j].push_back("");
                    }
                    else {
                        simplex_table[j].push_back(M(0));
                        visual_simplex_table[j].push_back("");
                    }
                }
                visual_simplex_table[m + 2].push_back("");
                simplex_table[i][n + i + 1] = M(-1);
            }
            else if (sign == "=") {
                basis[i - 1] = simplex_table[0].size();
                for (int j = 0; j < m + 2; j++) {
                    if (j == 0) {
                        simplex_table[j].push_back(M(0, -1));
                        visual_simplex_table[j].push_back("");
                    }
                    else if (j == i) {
                        simplex_table[j].push_back(M(1));
                        visual_simplex_table[j].push_back("");
                    }
                    else {
                        simplex_table[j].push_back(M(0));
                        visual_simplex_table[j].push_back("");
                    }
                }
            }
            else {
                throw SimplexInputError{};
            }
        }
        std::cout << "Введите стоимости производства изделий: ";
        for (int i = 2; i < simplex_table[0].size(); i++) {
            if (i - 1 <= n && simplex_table[0][i] == M(0)) {
                std::cin >> simplex_table[0][i];
            }
        }
        std::cout << "Введите задачу (max/min) - ";
        std::cin >> task;
        if (task != "min" && task != "max") {
            throw SimplexInputError{};
        }
        if (task == "min") {
            for (int i = n + 2; i < simplex_table[0].size(); i++) {
                if (simplex_table[0][i] == M(0, -1)) {
                    simplex_table[0][i] = M(0, 1);
                }
            }
        }
        for (int i = 1; i < m + 1; i++) {
            simplex_table[i][0] = simplex_table[0][basis[i - 1] + 1];
        }
        for (int i = 1; i < simplex_table[0].size(); i++) {
            if (i != 1) {
                M sum = M(0);
                for (int j = 1; j < simplex_table.size() - 1; j++) {
                    sum = sum + simplex_table[j][i] * simplex_table[j][0];
                }
                simplex_table[m + 1][i] = sum - simplex_table[0][i];
            }
            else {
                M sum = M(0);
                for (int j = 1; j < simplex_table.size() - 1; j++) {
                    sum = sum + simplex_table[j][1] * simplex_table[j][0];
                }
                simplex_table[m + 1][i] = sum;
            }
        }
        TableToVisual();
    }

    void TableToVisual() {
        for (int i = 0; i < visual_simplex_table.size(); i++) {
            for (int j = 0; j < visual_simplex_table[i].size(); j++) {
                visual_simplex_table[i][j] = "";
            }
        }
        visual_simplex_table[0][0] = "ci";
        for (int i = 0; i < simplex_table.size(); i++) {
            if (i == 0) {
                for (int j = 2; j < simplex_table[i].size(); j++) {
                    visual_simplex_table[i][j + 1] = std::to_string(simplex_table[i][j]);
                }
            }
            else if (i == 1) {
                visual_simplex_table[1][1] = "Bx";
                for (int j = 0; j < simplex_table[i].size() - 1; j++) {
                    visual_simplex_table[i][j + 2] = "A" + std::to_string(j);
                }
                visual_simplex_table[i + 1][0] = std::to_string(simplex_table[i][0]);
                visual_simplex_table[i + 1][1] = "x" + std::to_string(basis[i - 1]);
                for (int j = 1; j < simplex_table[i].size(); j++) {
                    visual_simplex_table[i + 1][j + 1] = std::to_string(simplex_table[i][j]);
                }
            }
            else if (i + 1 == simplex_table.size()) {
                visual_simplex_table[i + 1][1] = u8"\u0394";
                for (int j = 1; j < simplex_table[i].size(); j++) {
                    visual_simplex_table[i + 1][j + 1] = std::to_string(simplex_table[i][j]);
                }
            }
            else {
                visual_simplex_table[i + 1][0] = std::to_string(simplex_table[i][0]);
                visual_simplex_table[i + 1][1] = "x" + std::to_string(basis[i - 1]);
                for (int j = 1; j < simplex_table[i].size(); j++) {
                    visual_simplex_table[i + 1][j + 1] = std::to_string(simplex_table[i][j]);
                }
            }
        }
    }

    void PrintTable() {
        TableToVisual();
        Аlignment(visual_simplex_table);
        SetConsoleOutputCP(65001);
        for (auto i : visual_simplex_table) {
            for (auto j : i) {
                std::cout << j << " ";
            }
            std::cout << '\n';
        }
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);
    }

    bool CalculateTable(bool mode) {
        int index_guide_column = 0;
        M min_delta = M(0);
        for (int i = 2; i < simplex_table[0].size(); i++) {
            if (task == "max") {
                if (simplex_table[simplex_table.size() - 1][i] < M(0) && simplex_table[simplex_table.size() - 1][i] < min_delta) {
                    min_delta = simplex_table[simplex_table.size() - 1][i];
                    index_guide_column = i;
                }
            }
            else {
                if (simplex_table[simplex_table.size() - 1][i] > M(0) && simplex_table[simplex_table.size() - 1][i] > min_delta) {
                    min_delta = simplex_table[simplex_table.size() - 1][i];
                    index_guide_column = i;
                }
            }
        }
        int index_guide_element = 0;
        M guide_element = M(-1);
        for (int i = 1; i < m + 1; i++) {
            if (simplex_table[i][index_guide_column] > M(0) && (simplex_table[i][1] / simplex_table[i][index_guide_column] < guide_element || guide_element == M(-1))) {
                M a = simplex_table[i][1] / simplex_table[i][index_guide_column];
                index_guide_element = i;
                guide_element = simplex_table[i][1] / simplex_table[i][index_guide_column];
            }
        }
        if (guide_element == M(-1)) {
            return false;
        }
        guide_element = simplex_table[index_guide_element][index_guide_column];
        std::vector<std::vector<M>> new_simplex_table = simplex_table;
        basis[index_guide_element - 1] = index_guide_column - 1;
        for (int i = 1; i < m + 1; i++) {
            new_simplex_table[i][0] = simplex_table[0][basis[i - 1] + 1];
        }
        for (int i = 1; i <= m + 1; i++) {
            if (i == index_guide_element) {
                new_simplex_table[i][index_guide_column] = M(1);
            }
            else {
                new_simplex_table[i][index_guide_column] = M(0);
            }
        }
        for (int i = 1; i < new_simplex_table.size(); i++) {
            for (int j = 1; j < new_simplex_table[i].size(); j++) {
                if (i == index_guide_element) {
                    if (j != index_guide_column) {
                        new_simplex_table[i][j] = simplex_table[i][j] / guide_element;
                    }
                }
                else if (j != index_guide_column) {
                    new_simplex_table[i][j] = simplex_table[i][j] - (simplex_table[index_guide_element][j] * simplex_table[i][index_guide_column]) / guide_element;
                }
            }
        }
        for (int i = 1; i < simplex_table[0].size(); i++) {
            if (i != 1) {
                M sum = M(0);
                for (int j = 1; j < simplex_table.size() - 1; j++) {
                    sum = sum + new_simplex_table[j][i] * new_simplex_table[j][0];
                }
                new_simplex_table[m + 1][i] = sum - new_simplex_table[0][i];
            }
            else {
                M sum = M(0);
                for (int j = 1; j < simplex_table.size() - 1; j++) {
                    sum = sum + new_simplex_table[j][1] * new_simplex_table[j][0];
                }
                new_simplex_table[m + 1][i] = sum;
            }
        }
        simplex_table = new_simplex_table;
        if (mode) {
            std::cout << "Выводим - " << visual_simplex_table[index_guide_element + 1][1] << " , Вводим - x" << index_guide_column - 1 << '\n' << '\n';
        }
        return true;
    }
    void PrintAnswer() {
        if (simplex_table[m + 1][1].GetInf() != 0) {
            std::cout << '\n' << "Нет оптимального решения" << '\n';
        }
        else if (simplex_table[m - 1][1] > M(0)) {
            for (int i = 0; i < basis.size(); i++) {
                if (basis[i] <= n) {
                    answer[basis[i] - 1] = simplex_table[i + 1][1];
                }
            }
            answer[n] = simplex_table[m + 1][1];
            std::cout << '\n' << "Найдено оптимальное решение:" << '\n';
            for (int i = 0; i < n; i++) {
                std::cout << "x" << i + 1 << " = " << answer[i] << " , ";
            }
            std::cout << "F = " << answer[n] << '\n';
        }
        else {
            std::cout << '\n' << "Нет оптимального решения" << '\n';
        }
    }

    void CalculateSimplex(bool mode) {
        int iter = 0;
        bool optimal_solution = false;
        if (mode) {
            std::cout << "Итерация " << iter << ":" << '\n';
            PrintTable();
            iter++;
        }
        while (!optimal_solution) {
            if (CalculateTable(mode)) {
                if (mode) {
                    std::cout << "Итерация " << iter << ":" << '\n';
                    PrintTable();
                    iter++;
                }
            }
            else {
                std::cout << '\n' << "Нет оптимального решения" << '\n';
                return;
            }
            bool all_is_positive = true;
            for (int i = 2; i < simplex_table[0].size(); i++) {
                if (task == "max") {
                    if (simplex_table[m + 1][i] < M(0)) {
                        all_is_positive = false;
                    }
                }
                else {
                    if (simplex_table[m + 1][i] > M(0)) {
                        all_is_positive = false;
                    }
                }
            }
            if (all_is_positive) {
                optimal_solution = true;
                PrintAnswer();
            }
        }
    }
};

Simplex SimplexMethod() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int n;
    int m;
    std::cout << "Введите количеcтво видов продукции - ";
    std::cin >> n;
    std::cout << "Введите количеcтво машин - ";
    std::cin >> m;
    Simplex simplex(n, m);
    simplex.CreateTable();
    std::string mode;
    std::cout << "Выводить итерации? [Y/n] - ";
    std::cin >> mode;
    if (mode == "Y") {
        std::cout << '\n';
        simplex.CalculateSimplex(true);
    }
    else {
        simplex.CalculateSimplex(false);
    }
    return simplex;
}