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

int dir = 2;        // поворот
int num = 3;        // довжина змійки
int score = 0;
float game_delay = 0.1;   // затримка
float astar_delay = 0.1;  // затримка

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

// довжина змійки, структура
struct Snake
{
    int x, y; // координати 
}s[1705];     // загальна макс. кількість 

// планета, структура
struct Food
{
    int x, y; // координати 
} f;

// відповідає за гру
void SnakeLogic()
{

    // малюється змійка
    for (int i = num; i > 0; i--)
    {
        s[i].x = s[i - 1].x;
        s[i].y = s[i - 1].y;
    }

    // рух змійки
    switch (dir)
    {
    case SnakeDirection::Down:
        s[0].y += 1; // вниз
        break;
    case SnakeDirection::Left:
        s[0].x -= 1; // вліво
        break;
    case SnakeDirection::Right:
        s[0].x += 1; // вправо
        break;
    case SnakeDirection::Up:
        s[0].y -= 1; // вгору
        break;
    }

    // врізалась в стінку - програш
    if (s[0].x == W - 1 || s[0].x == 0 || s[0].y == H - 1 || s[0].y == 2) {
        game_state = false;
        border_touch = true;
    }

    // їсть планету
    if ((s[0].x == f.x) && (s[0].y == f.y)) {
        num++;   // виросла
        score++; // збільшився рахунок

    // міняються координати планети

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

    // програш
    for (int i = 1; i < num; i++) // для всіх плиток змійки
    {
        // врізалась в себе
        if ((s[0].x == s[i].x) && (s[0].y == s[i].y))
        {
            game_state = false;
        }
    }
}

// Функція для налаштування тексту
void InitText(Text& mtext, float xpos, float ypos, const String& str, TextFormat Ftext)
{
    mtext.setCharacterSize(Ftext.size_font);
    mtext.setPosition(xpos, ypos);
    mtext.setString(str);
    mtext.setFillColor(Ftext.menu_text_color);
    mtext.setOutlineThickness(Ftext.bord);
    mtext.setOutlineColor(Ftext.border_color);

}

// оголошую функції
void GameStart(int x);
void About_Game();
void Settings_Game();
void Opt_Buttons(const String& windowname, const String& buttonOne, const String& buttonTwo, int num);
void GameProcess(int x);


int main()
{
    // Створюю вікно для основного меню
    RenderWindow window(VideoMode(width_menu, height_menu), L"SuperSnake", Style::Default);

    // Назви пунктів
    std::vector<String> name_menu{ L"Play", L"A-star", L"Options", L"About", L"Exit"};

    // Об'єкт меню 
    SMenu mymenu(window, 100, 180, 80, 120, name_menu);

    // Налаштування кольору тексту і вирівнювання
    mymenu.setColorTextMenu(Color::Black, Color(53, 53, 255), Color::Cyan);
    mymenu.AlignMenu(1);

    // Фон
    RectangleShape background(Vector2f(width_menu, height_menu));

    Texture texture_window;
    texture_window.loadFromFile("C://Users//Legion//Desktop//дурастіка//photos//fonmenu.JPG");
    background.setTexture(&texture_window);

    // Шрифт 
    Font font;
    font.loadFromFile("C://Users//Legion//Desktop//дурастіка//fonts//cool_font.ttf");

    // початок
    Clock clock;

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyPressed)
            {
                // обробка клавіатури
                if (event.key.code == Keyboard::Up) { mymenu.MoveUp_Left(); }       // вгору
                if (event.key.code == Keyboard::Down) { mymenu.MoveDown_Right(); }  // вниз
                if (event.key.code == Keyboard::Enter)                              // ентер
                {
                    MenuOption selectedOption = static_cast<MenuOption>(mymenu.getSelectedMenuNumber());

                    // Перехід на обраний пункт меню
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

        // Відмальовую все     
        window.clear();
        window.draw(background);
        mymenu.draw();
        window.display();
    }
    return 0;
}

// Грати
void GameStart(int x)
{
    srand(time(0));

    RenderWindow game_window(VideoMode(tile_size * W, tile_size * H), "Snake!");

    // Шрифт для рахунку
    Font score_font;
    score_font.loadFromFile("C://Users//Legion//Desktop//дурастіка//fonts//cyber_font.ttf");

    // рахунок (зверху)
    Text Score;
    Score.setFont(score_font);
    TextFormat Stext;
    Stext.size_font = 100;
    Stext.menu_text_color = Color::Cyan;
    Stext.bord = 3;

    // плитка
    Texture t;
    t.loadFromFile("C://Users//Legion//Desktop//дурастіка//photos//blank.png");
    Sprite tiles(t);

    // змійка
    Texture sn;
    sn.loadFromFile("C://Users//Legion//Desktop//дурастіка//photos//snake_cool.png");
    Sprite snake(sn);

    // планетка
    Texture pl;
    pl.loadFromFile("C://Users//Legion//Desktop//дурастіка//photos//apple.png");
    Sprite planet(pl);

    // стіна
    Texture br;
    br.loadFromFile("C://Users//Legion//Desktop//дурастіка//photos//brick_galaxy.png");
    Sprite brick(br);


    f.x = 10; // координата х планети
    f.y = 10; // координата у планети
    
    // Початкові координати для змійки
    s[0].x = W / 2;
    s[0].y = H / 2;
    
    s[1].x = W / 2 - 1;
    s[1].y = H / 2;

    s[2].x = W / 2 - 2;
    s[2].y = H / 2;

    Clock clock;       // час
    float timer = 0;   // через який час змійка рухається

    Astar_class snake_astar;
    snake_astar.Astar_Create();
    
    while (game_window.isOpen())
    {
        // ігровий час
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event event;
        while (game_window.pollEvent(event))
        {
            if (event.type == Event::Closed) game_window.close();
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Escape) // пауза
                {
                    Opt_Buttons(L"Pause", L"Resume", L"Exit", 1);
                    if (exit_snake)
                    {
                        pause_exit = true;
                        game_window.close();

                    }
                }
                if (x == 1)                                          // якщо х = 1 граю сама
                {
                    if (event.key.code == Keyboard::Left) dir = 1;
                    if (event.key.code == Keyboard::Right) dir = 2;
                    if (event.key.code == Keyboard::Up) dir = 3;
                    if (event.key.code == Keyboard::Down) dir = 0;
                }
            }
        }
        if (x == 2)                                                  // якщо х = 2 викликаю алгоритм
            dir = snake_astar.Astar_Update(s[1].x, s[1].y, s[num - 1].x, s[num - 1].y, s[0].x, s[0].y, f.x, f.y);

        if (!pause_exit) {
            // робота гри
            if(x == 1)
                if (timer > game_delay && game_state) {
                    timer = 0;       // час
                    SnakeLogic();    // логіка гри (генерація планети, змійки і т д)
                }
            if(x == 2)
                if (timer > astar_delay && game_state) {
                    timer = 0;       // час
                    SnakeLogic();    // логіка гри (генерація планети, змійки і т д)
                }

            game_window.clear();

            // відмальовка поля
            for (int i = 0; i < W; i++)
            {
                for (int j = 2; j < H; j++)
                {
                    if (i == 0 || i == W - 1 || j == 2 || j == H - 1)
                    {
                        brick.setPosition(i * tile_size, j * tile_size); // розташування стінки
                        game_window.draw(brick);                         // малюю стінку
                    }
                    else
                    {
                        tiles.setPosition(i * tile_size, j * tile_size); // розташування поля
                        game_window.draw(tiles);                         // малюю поле
                    }
                }
            }

            // малюю змійку
            for (int i = 0; i < num; i++) {
                if (border_touch && i == 0)
                {
                    s[0].x -= tile_size;
                    s[0].y -= tile_size;
                    num++;
                }
                if (i != 0) // тіло 
                    snake.setTextureRect(IntRect(0, 0, tile_size, tile_size));
                if (!border_touch && i == 0)        // голова
                    snake.setTextureRect(IntRect(dir * tile_size, tile_size, tile_size, tile_size));

                // зміна голови якщо програш
                if (!game_state && i == 1)
                    snake.setTextureRect(IntRect(dir * tile_size, tile_size * 2, tile_size, tile_size));

                // розташування (х, у) кожної плитки змійки
                snake.setPosition(s[i].x * tile_size, s[i].y * tile_size); 

                // для плиток 
                game_window.draw(snake);

            }


            planet.setPosition(f.x * tile_size, f.y * tile_size); // розташування яблука
            game_window.draw(planet); //малюю яблуко

            InitText(Score, 600, -60, std::to_string(score), Stext);
            game_window.draw(Score);

            game_window.display();
            // програш - вивід Game Over
            if (!game_state)
            {
                Opt_Buttons(L"Game Over", L"Again", L"Exit", 2);

                game_window.close();

            }
        }
    }
}

// Налаштування
void Settings_Game()
{
    RenderWindow Settings_Window(VideoMode(width_menu, height_menu), L"Settings", Style::Default);

    // Для гри і для астар
    std::vector<String> snake_sett{ L"  Game mode", L"   A* mode"};

    // Об'єкт меню
    SMenu mySettMenu(Settings_Window, 125, 260, 80, 225, snake_sett);

    // Колір і вирівнювання
    mySettMenu.setColorTextMenu(Color::Black, Color(53, 53, 255), Color::Cyan);
    mySettMenu.AlignMenu(1);

    Font font;
    font.loadFromFile("C://Users//Legion//Desktop//дурастіка//fonts//cool_font.ttf");

    // поточна швидкість змійки для гри
    Text Speed_game;
    Speed_game.setFont(font);

    // поточна швидкість змійки для алгоритма
    Text Speed_astar;
    Speed_astar.setFont(font);

    TextFormat Stext;
    Stext.size_font = 30;
    Stext.menu_text_color = Color(23, 23, 147);
    Stext.bord = 3;

    // Фон 
    RectangleShape background_sett(Vector2f(width_menu, height_menu));

    Texture texture_sett;
    texture_sett.loadFromFile("C://Users//Legion//Desktop//дурастіка//photos//settings.JPG");
    background_sett.setTexture(&texture_sett);

    Clock clock;

    while (Settings_Window.isOpen())
    {
        Event event;
        while (Settings_Window.pollEvent(event))
        {

            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Up) { mySettMenu.MoveUp_Left(); }      // вгору
                if (event.key.code == Keyboard::Down) { mySettMenu.MoveDown_Right(); } // вниз
                if (event.key.code == Keyboard::Escape) Settings_Window.close();
                if (event.key.code == Keyboard::Left)                                  // вліво - зменшити
                {
                    if (mySettMenu.getSelectedMenuNumber() == 0) game_delay += 0.01;

                    if (mySettMenu.getSelectedMenuNumber() == 1) astar_delay += 0.01;
                }
                if (event.key.code == Keyboard::Right)                                 // вправо - збільшити
                {
                    if (mySettMenu.getSelectedMenuNumber() == 0) game_delay -= 0.01;

                    if (mySettMenu.getSelectedMenuNumber() == 1) astar_delay -= 0.01;

                }
            }
        }

        InitText(Speed_game, 350, 363, std::to_string(speed_converter(game_delay)), Stext);
        InitText(Speed_astar, 350, 587, std::to_string(speed_converter(astar_delay)), Stext);

        // Малюю все     
        Settings_Window.clear();
        Settings_Window.draw(background_sett);
        mySettMenu.draw();
        Settings_Window.draw(Speed_game);
        Settings_Window.draw(Speed_astar);
        Settings_Window.display();
    }
}

// Про гру
void About_Game()
{
    RenderWindow About_Window(VideoMode(width_menu, height_menu), L"Про гру", Style::Default);

    RectangleShape background_ab(Vector2f(width_menu, height_menu));
    Texture texture_ab;
    texture_ab.loadFromFile("C://Users//Legion//Desktop//дурастіка//photos//about.JPG");
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

// Пауза або вікно програшу
void Opt_Buttons(const String& windowname, const String& buttonOne, const String& buttonTwo, int num)
{
    RenderWindow OptButton(VideoMode(width_opt, height_opt), windowname, Style::Default);

    // Далі-Заново + Вихід
    std::vector<String> snake_options{ buttonOne, buttonTwo };

    // Об'єкт меню
    SMenu myOptmenu(OptButton, 75, 120, 50, 120, snake_options);

    // Колір і вирівнювання
    myOptmenu.setColorTextMenu(Color::Black, Color(53, 53, 255), Color::Cyan);
    myOptmenu.AlignMenu(2);

    Font score_font;
    score_font.loadFromFile("C://Users//Legion//Desktop//дурастіка//fonts//cool_font.ttf");

    // рахунок
    Text Score;
    Score.setFont(score_font);

    TextFormat Stext;
    Stext.size_font = 40;
    Stext.menu_text_color = Color(255, 0, 130);
    Stext.bord = 0;

    // Фон паузи(1) або фон GAMEOVER(2) 
    RectangleShape background_options(Vector2f(width_opt, height_opt));

    Texture texture_options;
    if (num == 1)
        texture_options.loadFromFile("C://Users//Legion//Desktop//дурастіка//photos//options_P.PNG");
    if (num == 2)
        texture_options.loadFromFile("C://Users//Legion//Desktop//дурастіка//photos//options_GO.PNG");
    
    background_options.setTexture(&texture_options);

    Clock clock;

    while (OptButton.isOpen())
    {
        Event event;
        while (OptButton.pollEvent(event))
        {

            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Left) { myOptmenu.MoveUp_Left(); }     // вліво
                if (event.key.code == Keyboard::Right) { myOptmenu.MoveDown_Right(); } // вправо
                if (event.key.code == Keyboard::Enter)                                 // ентер
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

        // Малюю все     
        OptButton.clear();
        OptButton.draw(background_options);
        myOptmenu.draw();
        OptButton.draw(Score);
        OptButton.display();
    }
}

// Відповідає за поновлення значень 
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