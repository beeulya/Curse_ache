#pragma warning(disable:4996)
#pragma warning(disable:4703)
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include <GLFW/stb_easy_font.h>
#include <time.h>
#define GL_SILENCE_DEPRECATION
#define INCLUDE_STB_EASY_FONT_H
#define INT_MIN -1000
#define INT_MAX 1000
#define INFINITY 1000
#define WIN_LEN 5

typedef enum symbol {
    pusto = '_', //0
    nolik = 'O', //1
    krestik = 'X', //2
    dot = '.' //3
} symbol;

typedef struct {
    symbol sign;
    short priority;
    short line_priority_X;
    short line_priority_O;
    short block;
} square;

typedef struct {
    short coordx;
    short coordy;
    short priority;
    short flag;
} squarePriority;

//
symbol player_move;
symbol bot_move;
//

//-------------------------------------------   OPENGL  -----------------------------------------
void putKrestik(short x, short y, short kolvoKletok, short windowSize) {
    short med = kolvoKletok / 2;
    GLfloat coordx, coordy, shag, delta;
    shag = 2.0f / kolvoKletok;
    coordx = 0.0f - (med - x) * shag;
    coordy = (med - y) * shag;
    delta = shag * 0.7 * 0.5;

    glColor3f(0.953f, 0.539f, 0.184f);
    glLineWidth(10);
    glBegin(GL_LINES);
    glVertex2f(coordx - delta, coordy - delta);
    glVertex2f(coordx + delta, coordy + delta);
    glVertex2f(coordx - delta, coordy + delta);
    glVertex2f(coordx + delta, coordy - delta);
    glEnd();
}

void putNolik(short x, short y, short kolvoKletok, short windowSize) {
    short med = kolvoKletok / 2;
    GLfloat coordx, coordy, shag, rad1, rad2;
    shag = 2.0f / kolvoKletok;
    coordx = 0.0f - (med - x) * shag;
    coordy = (med - y) * shag;
    rad1 = shag * 0.8 * 0.5;
    rad2 = rad1 * 0.6;

    glColor3f(1.0f, 0.349f, 0.561f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(coordx, coordy);
    for (short i = 0; i <= 50; i++) {
        float a = (float)i / 50.0f * 3.1415f * 2.0f;
        glVertex2f(coordx + cos(a) * rad1, coordy + sin(a) * rad1);
    }
    glEnd();
    glColor3f(0.62f, 0.906f, 0.961f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(coordx, coordy);
    for (short i = 0; i <= 50; i++) {
        float a = (float)i / 50.0f * 3.1415f * 2.0f;
        glVertex2f(coordx + cos(a) * rad2, coordy + sin(a) * rad2);
    }
    glEnd();
}

void print_string(float x, float y, char* text, float r, float g, float b)
{
    static char buffer[99999]; // ~500 chars
    short num_quads;

    num_quads = stb_easy_font_print(x, y, text, NULL, buffer, sizeof(buffer));

    glColor3f(r, g, b);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 16, buffer);
    glDrawArrays(GL_QUADS, 0, num_quads * 4);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void mainScreen(short windowSize) {
    glEnable(GL_SCISSOR_TEST);
    glClearColor(0.62f, 0.906f, 0.961f, 1.0f);
    glScissor(0, 0, windowSize, windowSize);
    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_SCISSOR_TEST);

    print_string(-57, -45, "NOUGHTS AND CROSSES", 0.137f, 0.431f, 0.588f);
    print_string(-9.5, -22, "Play", 0.055f, 0.663f, 0.306f);
    glColor3f(0.055f, 0.663f, 0.306f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-13.0, 0.0);
    glVertex2f(-13.0, 30.0);
    glVertex2f(17.0, 15.0);
    glEnd();


}

void chooseDesk(short windowSize) {
    glEnable(GL_SCISSOR_TEST);
    glClearColor(0.62f, 0.906f, 0.961f, 1.0f);
    glScissor(0, 0, windowSize, windowSize);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    print_string(-55, -50, "Select the field size", 0.137f, 0.431f, 0.588f);
    glColor3f(0.137f, 0.431f, 0.588f);
    glBegin(GL_QUADS);
    glVertex2f(-45.0, -35.0);
    glVertex2f(-45.0, -15.0);
    glVertex2f(45.0, -15.0);
    glVertex2f(45.0, -35.0);
    glEnd();
    print_string(-20, -28, "15 x 15", 1.0f, 0.349f, 0.561f);
    glColor3f(0.137f, 0.431f, 0.588f);
    glBegin(GL_QUADS);
    glVertex2f(-45.0, -10.0);
    glVertex2f(-45.0, 10.0);
    glVertex2f(45.0, 10.0);
    glVertex2f(45.0, -10.0);
    glEnd();
    print_string(-20, -3, "25 x 25", 0.953f, 0.539f, 0.184f);
    glColor3f(0.137f, 0.431f, 0.588f);
    glBegin(GL_QUADS);
    glVertex2f(-45.0, 15.0);
    glVertex2f(-45.0, 35.0);
    glVertex2f(45.0, 35.0);
    glVertex2f(45.0, 15.0);
    glEnd();
    print_string(-20, 22, "35 x 35", 1.0f, 0.349f, 0.561f);
}

void chooseSymbol(short windowSize) {
    glEnable(GL_SCISSOR_TEST);
    glClearColor(0.62f, 0.906f, 0.961f, 1.0f);
    glScissor(0, 0, windowSize, windowSize);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    print_string(-53, -50, "Choose what do you \n  want to play as", 0.137f, 0.431f, 0.588f);
    glColor3f(0.137f, 0.431f, 0.588f);
    glBegin(GL_QUADS);
    glVertex2f(-50.0, -10.0);
    glVertex2f(-50.0, 30.0);
    glVertex2f(-10.0, 30.0);
    glVertex2f(-10.0, -10.0);
    glEnd();

    glColor3f(0.137f, 0.431f, 0.588f);
    glBegin(GL_QUADS);
    glVertex2f(10.0, -10.0);
    glVertex2f(10.0, 30.0);
    glVertex2f(50.0, 30.0);
    glVertex2f(50.0, -10.0);
    glEnd();

    glColor3f(0.953f, 0.539f, 0.184f);
    glLineWidth(40);
    glBegin(GL_LINES);
    glVertex2f(-50 + 5, -10 + 5);
    glVertex2f(-10 - 5, 30 - 5);
    glVertex2f(-10 - 5, -10 + 5);
    glVertex2f(-50 + 5, 30 - 5);
    glEnd();

    glColor3f(1.0f, 0.349f, 0.561f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(30, 10);
    for (short i = 0; i <= 50; i++) {
        float a = (float)i / 50.0f * 3.1415f * 2.0f;
        glVertex2f(30 + cos(a) * 18, 10 + sin(a) * 18);
    }
    glEnd();
    glColor3f(0.137f, 0.431f, 0.588f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(30, 10);
    for (short i = 0; i <= 50; i++) {
        float a = (float)i / 50.0f * 3.1415f * 2.0f;
        glVertex2f(30 + cos(a) * 12, 10 + sin(a) * 12);
    }
    glEnd();
}

void victory(short windowSize) {
    glEnable(GL_SCISSOR_TEST);
    glClearColor(0.62f, 0.906f, 0.961f, 1.0f);
    glScissor(0, 0, windowSize, windowSize);
    glClear(GL_COLOR_BUFFER_BIT);
    glScalef(0.016, -0.016, 1);
    glDisable(GL_SCISSOR_TEST);

    print_string(-41, -38, "   You won!\nCongratulations!", 0.137f, 0.431f, 0.588f);


    glColor3f(1.0f, 0.843f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(15, 6);
    for (short i = 0; i <= 50; i++) {
        float a = (float)i / 50.0f * 3.1415f * 2.0f;
        glVertex2f(15 + cos(a) * 9, 6 + sin(a) * 9);
    }
    glEnd();
    glColor3f(0.62f, 0.906f, 0.961f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(15, 6);
    for (short i = 0; i <= 50; i++) {
        float a = (float)i / 50.0f * 3.1415f * 2.0f;
        glVertex2f(15 + cos(a) * 5, 6 + sin(a) * 5);
    }
    glEnd();
    glColor3f(1.0f, 0.843f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(-15, 6);
    for (short i = 0; i <= 50; i++) {
        float a = (float)i / 50.0f * 3.1415f * 2.0f;
        glVertex2f(-15 + cos(a) * 9, 6 + sin(a) * 9);
    }
    glEnd();
    glColor3f(0.62f, 0.906f, 0.961f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(-15, 6);
    for (short i = 0; i <= 50; i++) {
        float a = (float)i / 50.0f * 3.1415f * 2.0f;
        glVertex2f(-15 + cos(a) * 5, 6 + sin(a) * 5);
    }
    glEnd();

    glColor3f(1.0f, 0.843f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0, 10);
    for (short i = 0; i <= 50; i++) {
        float a = (float)i / 50.0f * 3.1415f * 2.0f;
        glVertex2f(0 + cos(a) * 15, 10 + sin(a) * 15);
    }
    glEnd();
    glColor3f(1.0f, 0.843f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-15.0, -7.0);
    glVertex2f(-15.0, 10.0);
    glVertex2f(15.0, 10.0);
    glVertex2f(15.0, -7.0);
    glEnd();
    glColor3f(1.0f, 0.843f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-3.0, 10.0);
    glVertex2f(-3.0, 40.0);
    glVertex2f(3.0, 40.0);
    glVertex2f(3.0, 10.0);
    glEnd();
    glColor3f(1.0f, 0.843f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-10.0, 40.0);
    glVertex2f(-10.0, 47.0);
    glVertex2f(10.0, 47.0);
    glVertex2f(10.0, 40.0);
    glEnd();
}

void fail(short windowSize) {
    glEnable(GL_SCISSOR_TEST);
    glClearColor(0.62f, 0.906f, 0.961f, 1.0f);
    glScissor(0, 0, windowSize, windowSize);
    glClear(GL_COLOR_BUFFER_BIT);
    glScalef(0.016, -0.016, 1);
    glDisable(GL_SCISSOR_TEST);

    print_string(-41, -30, "Unfortunately,\n you lost", 0.137f, 0.431f, 0.588f);
}

void renderDesk(short kolvoKletok, short windowSize) {
    glEnable(GL_SCISSOR_TEST);
    glClearColor(0.62f, 0.906f, 0.961f, 1.0f);
    glScissor(0, 0, windowSize, windowSize);
    glClear(GL_COLOR_BUFFER_BIT);

    for (short i = 1; i < kolvoKletok; i++) {
        glEnable(GL_SCISSOR_TEST);
        glClearColor(0.137f, 0.431f, 0.588f, 1.0f);
        float coordx = (float)i * (windowSize / kolvoKletok);
        glScissor(coordx - 2.5, 0, 5, windowSize);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    for (short i = 1; i < kolvoKletok; i++) {
        glEnable(GL_SCISSOR_TEST);
        glClearColor(0.137f, 0.431f, 0.588f, 1.0f);
        float coordy = (float)i * (windowSize / kolvoKletok);
        glScissor(0, coordy - 2.5, windowSize, 5);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    glDisable(GL_SCISSOR_TEST);
}


//�������� ��� ������ �� ����
void printDesk(square** desk, short kolvoKletok, GLFWwindow* window, short windowSize) {
    renderDesk(kolvoKletok, windowSize);
    for (short i = 0; i < kolvoKletok; i++) {
        for (short j = 0; j < kolvoKletok; j++) {
            if (desk[i][j].sign == 'X') putKrestik(j, i, kolvoKletok, windowSize);
            if (desk[i][j].sign == 'O') putNolik(j, i, kolvoKletok, windowSize);
        }
    }
    glfwSwapBuffers(window);
    glClear(GL_COLOR_BUFFER_BIT);
}

//�������, ������������� ��������� ������������ ���� �����
short check_win_Diagonal1(square** desk, short kolvoKletok, short coordx, short coordy, short flag) {
    symbol symb = desk[coordx][coordy].sign;
    short count = 0;
    short x = coordx;
    short y = coordy;
    while (x < kolvoKletok && y < kolvoKletok) {
        if (symb == desk[x][y].sign) count++;
        else {
            if (desk[x][y].sign != pusto) flag = 1;
            break;
        }
        x++;
        y++;
    }
    x = coordx - 1;
    y = coordy - 1;
    while (x >= 0 && y >= 0) {
        if (symb == desk[x][y].sign) count++;
        else {
            if (desk[x][y].sign != pusto) flag = 1;
            break;
        }
        x--;
        y--;
    }
    return count;
}
short check_win_Diagonal2(square** desk, short kolvoKletok, short coordx, short coordy, short flag) {
    symbol symb = desk[coordx][coordy].sign;
    short count = 0;
    short x = coordx;
    short y = coordy;
    while (x < kolvoKletok && y >= 0) {
        if (symb == desk[x][y].sign) count++;
        else {
            if (desk[x][y].sign != pusto) flag = 1;
            break;
        }
        x++;
        y--;
    }
    x = coordx - 1;
    y = coordy + 1;
    while (x >= 0 && y < kolvoKletok) {
        if (symb == desk[x][y].sign) count++;
        else {
            if (desk[x][y].sign != pusto) flag = 1;
            break;
        }
        x--;
        y++;
    }
    return count;
}
symbol checkDiagonals(square** desk, short kolvoKletok) { //check if there are any win position (for minimax)
    short i, j, k;
    symbol winner;
    for (i = 0; i <= kolvoKletok - WIN_LEN; i++) {
        for (j = 0; j <= kolvoKletok - WIN_LEN; j++) {
            if (desk[i][j].sign == krestik || desk[i][j].sign == nolik) {
                winner = desk[i][j].sign;
                for (k = 1; k < WIN_LEN; k++) {
                    if (desk[i + k][j + k].sign != winner)
                    {
                        break;
                    }
                }
                if (k == WIN_LEN) {
                    return winner;
                }
            }
            if (desk[i][j + WIN_LEN - 1].sign == krestik || desk[i][j + WIN_LEN - 1].sign == nolik) {
                winner = desk[i][j + WIN_LEN - 1].sign;
                for (k = 1; k < WIN_LEN; k++) {
                    if (desk[i + k][j + WIN_LEN - 1 - k].sign != winner) {
                        break;
                    }
                }
                if (k == WIN_LEN) {
                    return winner;
                }
            }
        }
    }
    winner = pusto;
    return winner;
}
short check_win_Gorizont(square** desk, short kolvoKletok, short coordx, short coordy, short flag) {
    symbol symb = desk[coordx][coordy].sign;
    short count = 0;
    short x = coordx;
    short y = coordy;
    while (x < kolvoKletok) {
        if (symb == desk[x][y].sign) count++;
        else {
            if (desk[x][y].sign != pusto) flag = 1;
            break;
        }
        x++;
    }
    x = coordx - 1;
    while (x >= 0) {
        if (symb == desk[x][y].sign) count++;
        else {
            if (desk[x][y].sign != pusto) flag = 1;
            break;
        }
        x--;
    }
    return count;
}
symbol checkGorizont(square** desk, short kolvoKletok) { //check if there are any win position (for minimax)
    short i, j, k;
    symbol winner;
    for (i = 0; i < kolvoKletok; i++) {
        for (j = 0; j <= kolvoKletok - WIN_LEN; j++) {
            if (desk[i][j].sign == krestik || desk[i][j].sign == nolik) {
                winner = desk[i][j].sign;
                for (k = 1; k < WIN_LEN; k++) {
                    if (desk[i][j + k].sign != winner) {
                        break;
                    }
                }
                if (k == WIN_LEN) {
                    return winner;
                }
            }
        }
    }
    winner = pusto;
    return winner;
}
short check_win_Vertikal(square** desk, short kolvoKletok, short coordx, short coordy, short flag) {
    symbol symb = desk[coordx][coordy].sign;
    short count = 0;
    short x = coordx;
    short y = coordy;
    while (y < kolvoKletok) {
        if (symb == desk[x][y].sign) count++;
        else {
            if (desk[x][y].sign != pusto) flag = 1;
            break;
        }
        y++;
    }
    y = coordy - 1;
    while (y >= 0) {
        if (symb == desk[x][y].sign) count++;
        else {
            if (desk[x][y].sign != pusto) flag = 1;
            break;
        }
        y--;
    }
    return count;
}
symbol checkVertikal(square** desk, short kolvoKletok) { //check if there are any win position (for minimax)
    short i, j, k;
    symbol winner;
    for (i = 0; i < kolvoKletok - WIN_LEN; i++) {
        for (j = 0; j < kolvoKletok; j++) {
            if (desk[i][j].sign == krestik || desk[i][j].sign == nolik) {
                winner = desk[i][j].sign;
                for (k = 1; k < WIN_LEN; k++) {
                    if (desk[i + k][j].sign != winner) {
                        break;
                    }
                }
                if (k == WIN_LEN) {
                    return winner;
                }
            }
        }
    }
    winner = pusto;
    return winner;
}
//���� ����� ������������ ����������� � ��������������� ��� ������ ����
void checkLines(square** desk, short kolvoKletok, short coordx, short coordy) {
    squarePriority count;
    count.priority = 0;
    count.flag = 0;
    short flag = 0;
    short countDiag1 = check_win_Diagonal1(desk, kolvoKletok, coordx, coordy, flag);
    if (countDiag1 >= count.priority) {
        if (desk[coordx + 1][coordy + 1].sign == pusto && coordx + 1 < kolvoKletok && coordy + 1 < kolvoKletok) {
            count.priority = countDiag1;
            count.coordx = coordx + 1;
            count.coordy = coordy + 1;
            count.flag = flag;
            desk[count.coordx][count.coordy].priority = count.priority;
            desk[count.coordx][count.coordy].block = count.flag;
            if (desk[coordx][coordy].sign == krestik && desk[count.coordx][count.coordy].line_priority_X < count.priority)
                desk[count.coordx][count.coordy].line_priority_X = count.priority;
            else if (desk[coordx][coordy].sign == nolik && desk[count.coordx][count.coordy].line_priority_O < count.priority)
                desk[count.coordx][count.coordy].line_priority_O = count.priority;

        }
        if (desk[coordx - 1][coordy - 1].sign == pusto && coordx - 1 >= 0 && coordy - 1 >= 0) {
            count.priority = countDiag1;
            count.coordx = coordx - 1;
            count.coordy = coordy - 1;
            count.flag = flag;
            desk[count.coordx][count.coordy].priority = count.priority;
            desk[count.coordx][count.coordy].block = count.flag;
            if (desk[coordx][coordy].sign == krestik && desk[count.coordx][count.coordy].line_priority_X < count.priority)
                desk[count.coordx][count.coordy].line_priority_X = count.priority;
            else if (desk[coordx][coordy].sign == nolik && desk[count.coordx][count.coordy].line_priority_O < count.priority)
                desk[count.coordx][count.coordy].line_priority_O = count.priority;
        }
    }
    flag = 0;
    short countDiag2 = check_win_Diagonal2(desk, kolvoKletok, coordx, coordy, flag);
    if (countDiag2 >= count.priority) {
        if (desk[coordx + 1][coordy - 1].sign == pusto && coordx + 1 < kolvoKletok && coordy - 1 >= 0) {
            count.priority = countDiag2;
            count.coordx = coordx + 1;
            count.coordy = coordy - 1;
            count.flag = flag;
            desk[count.coordx][count.coordy].priority = count.priority;
            desk[count.coordx][count.coordy].block = count.flag;
            if (desk[coordx][coordy].sign == krestik && desk[count.coordx][count.coordy].line_priority_X < count.priority)
                desk[count.coordx][count.coordy].line_priority_X = count.priority;
            else if (desk[coordx][coordy].sign == nolik && desk[count.coordx][count.coordy].line_priority_O < count.priority)
                desk[count.coordx][count.coordy].line_priority_O = count.priority;
        }
        if (desk[coordx - 1][coordy + 1].sign == pusto && coordx - 1 >= 0 && coordy + 1 < kolvoKletok) {
            count.priority = countDiag2;
            count.coordx = coordx - 1;
            count.coordy = coordy + 1;
            count.flag = flag;
            desk[count.coordx][count.coordy].priority = count.priority;
            desk[count.coordx][count.coordy].block = count.flag;
            if (desk[coordx][coordy].sign == krestik && desk[count.coordx][count.coordy].line_priority_X < count.priority)
                desk[count.coordx][count.coordy].line_priority_X = count.priority;
            else if (desk[coordx][coordy].sign == nolik && desk[count.coordx][count.coordy].line_priority_O < count.priority)
                desk[count.coordx][count.coordy].line_priority_O = count.priority;
        }
    }
    flag = 0;
    short countGor = check_win_Gorizont(desk, kolvoKletok, coordx, coordy, flag);
    if (countGor >= count.priority) {
        if (desk[coordx + 1][coordy].sign == pusto && coordx + 1 < kolvoKletok) {
            count.priority = countGor;
            count.coordx = coordx + 1;
            count.coordy = coordy;
            count.flag = flag;
            desk[count.coordx][count.coordy].priority = count.priority;
            desk[count.coordx][count.coordy].block = count.flag;
            if (desk[coordx][coordy].sign == krestik && desk[count.coordx][count.coordy].line_priority_X < count.priority)
                desk[count.coordx][count.coordy].line_priority_X = count.priority;
            else if (desk[coordx][coordy].sign == nolik && desk[count.coordx][count.coordy].line_priority_O < count.priority)
                desk[count.coordx][count.coordy].line_priority_O = count.priority;
        }
        if (desk[coordx - 1][coordy].sign == pusto && coordx - 1 >= 0) {
            count.priority = countGor;
            count.coordx = coordx - 1;
            count.coordy = coordy;
            count.flag = flag;
            desk[count.coordx][count.coordy].priority = count.priority;
            desk[count.coordx][count.coordy].block = count.flag;
            if (desk[coordx][coordy].sign == krestik && desk[count.coordx][count.coordy].line_priority_X < count.priority)
                desk[count.coordx][count.coordy].line_priority_X = count.priority;
            else if (desk[coordx][coordy].sign == nolik && desk[count.coordx][count.coordy].line_priority_O < count.priority)
                desk[count.coordx][count.coordy].line_priority_O = count.priority;
        }
    }
    flag = 0;
    short countVertik = check_win_Vertikal(desk, kolvoKletok, coordx, coordy, flag);
    if (countVertik >= count.priority) {
        if (desk[coordx][coordy + 1].sign == pusto && coordy + 1 < kolvoKletok) {
            count.priority = countVertik;
            count.coordx = coordx;
            count.coordy = coordy + 1;
            count.flag = flag;
            desk[count.coordx][count.coordy].priority = count.priority;
            desk[count.coordx][count.coordy].block = count.flag;
            if (desk[coordx][coordy].sign == krestik && desk[count.coordx][count.coordy].line_priority_X < count.priority)
                desk[count.coordx][count.coordy].line_priority_X = count.priority;
            else if (desk[coordx][coordy].sign == nolik && desk[count.coordx][count.coordy].line_priority_O < count.priority)
                desk[count.coordx][count.coordy].line_priority_O = count.priority;
        }
        if (desk[coordx][coordy - 1].sign == pusto && coordy - 1 >= 0) {
            count.priority = countVertik;
            count.coordx = coordx;
            count.coordy = coordy - 1;
            count.flag = flag;
            desk[count.coordx][count.coordy].priority = count.priority;
            desk[count.coordx][count.coordy].block = count.flag;
            if (desk[coordx][coordy].sign == krestik && desk[count.coordx][count.coordy].line_priority_X < count.priority)
                desk[count.coordx][count.coordy].line_priority_X = count.priority;
            else if (desk[coordx][coordy].sign == nolik && desk[count.coordx][count.coordy].line_priority_O < count.priority)
                desk[count.coordx][count.coordy].line_priority_O = count.priority;
        }
    }
}

short razrivFlyagi(square** desk, short kolvoKletok, short coordx, short coordy, symbol sign) {
    short priority = 0;
    //��� ���������
    if (desk[coordx - 1][coordy - 1].sign == desk[coordx + 1][coordy + 1].sign && desk[coordx - 1][coordy - 1].sign == sign) {
        short priority1 = check_win_Diagonal1(desk, kolvoKletok, coordx - 1, coordy - 1, 0);
        short priority2 = check_win_Diagonal1(desk, kolvoKletok, coordx + 1, coordy + 1, 0);
        if (priority1 + priority2 >= priority) priority = priority1 + priority2;
    }
    if (desk[coordx + 1][coordy - 1].sign == desk[coordx - 1][coordy + 1].sign && desk[coordx + 1][coordy - 1].sign == sign) {
        short priority1 = check_win_Diagonal2(desk, kolvoKletok, coordx + 1, coordy - 1, 0);
        short priority2 = check_win_Diagonal2(desk, kolvoKletok, coordx - 1, coordy + 1, 0);
        if (priority1 + priority2 >= priority) priority = priority1 + priority2;
    }
    //����������� -- ����� ������ � �����
    if (desk[coordx - 1][coordy].sign == desk[coordx + 1][coordy].sign && desk[coordx - 1][coordy].sign == sign) {
        short priority1 = check_win_Gorizont(desk, kolvoKletok, coordx - 1, coordy, 0);
        short priority2 = check_win_Gorizont(desk, kolvoKletok, coordx + 1, coordy, 0);
        if (priority1 + priority2 >= priority) priority = priority1 + priority2;
    }
    //��������� -- ����� ����� � ������
    if (desk[coordx][coordy - 1].sign == desk[coordx][coordy + 1].sign && desk[coordx][coordy - 1].sign == sign) {
        short priority1 = check_win_Vertikal(desk, kolvoKletok, coordx, coordy - 1, 0);
        short priority2 = check_win_Vertikal(desk, kolvoKletok, coordx, coordy + 1, 0);
        if (priority1 + priority2 >= priority) priority = priority1 + priority2;
    }
    return priority;
}

//
void setPriority(square** desk, short kolvoKletok, short coordx, short coordy) {
    desk[coordx][coordy].priority = 0;
    desk[coordx][coordy].line_priority_X = 0;
    desk[coordx][coordy].line_priority_O = 0;
    desk[coordx][coordy].block = 0;
    for (short i = 1; i < kolvoKletok - 1; i++) {
        for (short j = 1; j < kolvoKletok - 1; j++) {
            if (desk[i][j].sign != pusto) checkLines(desk, kolvoKletok, i, j);

            else {
                short flyaga_X = razrivFlyagi(desk, kolvoKletok, i, j, krestik);
                short flyaga_O = razrivFlyagi(desk, kolvoKletok, i, j, nolik);
                if (desk[i][j].line_priority_O < flyaga_O) desk[i][j].line_priority_O = flyaga_O;
                if (desk[i][j].line_priority_X < flyaga_X) desk[i][j].line_priority_X = flyaga_X;
                if (desk[i][j].priority < flyaga_O) desk[i][j].priority = flyaga_O;
                if (desk[i][j].priority < flyaga_X) desk[i][j].priority = flyaga_X;
            }

        }
    }
}

//���� ����� �������� ������
squarePriority checkPositions(square** desk, short kolvoKletok) {
    squarePriority maxPrioritet;
    maxPrioritet.priority = 0;
    maxPrioritet.coordx = 0;
    maxPrioritet.coordy = 0;
    maxPrioritet.flag = 0;
    for (short i = 1; i < kolvoKletok - 1; i++) {
        for (short j = 1; j < kolvoKletok - 1; j++) {
            if (bot_move == krestik) { //�������� ��� ��������
                if (desk[i][j].line_priority_X == 4 && maxPrioritet.priority < desk[i][j].line_priority_X) { //���� ������� 4 ��������, ������ ����� -- ���������� �������
                    maxPrioritet.priority = desk[i][j].line_priority_X;
                    maxPrioritet.coordx = i;
                    maxPrioritet.coordy = j;
                }
                else if (desk[i][j].line_priority_O == 4 && maxPrioritet.priority < desk[i][j].line_priority_O) { //���� ������� 4 ������, ����������� �����
                    maxPrioritet.priority = desk[i][j].line_priority_O;
                    maxPrioritet.coordx = i;
                    maxPrioritet.coordy = j;
                }
                else if (desk[i][j].line_priority_O == 3 && desk[i][j].block == 0 && maxPrioritet.priority < desk[i][j].line_priority_O) { //���� ������� 3 ������ (����� �� ������������� �� � ����� �� ������), ����������� � ����� �������
                    maxPrioritet.priority = desk[i][j].line_priority_O;
                    maxPrioritet.coordx = i;
                    maxPrioritet.coordy = j;
                }
                else if (desk[i][j].line_priority_X == 3 && maxPrioritet.priority < desk[i][j].line_priority_X) { //������� 3 ���� �������� (����)
                    maxPrioritet.priority = desk[i][j].line_priority_X;
                    maxPrioritet.coordx = i;
                    maxPrioritet.coordy = j;
                }
                else if (desk[i][j].line_priority_O == 3 && desk[i][j].block == 1 && maxPrioritet.priority < desk[i][j].line_priority_O) { //���� ������� 3 ������ (������������� � ����� �������)
                    maxPrioritet.priority = desk[i][j].line_priority_O;
                    maxPrioritet.coordx = i;
                    maxPrioritet.coordy = j;
                }
                else if (desk[i][j].line_priority_X >= maxPrioritet.priority && maxPrioritet.priority < desk[i][j].line_priority_X) { //��������� ������ (������������ ����) -- ����� ���������
                    maxPrioritet.priority = desk[i][j].line_priority_X;
                    maxPrioritet.coordx = i;
                    maxPrioritet.coordy = j;
                }
                else if (desk[i][j].line_priority_O >= maxPrioritet.priority && maxPrioritet.priority < desk[i][j].line_priority_O) { //��������� ������ (������������ ����) -- ����� �������
                    maxPrioritet.priority = desk[i][j].line_priority_O;
                    maxPrioritet.coordx = i;
                    maxPrioritet.coordy = j;
                }
            }
            if (bot_move == nolik) { //�������� ��� ������
                if (desk[i][j].line_priority_O == 4 && maxPrioritet.priority < desk[i][j].line_priority_O) { //���� ������� 4 ������, ������ ����� -- ���������� �������
                    maxPrioritet.priority = desk[i][j].line_priority_O;
                    maxPrioritet.coordx = i;
                    maxPrioritet.coordy = j;
                }
                else if (desk[i][j].line_priority_X == 4 && maxPrioritet.priority < desk[i][j].line_priority_X) { //���� ������� 4 ��������, ����������� �����
                    maxPrioritet.priority = desk[i][j].line_priority_X;
                    maxPrioritet.coordx = i;
                    maxPrioritet.coordy = j;
                }
                else if (desk[i][j].line_priority_X == 3 && desk[i][j].block == 0 && maxPrioritet.priority < desk[i][j].line_priority_X) { //���� ������� 3 �������� (����� �� ������������� �� � ����� �� ������), ����������� � ����� �������
                    maxPrioritet.priority = desk[i][j].line_priority_X;
                    maxPrioritet.coordx = i;
                    maxPrioritet.coordy = j;
                }
                else if (desk[i][j].line_priority_O == 3 && maxPrioritet.priority < desk[i][j].line_priority_O) { //������� 3 ���� ������ (����)
                    maxPrioritet.priority = desk[i][j].line_priority_O;
                    maxPrioritet.coordx = i;
                    maxPrioritet.coordy = j;
                }
                else if (desk[i][j].line_priority_X == 3 && desk[i][j].block == 1 && maxPrioritet.priority < desk[i][j].line_priority_X) { //���� ������� 3 �������� (������������� � ����� �������)
                    maxPrioritet.priority = desk[i][j].line_priority_X;
                    maxPrioritet.coordx = i;
                    maxPrioritet.coordy = j;
                }
                else if (desk[i][j].line_priority_O >= maxPrioritet.priority && maxPrioritet.priority < desk[i][j].line_priority_O) { //��������� ������ (������������ ����) -- ����� �������
                    maxPrioritet.priority = desk[i][j].line_priority_O;
                    maxPrioritet.coordx = i;
                    maxPrioritet.coordy = j;
                }
                else if (desk[i][j].line_priority_X >= maxPrioritet.priority && maxPrioritet.priority < desk[i][j].line_priority_X) { //��������� ������ (������������ ����) -- ����� ���������
                    maxPrioritet.priority = desk[i][j].line_priority_X;
                    maxPrioritet.coordx = i;
                    maxPrioritet.coordy = j;
                }
            }
        }
    }
    return maxPrioritet;
}

//-------------------------------------

symbol check_state(square** desk, short kolvoKletok) {
    if (desk == NULL) return pusto;
    symbol state;
    state = checkGorizont(desk, kolvoKletok);
    if (state != pusto) {
        return state;
    }

    state = checkVertikal(desk, kolvoKletok);
    if (state != pusto) {
        return state;
    }
    state = checkDiagonals(desk, kolvoKletok);
    if (state != pusto) {
        return state;
    }
    for (short i = 0; i < kolvoKletok; ++i) {
        for (short j = 0; j < kolvoKletok; ++j) {
            if (desk[i][j].sign == pusto || desk[i][j].sign == dot) {
                return 4; // found en empty square, CONTINUE GAME
            }
        }
    }
    return 0; // DRAW
}

void putDot(square** desk, short kolvoKletok) {
    for (short i = 0; i < kolvoKletok; i++) {
        for (short j = 0; j < kolvoKletok; j++) {
            if (desk[i][j].sign != pusto) continue;
            if (i - 1 > 0) {
                if (desk[i - 1][j].sign == 'X' || desk[i - 1][j].sign == 'O') {
                    desk[i][j].sign = dot;
                    continue;
                }
                if (j - 1 > 0 && (desk[i - 1][j - 1].sign == 'X' || desk[i - 1][j - 1].sign == 'O')) {
                    desk[i][j].sign = dot;
                    continue;
                }
                if (j + 1 < kolvoKletok && (desk[i - 1][j + 1].sign == 'X' || desk[i - 1][j + 1].sign == 'O')) {
                    desk[i][j].sign = dot;
                    continue;
                }
            }
            if (i + 1 < kolvoKletok) {
                if (desk[i + 1][j].sign == 'X' || desk[i + 1][j].sign == 'O') {
                    desk[i][j].sign = dot;
                    continue;
                }
                if (j - 1 > 0 && (desk[i + 1][j - 1].sign == 'X' || desk[i + 1][j - 1].sign == 'O')) {
                    desk[i][j].sign = dot;
                    continue;
                }
                if (j + 1 < kolvoKletok && (desk[i + 1][j + 1].sign == 'X' || desk[i + 1][j + 1].sign == 'O')) {
                    desk[i][j].sign = dot;
                    continue;
                }
            }
            if (j - 1 > 0 && (desk[i][j - 1].sign == 'X' || desk[i][j - 1].sign == 'O')) {
                desk[i][j].sign = dot;
                continue;
            }
            if (j + 1 < kolvoKletok && (desk[i][j + 1].sign == 'X' || desk[i][j + 1].sign == 'O')) {
                desk[i][j].sign = dot;
                continue;
            }
        }
    }
}

void eraseDot(square** desk, short kolvoKletok) {
    for (short i = 0; i < kolvoKletok; i++) {
        for (short j = 0; j < kolvoKletok; j++) {
            if (desk[i][j].sign == dot) desk[i][j].sign = pusto;
        }
    }
}


short minimax(short depth, symbol player, short alpha, short beta, short kolvoKletok, square** desk) {
    if (depth < 4) {
        symbol stat = check_state(desk, kolvoKletok);
        if (stat == bot_move) return 100 - depth;
        else if (stat == player_move) return -100 + depth;
        else if (stat == 0) return 0;
        if (player == bot_move) {
            short best_score = -INFINITY;
            for (short i = 0; i < kolvoKletok; i++) {
                for (short j = 0; j < kolvoKletok; j++) {
                    if (desk[i][j].sign == dot) {
                        desk[i][j].sign = bot_move;
                        putDot(desk, kolvoKletok);
                        short score = minimax(depth + 1, player_move, alpha, beta, kolvoKletok, desk);
                        desk[i][j].sign = pusto;
                        eraseDot(desk, kolvoKletok);
                        putDot(desk, kolvoKletok);
                        best_score = fmax(best_score, score);
                        alpha = fmax(best_score, alpha);
                        if (alpha >= beta) return best_score;
                    }
                }
                if (alpha >= beta) return best_score;
            }
            return best_score;
        }
        else {
            short best_score = INFINITY;
            for (short i = 0; i < kolvoKletok; i++) {
                for (short j = 0; j < kolvoKletok; j++) {
                    if (desk[i][j].sign == dot) {
                        desk[i][j].sign = player_move;
                        putDot(desk, kolvoKletok);
                        short score = minimax(depth + 1, bot_move, alpha, beta, kolvoKletok, desk);
                        desk[i][j].sign = pusto;
                        eraseDot(desk, kolvoKletok);
                        putDot(desk, kolvoKletok);
                        best_score = fmin(best_score, score);
                        beta = fmin(beta, best_score);
                        if (beta <= alpha) return best_score;
                    }
                }
                if (beta <= alpha) return best_score;
            }
            return best_score;
        }
    }
    return 0;
}

squarePriority get_move_minimax(square** desk, short kolvoKletok) {
    short best_score = -INFINITY;
    squarePriority bestMove;
    bestMove.coordx = -1;
    bestMove.coordy = -1;
    short alpha = -INFINITY;
    short beta = INFINITY;
    for (short i = 0; i < kolvoKletok; i++) {
        for (short j = 0; j < kolvoKletok; j++) {
            if (desk[i][j].sign == dot) {
                desk[i][j].sign = bot_move;
                putDot(desk, kolvoKletok);
                short score = minimax(0, player_move, alpha, beta, kolvoKletok, desk);
                desk[i][j].sign = pusto;
                eraseDot(desk, kolvoKletok);
                putDot(desk, kolvoKletok);

                if (score > best_score && best_score > 0) {
                    best_score = score;
                    bestMove.coordx = i;
                    bestMove.coordy = j;
                }
            }
        }
    }
    return bestMove;
}


//�������, ����������� ������� ������
short checkWin(square** desk, short kolvoKletok, short windowSize, GLFWwindow* window, short option) {
    for (short i = 0; i < kolvoKletok; i++) {
        for (short j = 0; j < kolvoKletok; j++) {
            short pos_X = 0;
            short pos_O = 0;
            if (desk[i][j].sign == krestik) {
                if (pos_X < check_win_Diagonal1(desk, kolvoKletok, i, j, 0)) pos_X = check_win_Diagonal1(desk, kolvoKletok, i, j, 0);
                if (pos_X < check_win_Diagonal2(desk, kolvoKletok, i, j, 0)) pos_X = check_win_Diagonal2(desk, kolvoKletok, i, j, 0);
                if (pos_X < check_win_Gorizont(desk, kolvoKletok, i, j, 0)) pos_X = check_win_Gorizont(desk, kolvoKletok, i, j, 0);
                if (pos_X < check_win_Vertikal(desk, kolvoKletok, i, j, 0)) pos_X = check_win_Vertikal(desk, kolvoKletok, i, j, 0);
            }
            if (desk[i][j].sign == nolik) {
                if (pos_O < check_win_Diagonal1(desk, kolvoKletok, i, j, 0)) pos_O = check_win_Diagonal1(desk, kolvoKletok, i, j, 0);
                if (pos_O < check_win_Diagonal2(desk, kolvoKletok, i, j, 0)) pos_O = check_win_Diagonal2(desk, kolvoKletok, i, j, 0);
                if (pos_O < check_win_Gorizont(desk, kolvoKletok, i, j, 0)) pos_O = check_win_Gorizont(desk, kolvoKletok, i, j, 0);
                if (pos_O < check_win_Vertikal(desk, kolvoKletok, i, j, 0)) pos_O = check_win_Vertikal(desk, kolvoKletok, i, j, 0);
            }
            if (pos_O >= WIN_LEN) { // 5
                if (option == 1) return 6;
                else return 5;
            }
            else if (pos_X >= WIN_LEN) { // 5
                if (option == 1) return 5;
                else return 6;
            }
        }
    }
    return 4;
}

void do_first_step(square** desk, short kolvoKletok) {
    squarePriority bot_next_move = checkPositions(desk, kolvoKletok);
    short coordx = bot_next_move.coordx;
    short coordy = bot_next_move.coordy;
    desk[coordx][coordy].sign = bot_move;
    setPriority(desk, kolvoKletok, coordx, coordy);
}


int main(void) {
    short kolvoKletok = 0;
    square** desk;
    short option = 0;

    GLFWwindow* window;
    short windowSize = 1050;

    if (!glfwInit())return -1;

    window = glfwCreateWindow(windowSize, windowSize, "Krestik-nolic2024", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glClear(GL_COLOR_BUFFER_BIT);
    glfwMakeContextCurrent(window);
    double x, y;
    short flag = 0;
    glClear(GL_COLOR_BUFFER_BIT);
    short step = 0;
    short coordx = 0, coordy = 0;
    glScalef(0.016, -0.016, 1);
    mainScreen(windowSize);
    glfwSwapBuffers(window);
    while (!glfwWindowShouldClose(window)) {

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
            glfwGetCursorPos(window, &x, &y);
            flag = 1;
        }
        else if (flag == 1) {
            flag = 0;
            if (step == 0) {
                glClear(GL_COLOR_BUFFER_BIT);
                if (x > (windowSize / 2) * 0.8 && x < (windowSize / 2) * 1.3 && y >(double)windowSize / 2 && y < (windowSize / 2) * 1.45) {
                    step++;
                    chooseDesk(windowSize);
                    glfwSwapBuffers(window);
                }
            }
            else if (step == 1) {
                glClear(GL_COLOR_BUFFER_BIT);
                if (x > (windowSize / 2) * 0.28 && x < (windowSize / 2) * 1.72 && y >(windowSize / 2) * 0.45 && y < (windowSize / 2) * 0.76) {
                    kolvoKletok = 15;
                }
                else if (x > (windowSize / 2) * 0.28 && x < (windowSize / 2) * 1.72 && y >(windowSize / 2) * 0.83 && y < (windowSize / 2) * 1.16) {
                    kolvoKletok = 25;
                }
                else if (x > (windowSize / 2) * 0.28 && x < (windowSize / 2) * 1.72 && y >(windowSize / 2) * 1.24 && y < (windowSize / 2) * 1.56) {
                    kolvoKletok = 35;
                }
                else continue;
                step++;
                desk = (square**)malloc(sizeof(square*) * kolvoKletok);
                for (short i = 0; i < kolvoKletok; i++) {
                    desk[i] = (square*)malloc(sizeof(square) * kolvoKletok);
                }
                for (short i = 0; i < kolvoKletok; i++) {
                    for (short j = 0; j < kolvoKletok; j++) {
                        desk[i][j].priority = 0;
                        desk[i][j].line_priority_X = 0;
                        desk[i][j].line_priority_O = 0;
                        desk[i][j].sign = pusto;
                        desk[i][j].block = 0;
                    }
                }
                chooseSymbol(windowSize);
                glfwSwapBuffers(window);
            }
            else if (step == 2) {
                glClear(GL_COLOR_BUFFER_BIT);
                chooseSymbol(windowSize);
                glfwSwapBuffers(window);
                if (x > (windowSize / 2) * 0.3 && x < (windowSize / 2) * 0.86 && y >(windowSize / 2) * 0.86 && y < (windowSize / 2) * 1.42) {
                    option = 1;
                    step++;
                    player_move = krestik;
                    bot_move = nolik;
                }
                else if (x > (windowSize / 2) * 1.14 && x < (windowSize / 2) * 1.7 && y >(windowSize / 2) * 0.86 && y < (windowSize / 2) * 1.42) {
                    option = 2;
                    step++;
                    player_move = nolik;
                    bot_move = krestik;
                }
                glScalef(62.5, -62.5, 1);
                if (option == 1) {
                    renderDesk(kolvoKletok, windowSize);
                    glfwSwapBuffers(window);
                }
                else if (option == 2) {
                    coordx = kolvoKletok / 2 + 1;
                    coordy = kolvoKletok / 2 + 1;
                    if (desk == NULL) return 1;
                    desk[coordx][coordy].sign = krestik;
                    setPriority(desk, kolvoKletok, coordx, coordy);
                    printDesk(desk, kolvoKletok, window, windowSize);
                }
                else {
                    continue;
                }
                step++;
            }
            else if (step == 4) {
                glClear(GL_COLOR_BUFFER_BIT);
                coordy = ((short)x / (windowSize / kolvoKletok));
                coordx = ((short)y / (windowSize / kolvoKletok));
                if (desk[coordx][coordy].sign != pusto) {
                    continue;
                }
                desk[coordx][coordy].sign = player_move;
                setPriority(desk, kolvoKletok, coordx, coordy);
                printDesk(desk, kolvoKletok, window, windowSize);
                step = checkWin(desk, kolvoKletok, windowSize, window, option);
                if (step == 5 || step == 6) continue;

                double time1 = clock();
                putDot(desk, kolvoKletok);
                squarePriority bot_next_move = get_move_minimax(desk, kolvoKletok);
                coordx = bot_next_move.coordx;
                coordy = bot_next_move.coordy;
                eraseDot(desk, kolvoKletok);
                if (coordx == -1 && coordy == -1) {
                    do_first_step(desk, kolvoKletok);
                }
                else {
                    desk[coordx][coordy].sign = bot_move;
                }
                double time2 = clock();
                double operating_time = (time2 - time1) / CLOCKS_PER_SEC;
                printDesk(desk, kolvoKletok, window, windowSize);
                printf("Algorithm time operating: %f\n", operating_time);
                step = checkWin(desk, kolvoKletok, windowSize, window, option);
                if (step == 5 || step == 6) continue;
            }
            else if (step == 5) {
                step += 2;
                victory(windowSize);
                glfwSwapBuffers(window);
            }
            else if (step == 6) {
                step++;
                fail(windowSize);
                glfwSwapBuffers(window);
            }
            else if (step == 7) {
                break;
            }

        }

        glfwPollEvents();
    }

    glfwTerminate();

    for (short i = 0; i < kolvoKletok; i++) {
        free(desk[i]);
    }
    free(desk);

    return 0;
}
