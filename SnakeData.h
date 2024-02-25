#pragma once
#include <SFML/Graphics.hpp>
#include <string>

const int width_menu = 1375;   // ������ ��������� ���� + ���� ���
const int height_menu = 775;   // ������ ��������� ���� + ���� ���

const int width_opt = 687;     // ������ ���� �����
const int height_opt = 194;    // ������ ���� �����

const int W = 55;              // ������ � ������� ������ ����
const int H = 31;              // ������ � ������� ������ ����
const int tile_size = 25;      // ����� ����� ������

const int W_pole = 53;         // ������ � ������� ����
const int H_pole = 27;         // ������ � ������� ����

enum MenuOption                // ������ ������ ���� 
{
    PLAY_GAME,
    ASTAR,
    SETTINGS,
    ABOUT_GAME,
    QUIT_GAME
};

enum SnakeDirection            // �������� ����� 
{
    Down,
    Left,
    Right,
    Up
};


struct TextFormat              // ������ ������
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


 ������ _ ������ ���� - 53 , ���������� [1 ; 53] ��� (0 ; 54)
 ������ | ������ ���� - 27 , ���������� [3 ; 29] ��� (2 ; 30)
*/