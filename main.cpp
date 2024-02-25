#include <SFML/Graphics.hpp>

#include <string>
#include <vector>
#include <list>
#include <time.h>
#include <cmath>

#include "SnakeMenu.h"
#include "Astar_nodes.h"
#include "SnakeData.h"

using namespace sf;

int dir = 2;        // �������
int num = 3;        // ������� �����
int score = 0;
float game_delay = 0.1;   // ��������
float astar_delay = 0.1;  // ��������

int speed_converter(float delay) 
{
    return std::round((delay - 0.1) / (- 0.01));
}

bool game_state = true;
bool border_touch = false;
bool exit_snake = false;
bool play_again = false;
bool playing = false;
bool pause_exit = false;

// ������� �����, ���������
struct Snake
{
    int x, y; // ���������� 
}s[1705];     // �������� ����. ������� 

// �������, ���������
struct Food
{
    int x, y; // ���������� 
} f;

// ������� �� ���
void SnakeLogic()
{

    // ��������� �����
    for (int i = num; i > 0; i--)
    {
        s[i].x = s[i - 1].x;
        s[i].y = s[i - 1].y;
    }

    // ��� �����
    switch (dir)
    {
    case SnakeDirection::Down:
        s[0].y += 1; // ����
        break;
    case SnakeDirection::Left:
        s[0].x -= 1; // ����
        break;
    case SnakeDirection::Right:
        s[0].x += 1; // ������
        break;
    case SnakeDirection::Up:
        s[0].y -= 1; // �����
        break;
    }

    // �������� � ����� - �������
    if (s[0].x == W - 1 || s[0].x == 0 || s[0].y == H - 1 || s[0].y == 2) {
        game_state = false;
        border_touch = true;
    }

    // ���� �������
    if ((s[0].x == f.x) && (s[0].y == f.y)) {
        num++;   // �������
        score++; // ��������� �������

    // �������� ���������� �������

        bool goodapple = false;

        while (goodapple!= true)
        {
            bool flag = false;

            f.x = rand() % (W - 4) + 3;
            f.y = rand() % (H - 4) + 3;
            for (int i = 0; i < num; i++)
            {
                if (f.x == s[i].x && f.y == s[i].y)
                {
                    goodapple = false;
                    flag = true;
                    break;
                }
            }
            if (!flag)
                goodapple = true;

        }
    }

    // �������
    for (int i = 1; i < num; i++) // ��� ��� ������ �����
    {
        // �������� � ����
        if ((s[0].x == s[i].x) && (s[0].y == s[i].y))
        {
            game_state = false;
        }
    }
}

// ������� ��� ������������ ������
void InitText(Text& mtext, float xpos, float ypos, const String& str, TextFormat Ftext)
{
    mtext.setCharacterSize(Ftext.size_font);
    mtext.setPosition(xpos, ypos);
    mtext.setString(str);
    mtext.setFillColor(Ftext.menu_text_color);
    mtext.setOutlineThickness(Ftext.bord);
    mtext.setOutlineColor(Ftext.border_color);

}

// �������� �������
void GameStart(int x);
void About_Game();
void Settings_Game();
void Opt_Buttons(const String& windowname, const String& buttonOne, const String& buttonTwo, int num);
void GameProcess(int x);


int main()
{
    // ������� ���� ��� ��������� ����
    RenderWindow window(VideoMode(width_menu, height_menu), L"SuperSnake", Style::Default);

    // ����� ������
    std::vector<String> name_menu{ L"Play", L"A-star", L"Options", L"About", L"Exit"};

    // ��'��� ���� 
    SMenu mymenu(window, 100, 180, 80, 120, name_menu);

    // ������������ ������� ������ � �����������
    mymenu.setColorTextMenu(Color::Black, Color(53, 53, 255), Color::Cyan);
    mymenu.AlignMenu(1);

    // ���
    RectangleShape background(Vector2f(width_menu, height_menu));

    Texture texture_window;
    texture_window.loadFromFile("C://Users//Legion//Desktop//��������//photos//fonmenu.JPG");
    background.setTexture(&texture_window);

    // ����� 
    Font font;
    font.loadFromFile("C://Users//Legion//Desktop//��������//fonts//cool_font.ttf");

    // �������
    Clock clock;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyPressed)
            {
                // ������� ���������
                if (event.key.code == Keyboard::Up) { mymenu.MoveUp_Left(); }       // �����
                if (event.key.code == Keyboard::Down) { mymenu.MoveDown_Right(); }  // ����
                if (event.key.code == Keyboard::Enter)                              // �����
                {
                    MenuOption selectedOption = static_cast<MenuOption>(mymenu.getSelectedMenuNumber());

                    // ������� �� ������� ����� ����
                    switch (selectedOption)
                    {
                    case PLAY_GAME:
                        playing = true;
                        GameProcess(1);
                        break;
                    case ASTAR:
                        playing = true;
                        GameProcess(2);
                        break;
                    case SETTINGS:
                        Settings_Game();
                        break;
                    case ABOUT_GAME:
                        About_Game();
                        break;
                    case QUIT_GAME:
                        window.close();
                        break;
                    default:
                        break;
                    }
                }
            }
        }

        // ³��������� ���     
        window.clear();
        window.draw(background);
        mymenu.draw();
        window.display();
    }
    return 0;
}

// �����
void GameStart(int x)
{
    srand(time(0));

    RenderWindow game_window(VideoMode(tile_size * W, tile_size * H), "Snake!");

    // ����� ��� �������
    Font score_font;
    score_font.loadFromFile("C://Users//Legion//Desktop//��������//fonts//cyber_font.ttf");

    // ������� (������)
    Text Score;
    Score.setFont(score_font);
    TextFormat Stext;
    Stext.size_font = 100;
    Stext.menu_text_color = Color::Cyan;
    Stext.bord = 3;

    // ������
    Texture t;
    t.loadFromFile("C://Users//Legion//Desktop//��������//photos//blank.png");
    Sprite tiles(t);

    // �����
    Texture sn;
    sn.loadFromFile("C://Users//Legion//Desktop//��������//photos//snake_cool.png");
    Sprite snake(sn);

    // ��������
    Texture pl;
    pl.loadFromFile("C://Users//Legion//Desktop//��������//photos//apple.png");
    Sprite planet(pl);

    // ����
    Texture br;
    br.loadFromFile("C://Users//Legion//Desktop//��������//photos//brick_galaxy.png");
    Sprite brick(br);


    f.x = 10; // ���������� � �������
    f.y = 10; // ���������� � �������
    
    // �������� ���������� ��� �����
    s[0].x = W / 2;
    s[0].y = H / 2;
    
    s[1].x = W / 2 - 1;
    s[1].y = H / 2;

    s[2].x = W / 2 - 2;
    s[2].y = H / 2;

    Clock clock;       // ���
    float timer = 0;   // ����� ���� ��� ����� ��������

    Astar_class snake_astar;
    snake_astar.Astar_Create();
    
    while (game_window.isOpen())
    {
        // ������� ���
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event event;
        while (game_window.pollEvent(event))
        {
            if (event.type == Event::Closed) game_window.close();
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Escape) // �����
                {
                    Opt_Buttons(L"Pause", L"Resume", L"Exit", 1);
                    if (exit_snake)
                    {
                        pause_exit = true;
                        game_window.close();

                    }
                }
                if (x == 1)                                          // ���� � = 1 ���� ����
                {
                    if (event.key.code == Keyboard::Left) dir = 1;
                    if (event.key.code == Keyboard::Right) dir = 2;
                    if (event.key.code == Keyboard::Up) dir = 3;
                    if (event.key.code == Keyboard::Down) dir = 0;
                }
            }
        }
        if (x == 2)                                                  // ���� � = 2 �������� ��������
            dir = snake_astar.Astar_Update(s[1].x, s[1].y, s[num - 1].x, s[num - 1].y, s[0].x, s[0].y, f.x, f.y);

        if (!pause_exit) {
            // ������ ���
            if(x == 1)
                if (timer > game_delay && game_state) {
                    timer = 0;       // ���
                    SnakeLogic();    // ����� ��� (��������� �������, ����� � � �)
                }
            if(x == 2)
                if (timer > astar_delay && game_state) {
                    timer = 0;       // ���
                    SnakeLogic();    // ����� ��� (��������� �������, ����� � � �)
                }

            game_window.clear();

            // ���������� ����
            for (int i = 0; i < W; i++)
            {
                for (int j = 2; j < H; j++)
                {
                    if (i == 0 || i == W - 1 || j == 2 || j == H - 1)
                    {
                        brick.setPosition(i * tile_size, j * tile_size); // ������������ �����
                        game_window.draw(brick);                         // ����� �����
                    }
                    else
                    {
                        tiles.setPosition(i * tile_size, j * tile_size); // ������������ ����
                        game_window.draw(tiles);                         // ����� ����
                    }
                }
            }

            // ����� �����
            for (int i = 0; i < num; i++) {
                if (border_touch && i == 0)
                {
                    s[0].x -= tile_size;
                    s[0].y -= tile_size;
                    num++;
                }
                if (i != 0) // ��� 
                    snake.setTextureRect(IntRect(0, 0, tile_size, tile_size));
                if (!border_touch && i == 0)        // ������
                    snake.setTextureRect(IntRect(dir * tile_size, tile_size, tile_size, tile_size));

                // ���� ������ ���� �������
                if (!game_state && i == 1)
                    snake.setTextureRect(IntRect(dir * tile_size, tile_size * 2, tile_size, tile_size));

                // ������������ (�, �) ����� ������ �����
                snake.setPosition(s[i].x * tile_size, s[i].y * tile_size); 

                // ��� ������ 
                game_window.draw(snake);

            }


            planet.setPosition(f.x * tile_size, f.y * tile_size); // ������������ ������
            game_window.draw(planet); //����� ������

            InitText(Score, 600, -60, std::to_string(score), Stext);
            game_window.draw(Score);

            game_window.display();
            // ������� - ���� Game Over
            if (!game_state)
            {
                Opt_Buttons(L"Game Over", L"Again", L"Exit", 2);

                game_window.close();

            }
        }
    }
}

// ������������
void Settings_Game()
{
    RenderWindow Settings_Window(VideoMode(width_menu, height_menu), L"Settings", Style::Default);

    // ��� ��� � ��� �����
    std::vector<String> snake_sett{ L"  Game mode", L"   A* mode"};

    // ��'��� ����
    SMenu mySettMenu(Settings_Window, 125, 260, 80, 225, snake_sett);

    // ���� � �����������
    mySettMenu.setColorTextMenu(Color::Black, Color(53, 53, 255), Color::Cyan);
    mySettMenu.AlignMenu(1);

    Font font;
    font.loadFromFile("C://Users//Legion//Desktop//��������//fonts//cool_font.ttf");

    // ������� �������� ����� ��� ���
    Text Speed_game;
    Speed_game.setFont(font);

    // ������� �������� ����� ��� ���������
    Text Speed_astar;
    Speed_astar.setFont(font);

    TextFormat Stext;
    Stext.size_font = 30;
    Stext.menu_text_color = Color(23, 23, 147);
    Stext.bord = 3;

    // ��� 
    RectangleShape background_sett(Vector2f(width_menu, height_menu));

    Texture texture_sett;
    texture_sett.loadFromFile("C://Users//Legion//Desktop//��������//photos//settings.JPG");
    background_sett.setTexture(&texture_sett);

    Clock clock;

    while (Settings_Window.isOpen())
    {
        Event event;
        while (Settings_Window.pollEvent(event))
        {

            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Up) { mySettMenu.MoveUp_Left(); }      // �����
                if (event.key.code == Keyboard::Down) { mySettMenu.MoveDown_Right(); } // ����
                if (event.key.code == Keyboard::Escape) Settings_Window.close();
                if (event.key.code == Keyboard::Left)                                  // ���� - ��������
                {
                    if (mySettMenu.getSelectedMenuNumber() == 0) game_delay += 0.01;

                    if (mySettMenu.getSelectedMenuNumber() == 1) astar_delay += 0.01;
                }
                if (event.key.code == Keyboard::Right)                                 // ������ - ��������
                {
                    if (mySettMenu.getSelectedMenuNumber() == 0) game_delay -= 0.01;

                    if (mySettMenu.getSelectedMenuNumber() == 1) astar_delay -= 0.01;

                }
            }
        }

        InitText(Speed_game, 350, 363, std::to_string(speed_converter(game_delay)), Stext);
        InitText(Speed_astar, 350, 587, std::to_string(speed_converter(astar_delay)), Stext);

        // ����� ���     
        Settings_Window.clear();
        Settings_Window.draw(background_sett);
        mySettMenu.draw();
        Settings_Window.draw(Speed_game);
        Settings_Window.draw(Speed_astar);
        Settings_Window.display();
    }
}

// ��� ���
void About_Game()
{
    RenderWindow About_Window(VideoMode(width_menu, height_menu), L"��� ���", Style::Default);

    RectangleShape background_ab(Vector2f(width_menu, height_menu));
    Texture texture_ab;
    texture_ab.loadFromFile("C://Users//Legion//Desktop//��������//photos//about.JPG");
    background_ab.setTexture(&texture_ab);

    while (About_Window.isOpen())
    {
        Event event_play;
        while (About_Window.pollEvent(event_play))
        {
            if (event_play.type == Event::Closed) About_Window.close();
            if (event_play.type == Event::KeyPressed)
            {
                if (event_play.key.code == Keyboard::Escape) About_Window.close();
            }
        }
        About_Window.clear();
        About_Window.draw(background_ab);
        About_Window.display();
    }
}

// ����� ��� ���� ��������
void Opt_Buttons(const String& windowname, const String& buttonOne, const String& buttonTwo, int num)
{
    RenderWindow OptButton(VideoMode(width_opt, height_opt), windowname, Style::Default);

    // ���-������ + �����
    std::vector<String> snake_options{ buttonOne, buttonTwo };

    // ��'��� ����
    SMenu myOptmenu(OptButton, 75, 120, 50, 120, snake_options);

    // ���� � �����������
    myOptmenu.setColorTextMenu(Color::Black, Color(53, 53, 255), Color::Cyan);
    myOptmenu.AlignMenu(2);

    Font score_font;
    score_font.loadFromFile("C://Users//Legion//Desktop//��������//fonts//cool_font.ttf");

    // �������
    Text Score;
    Score.setFont(score_font);

    TextFormat Stext;
    Stext.size_font = 40;
    Stext.menu_text_color = Color(255, 0, 130);
    Stext.bord = 0;

    // ��� �����(1) ��� ��� GAMEOVER(2) 
    RectangleShape background_options(Vector2f(width_opt, height_opt));

    Texture texture_options;
    if (num == 1)
        texture_options.loadFromFile("C://Users//Legion//Desktop//��������//photos//options_P.PNG");
    if (num == 2)
        texture_options.loadFromFile("C://Users//Legion//Desktop//��������//photos//options_GO.PNG");
    
    background_options.setTexture(&texture_options);

    Clock clock;

    while (OptButton.isOpen())
    {
        Event event;
        while (OptButton.pollEvent(event))
        {

            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Left) { myOptmenu.MoveUp_Left(); }     // ����
                if (event.key.code == Keyboard::Right) { myOptmenu.MoveDown_Right(); } // ������
                if (event.key.code == Keyboard::Enter)                                 // �����
                {
                    switch (myOptmenu.getSelectedMenuNumber())
                    {
                    case 0:play_again = true; exit_snake = false; OptButton.close(); break;
                    case 1:exit_snake = true; play_again = false;  OptButton.close(); break;
                    default: OptButton.close(); break;
                    }
                }
            }
        }

        InitText(Score, 220, 67, std::to_string(score), Stext);

        // ����� ���     
        OptButton.clear();
        OptButton.draw(background_options);
        myOptmenu.draw();
        OptButton.draw(Score);
        OptButton.display();
    }
}

// ³������ �� ���������� ������� 
void GameProcess(int x)
{
    while (playing)
    {
        GameStart(x);

        if (play_again)
        {
            game_state = true;
            border_touch = false;
            exit_snake = false;
            play_again = false;
            pause_exit = false;
            dir = 2;
            num = 3;
            score = 0;
            playing = true;
        }

        if (exit_snake)
        {
            game_state = true;
            border_touch = false;
            exit_snake = false;
            play_again = false;
            pause_exit = false;
            dir = 2;
            num = 3;
            score = 0;
            playing = false;
            break;

        }
    }
}