#ifndef funcs_hpp
#define funcs_hpp
#include <stdio.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <ctime>
#include <unistd.h>
#include <random>
using namespace std;

struct tile
{
    sf::RectangleShape shape;
    int x, y;
};

enum direction {d_up, d_right, d_down, d_left, d_stop};
struct snake
{
    int x, y, change_x, change_y;
    direction dir = d_stop;
};

struct apple
{
    int x, y;
};

struct button //структура кнопки
{
    sf::RectangleShape body;
    sf::Text text;
};

struct textbox //структура текстбокса
{
    sf::RectangleShape body;
    sf::Text text;
    sf::Text label;
    string text_s;
    string label_s;
    bool clicked = false;
    sf::View v;
};

void draw_snake (tile tiles[15][16], snake player[240], int length); //отрисовка
void move_snake (snake player[240], int length, direction dir); //сдвиг частей змеи
void generate_apple (tile tiles[15][16], snake player[240], int length, apple &a); //генерация яблока
bool collision_self (snake player[240], int length); //проверка на столкновение с самим собой
bool mouse_on_button (sf::Mouse m, sf::RenderWindow &w, button b); //расположена ли мышь на кнопке
void init_button (button &b, float pos_x, float pos_y, sf::Color b_color, string text, sf::Font &font, sf::Color t_color, float t_size); //инициализация кнопки
void color_button (button &b, sf::Color b_color, sf::Color t_color); //изменение стиля цвета
void init_textbox (textbox &tb, float pos_x, float pos_y, float size_x, float size_y, string text, sf::Font &font, string lab, sf::Font &font_lab, sf::RenderWindow &window); //инициализация текстбокса
bool mouse_on_textbox (sf::Mouse m, sf::RenderWindow &w, textbox tx); //расположена ли мышь на текстбоксе
void name_update (string &name, textbox &tb, sf::Event event); //обновление переменной (для текстбокса)
void restart (tile tiles[15][16], snake player[240], FILE *f, sf::RenderWindow &window, sf::RectangleShape &reset_screen, int &apples_collected, apple appl, sf::Text &broke_record,  bool &restarted, button &btn, sf::RectangleShape &black_screen); //рестарт
bool mouse_on_triangle (sf::RenderWindow &w, sf::Mouse m, sf::CircleShape tri); //мышь на треугольнике
void generate_snake (tile tiles[15][16], snake player[240]); //создание змеи

#endif /* funcs_hpp */
