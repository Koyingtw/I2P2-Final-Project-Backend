#pragma once
#include <vector>
#include <string>

typedef std::vector<std::vector<char>> Matrix;

Matrix stringToMatrix(const std::string str);
std::string matrixToString(const Matrix& matrix);
void printMatrix(const std::vector<std::vector<char>>& matrix);
bool canPlaceBlock(const std::vector<std::vector<char>>& board, const std::vector<std::vector<char>>& block, int startRow, int startCol);
int placeBlock(std::vector<std::vector<char>>& board, const std::vector<std::vector<char>>& block, int startRow, int startCol);
std::string generateBlock();
char generateBlockID();
std::string generateBlockString(char blockID);
char transToBlockID(const Matrix& block);