#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

const int BLOCK_HEIGHT = 4;
const int WIDTH = 10;
const int BOARD_HEIGHT = 20;

void printMatrix(const vector<vector<char>>& matrix) {
    for (const auto& row : matrix) {
        for (char cell : row) {
            if (cell == 'X')
                cout << ' ';
            else
                cout << cell;
            // cout << cell;
        }
        cout << endl;
    }
}

bool canPlaceBlock(const vector<vector<char>>& board, const vector<vector<char>>& block, int startRow, int startCol) {
    for (int i = 0; i < BLOCK_HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (block[i][j] != 'X' && board[startRow + i][startCol + j] != 'X') {
                return false;
            }
        }
    }
    return true;
}

void placeBlock(vector<vector<char>>& board, const vector<vector<char>>& block, int startRow, int startCol) {
    for (int i = 0; i < BLOCK_HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (block[i][j] != 'X') {
                board[startRow + i][startCol + j] = block[i][j];
            }
        }
    }
}

int main() {
    vector<vector<char>> block(BLOCK_HEIGHT, vector<char>(WIDTH, 'X'));
    vector<vector<char>> board(BOARD_HEIGHT, vector<char>(WIDTH, 'X'));

    int status;
    while (cin >> status) {
        // 讀取方塊矩陣
        for (int i = 0; i < BLOCK_HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                cin >> block[i][j];
            }
        }

        // 查找方塊可以放置的最底部位置
        int dropRow = 0;
        for (int i = 0; i <= BOARD_HEIGHT - BLOCK_HEIGHT; ++i) {
            if (!canPlaceBlock(board, block, i, 0)) {
                break;
            }
            dropRow = i;
        }

        // 將方塊放置在盤面上
        placeBlock(board, block, dropRow, 0);

        // 清空方塊矩陣
        for (int i = 0; i < BLOCK_HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                block[i][j] = 'X';
            }
        }
    }
    printMatrix(board);


    return 0;
}