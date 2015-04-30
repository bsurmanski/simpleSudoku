#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

uint8_t board[9][9];

void read_board(FILE *f) {
    memset(board, 0, sizeof(uint8_t) * 9 * 9);

    int i = 0;
    int j = 0;
    while(!feof(f)) {
        int c = fgetc(f);
        if(c == '\n') {
            i = 0;
            j++;
            continue;
        } else if(isspace(c)) {
            i++;
        } else if(isdigit(c)) {
            int num = c - '0';
            assert(num > 0 && num < 9);
            board[j][i] = num;
            i++;
        }
    }
}

void print_board(void) {
    int i;
    int j;
    for(j = 0; j < 9; j++) {
        if(j > 0 && j % 3 == 0) {
            printf("-----------------------\n");
        }
        for(i = 0; i < 9; i++) {
            if(i > 0 && i % 3 == 0) {
                printf(" | ");
            }
            printf("%d ", board[j][i]);
        }
        printf("\n");
    }
}

int next_square(int *ni, int *nj) {
    int j;
    int i;
    if(*ni + 1 >= 9) {
        *ni = 0;
        *nj = *nj + 1;
    } else {
        (*ni)++;
    }

    for(j = *nj; j < 9; j++) {
        for(i = *ni; i < 9; i++) {
            if(board[j][i] == 0) {
                *ni = i;
                *nj = j;
                return 1;
            }
        }
    }
    return 0;
}

int next_candidate(int ni, int nj, int *candidate) {
    int c;
    int i;
    int j;

    for(c = *candidate + 1; c < 10; c++) {
        for(i = 0; i < 9; i++) {
            if(board[nj][i] == c) {
                goto NEXT;
            }
        }

        for(j = 0; j < 9; j++) {
            if(board[j][ni] == c) {
                goto NEXT;
            }
        }

        int si = ni / 3;
        int sj = nj / 3;
        for(j = 0; j < 3; j++) {
            for(i = 0; i < 3; i++) {
                if(board[sj*3 + j][si*3 + i] == c) {
                    goto NEXT;
                }
            }
        }

        *candidate = c;
        return 1;
NEXT:
        ;
    }
    return 0;
}

int board_complete() {
    int i;
    int j;

    for(j = 0; j < 9; j++) {
        for(i = 0; i < 9; i++) {
            if(board[j][i] == 0) return 0;
        }
    }
    return 1;
}

int board_valid() {
    int i;
    int j;
    int k;

    for(j = 0; j < 9; j++) {
        for(i = 0; i < 9; i++) {
            if(board[j][i] == 0) return 0;
            for(k = 0; k < 9; k++) {
                if(k != i && board[j][k] == board[j][i]) return 0;
                if(k != j && board[k][i] == board[j][i]) return 0;
            }
        }
    }
    return 1;
}

int solve(void) {
    int ni = -1;
    int nj = 0;
    int candidate = 0;

    if(next_square(&ni, &nj)) {
        while(next_candidate(ni, nj, &candidate)) {
            board[nj][ni] = candidate;
            if(board_valid()) return 1;
            if(solve()) return 1; // DFS recursive
            board[nj][ni] = 0;
        }
    }

    return 0;
}

int main(int argc, char **argv) {
    FILE *input = fopen("input", "r");
    read_board(input);
    fclose(input);

    print_board();
    solve();
    printf("\n");
    print_board();
    return 0;
}
