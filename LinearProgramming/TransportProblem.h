#pragma once

#include <windows.h>
#include <algorithm>

#include "Functions.h"
#include "Rational.h"

const Rational INF = 10000;

std::pair<std::vector<Rational>, std::vector<Rational>> GetDifferens(std::vector<std::vector<Rational>>& transport_costs) {
    std::vector<Rational> row_differens;
    std::vector<Rational> col_differens;

    for (int i = 0; i < transport_costs.size(); i++) {
        std::vector<Rational> arr = transport_costs[i];
        std::sort(arr.begin(), arr.end());
        row_differens.push_back(arr[1] - arr[0]);
    }

    for (int col = 0; col < transport_costs[0].size(); col++) {
        std::vector<Rational> arr;
        for (int i = 0; i < transport_costs.size(); i++) {
            arr.push_back(transport_costs[i][col]);
        }
        std::sort(arr.begin(), arr.end());
        col_differens.push_back(arr[1] - arr[0]);
    }
    return make_pair(row_differens, col_differens);
}

void PrintMatrix(std::vector<std::vector<Rational>> matrix) {
    std::vector<std::vector<std::string>>visual_matrix = std::to_string(matrix);
    Аlignment(visual_matrix);
    for (auto i : visual_matrix) {
        for (auto j : i) {
            std::cout << j << " ";
        }
        std::cout << '\n';
    }
}

class TransportProblem {
    int m;
    int n;
    std::vector<std::vector<Rational>> transportation_costs;
    std::vector<std::vector<Rational>> reference_plan;
    std::vector<Rational> suppliers;
    std::vector<Rational> consumers;
public:
    TransportProblem(int m, int n) : m(m), n(n) {
        suppliers.resize(m);
        consumers.resize(n);
        transportation_costs.resize(m, std::vector<Rational>(n));
    }

    void FillTransport() {
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);
        std::cout << "Введите ресурсы поставщиков: ";
        for (int i = 0; i < m; i++) {
            std::cin >> suppliers[i];
        }
        std::cout << "Введите запросы потребителей: ";
        for (int i = 0; i < n; i++) {
            std::cin >> consumers[i];
        }
        std::cout << "Введите матрицу стоимостей перевозок:\n";
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                std::cin >> transportation_costs[i][j];
            }
        }
    }

    void CheckBalance() {
        Rational suppliers_sum = std::accumulate(suppliers.begin(), suppliers.end(), Rational(0));
        Rational consumers_sum = std::accumulate(consumers.begin(), consumers.end(), Rational(0));
        if (suppliers_sum > consumers_sum) {
            for (int i = 0; i < m; i++) {
                transportation_costs[i].push_back(0);
            }
            n += 1;
            consumers.push_back(suppliers_sum - consumers_sum);
        }
        else if (consumers_sum > suppliers_sum) {
            transportation_costs.push_back(std::vector<Rational>(n, 0));
            m += 1;
            suppliers.push_back(consumers_sum - suppliers_sum);
        }
    }

    void PrintVogel(std::vector<std::vector<Rational>> diff_row, std::vector<std::vector<Rational>> diff_col) {
        std::cout << "Опорный план перевозок: \n";
        std::vector<std::vector<std::string>> visul_diff_row = std::to_string(diff_row);
        std::vector<std::vector<std::string>> visul_diff_col = std::to_string(diff_col);
        std::vector<std::vector<std::string>> transpos_visul_diff_row(visul_diff_row[0].size(), std::vector<std::string>(visul_diff_row.size()));
        for (int i = 0; i < visul_diff_row.size(); i++) {
            for (int j = 0; j < visul_diff_row[0].size(); j++) {
                transpos_visul_diff_row[j][i] = visul_diff_row[i][j];
            }
        }
        std::vector<std::vector<std::string>> visual_plan = std::to_string(reference_plan);
        for (int i = 0; i < transpos_visul_diff_row.size(); i++) {
            transpos_visul_diff_row[i].insert(transpos_visul_diff_row[i].cbegin(), std::to_string(suppliers[i]));
        }
        for (int i = 0; i < visual_plan.size(); i++) {
            for (int j = 0; j < visual_plan[i].size(); j++) {
                visual_plan[i][j] = std::to_string(transportation_costs[i][j]) + "|" + visual_plan[i][j];
            }
        }
        visual_plan.push_back(std::to_string(consumers));
        for (auto i : visul_diff_col) {
            visual_plan.push_back(i);
        }
        Аlignment(visual_plan);
        Аlignment(transpos_visul_diff_row);
        for (int i = 0; i < visual_plan.size(); i++) {
            if (i < transpos_visul_diff_row.size()) {
                for (auto j : visual_plan[i]) {
                    std::cout << j << " ";
                }
                for (auto j : transpos_visul_diff_row[i]) {
                    std::cout << j << " ";
                }
                std::cout << '\n';
            }
            else {
                for (auto j : visual_plan[i]) {
                    std::cout << j << " ";
                }
                std::cout << '\n';
            }
        }
        std::cout << "-------------------------------\n";
    }

    void ReferencePlanVogel(bool mode) {
        std::vector<Rational> tmp_suppliers = suppliers;
        std::vector<Rational> tmp_consumers = consumers;
        std::vector<std::vector<Rational>> tmp_transportation_costs = transportation_costs;
        std::vector<std::vector<Rational>> save_diff_row;
        std::vector<std::vector<Rational>> save_diff_col;
        reference_plan.resize(m, std::vector<Rational>(n, 0));
        while (*max_element(tmp_suppliers.begin(), tmp_suppliers.end()) != 0 || *max_element(tmp_consumers.begin(), tmp_consumers.end()) != 0) {
            std::pair<std::vector<Rational>, std::vector<Rational>> diffs = GetDifferens(tmp_transportation_costs);
            std::vector<Rational> row = diffs.first;
            std::vector<Rational> col = diffs.second;
            save_diff_row.push_back(row);
            save_diff_col.push_back(col);
            Rational max_row = *max_element(row.begin(), row.end());
            Rational max_col = *max_element(col.begin(), col.end());
            if (max_row >= max_col) {
                for (int i = 0; i < row.size(); i++) {
                    if (row[i] == max_row) {
                        Rational min_first = *min_element(tmp_transportation_costs[i].begin(), tmp_transportation_costs[i].end());
                        for (int j = 0; j < tmp_transportation_costs[i].size(); j++) {
                            if (tmp_transportation_costs[i][j] == min_first) {
                                Rational min_second = min(tmp_suppliers[i], tmp_consumers[j]);
                                reference_plan[i][j] = min_second;
                                tmp_suppliers[i] -= min_second;
                                tmp_consumers[j] -= min_second;
                                if (tmp_consumers[j] == 0) {
                                    for (int r = 0; r < m; r++) {
                                        tmp_transportation_costs[r][j] = INF;
                                    }
                                }
                                else {
                                    fill(tmp_transportation_costs[i].begin(), tmp_transportation_costs[i].end(), INF);
                                }
                                break;
                            }
                        }
                        break;
                    }
                }
            }
            else {
                for (int i = 0; i < col.size(); i++) {
                    if (col[i] == max_col) {
                        Rational min_first = INF;
                        for (int j = 0; j < m; j++) {
                            min_first = min(min_first, tmp_transportation_costs[j][i]);
                        }
                        for (int j = 0; j < m; j++) {
                            if (tmp_transportation_costs[j][i] == min_first) {
                                Rational min_second = min(tmp_suppliers[j], tmp_consumers[i]);
                                reference_plan[j][i] = min_second;
                                tmp_suppliers[j] -= min_second;
                                tmp_consumers[i] -= min_second;
                                if (tmp_consumers[i] == 0) {
                                    for (int r = 0; r < m; r++) {
                                        tmp_transportation_costs[r][i] = INF;
                                    }
                                }
                                else {
                                    fill(tmp_transportation_costs[j].begin(), tmp_transportation_costs[j].end(), INF);
                                }
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        }
        if (mode) {
            PrintVogel(save_diff_row, save_diff_col);
        }
    }

    bool DegenerateSolution() {
        int positive_elem_plan = 0;
        for (int m = 0; m < reference_plan.size(); ++m) {
            for (int n = 0; n < reference_plan[0].size(); ++n) {
                if (reference_plan[m][n] > 0) {
                    positive_elem_plan += 1;
                }
            }
        }
        return reference_plan.size() + reference_plan[0].size() - 1 != positive_elem_plan;
    }

    std::vector<Rational> DeleteColumns(std::vector<Rational>& columns, std::vector<Rational>& rows) {
        std::vector<Rational> result;
        for (auto i : columns) {
            int count = 0;
            for (auto j : rows) {
                if (reference_plan[j.GetNumerator()][i.GetNumerator()] != 0) {
                    count++;
                    if (count > 1) {
                        break;
                    }
                }
            }
            if (count <= 1) {
                result.push_back(i);
            }
        }
        return result;
    }

    std::vector<Rational> DeleteRows(std::vector<Rational>& columns, std::vector<Rational>& rows) {
        std::vector<Rational> result;
        for (auto j : rows) {
            int count = 0;
            for (auto i : columns) {
                if (reference_plan[j.GetNumerator()][i.GetNumerator()] != 0) {
                    count++;
                    if (count > 1) {
                        break;
                    }
                }
            }
            if (count <= 1) {
                result.push_back(j);
            }
        }
        return result;
    }

    std::vector<std::vector<Rational>> CycleSearch() {
        std::vector<Rational> columns(reference_plan[0].size());
        std::vector<Rational> rows(reference_plan.size());
        for (int i = 0; i < reference_plan[0].size(); ++i) {
            columns[i] = i;
        }
        for (int i = 0; i < reference_plan.size(); ++i) {
            rows[i] = i;
        }
        bool cycle = false;
        while (!cycle && (columns.size() > 1 && rows.size() > 1)) {
            std::vector<Rational> array = DeleteColumns(columns, rows);
            if (array.empty()) {
                cycle = true;
            }
            for (auto j : array) {
                columns.erase(std::remove(columns.begin(), columns.end(), j), columns.end());
            }

            array = DeleteRows(columns, rows);
            if (!array.empty()) {
                cycle = false;
            }
            for (auto j : array) {
                rows.erase(std::remove(rows.begin(), rows.end(), j), rows.end());
            }
        }
        std::vector<std::vector<Rational>> cycle_coordinates;
        if (cycle) {
            for (auto i : rows) {
                if (cycle_coordinates.empty()) {
                    for (auto j : columns) {
                        if (reference_plan[i.GetNumerator()][j.GetNumerator()] != 0) {
                            cycle_coordinates.push_back({ i, j });
                            break;
                        }
                    }
                }
            }
            while (cycle_coordinates[0] != cycle_coordinates.back() || cycle_coordinates.size() < 3) {
                if (cycle_coordinates.size() % 2 != 0) {
                    Rational i = cycle_coordinates.back()[0];
                    Rational old_j = cycle_coordinates.back()[1];
                    for (auto j : columns) {
                        if (j != old_j && reference_plan[i.GetNumerator()][j.GetNumerator()] != 0) {
                            cycle_coordinates.push_back({ i, j });
                        }
                    }
                }
                else {
                    Rational j = cycle_coordinates.back()[1];
                    Rational old_i = cycle_coordinates.back()[0];
                    for (auto i : rows) {
                        if (i != old_i && reference_plan[i.GetNumerator()][j.GetNumerator()] != 0) {
                            cycle_coordinates.push_back({ i, j });
                        }
                    }
                }
            }
            cycle_coordinates.pop_back();
        }
        return cycle_coordinates;
    }


    void DeleteDegeneracy() {
        for (int i = 0; i < reference_plan.size(); ++i) {
            for (int j = 0; j < reference_plan[i].size(); ++j) {
                if (reference_plan[i][j] == 0) {
                    reference_plan[i][j] = Rational(1, 1000);
                    if (!CycleSearch().empty()) {
                        return;
                    }
                    else {
                        reference_plan[i][j] = 0;
                    }
                }
            }
        }
    }

    std::pair<std::vector<Rational>, std::vector<Rational>> PotentialCalculation() {
        std::vector<Rational> potential_u(reference_plan.size(), 10000);
        std::vector<Rational> potential_v(reference_plan[0].size(), 10000);
        potential_u[0] = 0;
        bool in_u = true;
        bool in_v = true;
        while (in_u || in_v) {
            in_u = false;
            in_v = false;
            for (int u = 0; u < potential_u.size(); ++u) {
                if (potential_u[u] != 10000) {
                    for (int v = 0; v < potential_v.size(); ++v) {
                        if (reference_plan[u][v] != 0 && potential_v[v] == 10000) {
                            potential_v[v] = potential_u[u] + transportation_costs[u][v];
                            in_v = true;
                        }
                    }
                }
            }
            for (int v = 0; v < potential_v.size(); ++v) {
                if (potential_v[v] != 10000) {
                    for (int u = 0; u < potential_u.size(); ++u) {
                        if (reference_plan[u][v] != 0 && potential_u[u] == 10000) {
                            potential_u[u] = potential_v[v] - transportation_costs[u][v];
                            in_u = true;
                        }
                    }
                }
            }
        }
        return std::make_pair(potential_u, potential_v);
    }

    std::pair<Rational, std::pair<int, int>> MinCost(std::vector<std::vector<Rational>>& matrix) {
        int x = 0, y = 0;
        Rational min_elem = matrix[x][y];

        for (int m = 0; m < matrix.size(); ++m) {
            for (int n = 0; n < matrix[m].size(); ++n) {
                if (min_elem > matrix[m][n]) {
                    min_elem = matrix[m][n];
                    x = m;
                    y = n;
                }
            }
        }
        return std::make_pair(min_elem, std::make_pair(x, y));
    }

    std::vector<std::vector<Rational>> CalculateCostsMatrix() {
        std::vector<std::vector<Rational>> costs_matrix(transportation_costs.size(), std::vector<Rational>(transportation_costs[0].size(), 0));
        std::vector<Rational> potential_u = PotentialCalculation().first;
        std::vector<Rational> potential_v = PotentialCalculation().second;
        for (int i = 0; i < transportation_costs.size(); ++i) {
            for (int j = 0; j < transportation_costs[i].size(); ++j) {
                costs_matrix[i][j] = transportation_costs[i][j] + potential_u[i] - potential_v[j];
            }
        }
        return costs_matrix;
    }

    std::vector<std::vector<Rational>> RecalculationPlan(int min_i, int min_j) {
        reference_plan[min_i][min_j] = 1;
        std::vector<std::vector<Rational>> cycle_elem = CycleSearch();
        std::vector<std::vector<Rational>> new_reference_plan(reference_plan.size(), std::vector<Rational>(reference_plan[0].size(), 0));
        for (int m = 0; m < reference_plan.size(); ++m) {
            new_reference_plan[m] = reference_plan[m];
        }
        reference_plan[min_i][min_j] = 0;
        Rational minus_ind = 0;
        Rational theta = INF;
        for (int j = 0; j < cycle_elem.size(); ++j) {
            if (cycle_elem[j][0] == min_i && cycle_elem[j][1] == min_j) {
                minus_ind = (j - 1) % 2;
                break;
            }
        }
        for (int i = minus_ind.GetNumerator(); i < cycle_elem.size(); i += 2) {
            theta = min(reference_plan[cycle_elem[i][0].GetNumerator()][cycle_elem[i][1].GetNumerator()], theta);
        }
        for (int i = (minus_ind.GetNumerator() + 1) % 2; i < cycle_elem.size(); i += 2) {
            new_reference_plan[cycle_elem[i][0].GetNumerator()][cycle_elem[i][1].GetNumerator()] = reference_plan[cycle_elem[i][0].GetNumerator()][cycle_elem[i][1].GetNumerator()] + theta;
        }
        for (int i = minus_ind.GetNumerator(); i < cycle_elem.size(); i += 2) {
            new_reference_plan[cycle_elem[i][0].GetNumerator()][cycle_elem[i][1].GetNumerator()] = reference_plan[cycle_elem[i][0].GetNumerator()][cycle_elem[i][1].GetNumerator()] - theta;
        }
        return new_reference_plan;
    }

    Rational GetCost() {
        Rational result = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                result += reference_plan[i][j] * transportation_costs[i][j];
            }
        }
        return result;
    }

    void PotentialsMethod(bool mode) {
        while (DegenerateSolution()) {
            DeleteDegeneracy();
        }
        std::vector<std::vector<Rational>> costs_matrix = CalculateCostsMatrix();
        if (mode) {
            std::cout << "Итерация 0\n";
            std::cout << "X0\n";
            PrintMatrix(reference_plan);
            std::cout << "\n";
            std::cout << "C0\n";
            PrintMatrix(costs_matrix);
            std::cout << "--------------------------\n";
        }
        int iter = 1;
        while (MinCost(costs_matrix).first < 0) {
            int i = MinCost(costs_matrix).second.first;
            int j = MinCost(costs_matrix).second.second;
            reference_plan = RecalculationPlan(i, j);
            while (DegenerateSolution()) {
                DeleteDegeneracy();
            }
            costs_matrix = CalculateCostsMatrix();
            if (mode) {
                std::cout << "Итерация " << iter << '\n';
                std::cout << "X" << iter << '\n';
                PrintMatrix(reference_plan);
                std::cout << "\n";
                std::cout << "C" << iter << '\n';
                PrintMatrix(costs_matrix);
                std::cout << "--------------------------\n";
                iter++;
            }
        }
        std::cout << "План оптимален. Суммарная стоимость перевозок(L) = " << GetCost() << '\n';
        std::cout << "Оптимальный план перевозок:\n";
        PrintMatrix(reference_plan);
    }
};

void Transport() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int m;
    std::cout << "Введите количество поставщиков: ";
    std::cin >> m;
    int n;
    std::cout << "Введите количество потребителей: ";
    std::cin >> n;
    TransportProblem t(m, n);
    t.FillTransport();
    bool mode = true;
    char ans;
    std::cout << "Выводить ход решения[Y/n] - ";
    std::cin >> ans;
    if (ans == 'n') {
        mode = false;
    }
    t.CheckBalance();
    t.ReferencePlanVogel(mode);
    t.PotentialsMethod(mode);
}