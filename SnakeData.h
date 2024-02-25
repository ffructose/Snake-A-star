#pragma once
#include <SFML/Graphics.hpp>
#include <string>

const int width_menu = 1375;   // Ширина основного меню + поля гри
const int height_menu = 775;   // Висота основного меню + поля гри

const int width_opt = 687;     // Ширина вікна паузи
const int height_opt = 194;    // Висота вікна паузи

const int W = 55;              // Ширина в плитках всього вікна
const int H = 31;              // Висота в плитках всього вікна
const int tile_size = 25;      // Розмір кожної плитки

const int W_pole = 53;         // Ширина в плитках ПОЛЯ
const int H_pole = 27;         // Висота в плитках ПОЛЯ

enum MenuOption                // Перелік пунктів меню 
{
    PLAY_GAME,
    ASTAR,
    SETTINGS,
    ABOUT_GAME,
    QUIT_GAME
};

enum SnakeDirection            // Навігація змійки 
{
    Down,
    Left,
    Right,
    Up
};


struct TextFormat              // Формат тексту
{
    int size_font;
    sf::Color menu_text_color;
    float bord;
    sf::Color border_color;
};




/*
* 
  0  1  2  3  4  5  .  .  .  52  53  54
  1                                   |
  2  -  -  -  -  -  -  -  -  -  -  -  -  
  3                                   |
  |                                   |
  |                                   |
  |                                   |
 28                                   |
 29                                   |
 30  -  -  -  -  -  -  -  -  -  -  -  -


 Ширина _ самого поля - 53 , починається [1 ; 53] або (0 ; 54)
 Висота | самого поля - 27 , починається [3 ; 29] або (2 ; 30)
*/