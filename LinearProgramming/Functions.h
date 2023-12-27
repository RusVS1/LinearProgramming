#pragma once

#include <string>
#include <vector>

#include "Rational.h"

int Count(std::string str, char key) {
    int count = 0;
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == key) {
            count++;
        }
    }
    return count;
}

namespace std {
    std::vector<std::vector<std::string>> to_string(std::vector<std::vector<Rational>> matrix) {
        std::vector<std::vector<std::string>> visual_matrix(matrix.size(), std::vector<std::string>(matrix[0].size()));
        for (int i = 0; i < matrix.size(); i++) {
            for (int j = 0; j < matrix[i].size(); j++) {
                if (matrix[i][j] > 8000) {
                    visual_matrix[i][j] = "-";
                }
                else {
                    visual_matrix[i][j] = std::to_string(matrix[i][j]);
                }
            }
        }
        return visual_matrix;
    }

    std::vector<std::string> to_string(std::vector<Rational> vector) {
        std::vector<std::string> visual_vector(vector.size());
        for (int i = 0; i < vector.size(); i++) {
            visual_vector[i] = std::to_string(vector[i]);
        }
        return visual_vector;
    }
}

void Àlignment(std::vector<std::vector<std::string>>& matrix) {
    for (int j = 0; j < matrix[0].size(); j++) {
        int max_len = 0;
        for (int i = 0; i < matrix.size(); i++) {
            if (matrix[i][j] == u8"\u0394") {
                if (matrix[i][j].size() - 1 > max_len) {
                    max_len = matrix[i][j].size() - 1;
                }
            }
            else {
                if (matrix[i][j].size() > max_len) {
                    max_len = matrix[i][j].size();
                }
            }
        }
        for (int i = 0; i < matrix.size(); i++) {
            if (matrix[i][j] == u8"\u0394") {
                while (matrix[i][j].size() - 1 < max_len) {
                    matrix[i][j] = matrix[i][j] + " ";
                }
            }
            else {
                while (matrix[i][j].size() < max_len) {
                    matrix[i][j] = matrix[i][j] + " ";
                }
            }
        }
    }
}