#pragma once
#include <SFML/Graphics.hpp>


class SMenu
{

	float menu_X, menu_Y;		     // Координати меню по X, Y
	int menu_Step;                   // Відстань між пунктами меню
	int max_menu;                    // Кількість пунктів меню
	int size_font;                   // Розмір шрифта
	int mainMenuSelected;            // Номер вибраного пункту меню 
	sf::Font font;                   // Шрифт тексту меню 
	std::vector<sf::Text> mainMenu;  // Назви меню, динамічний масив

	sf::Color menu_text_color;       // Колір тексту пунктів меню
	sf::Color chose_text_color;      // Колір вибраного пункту меню
	sf::Color border_color;          // Колір обведення тексту пунктів меню


	// Налаштування тексту меню
	void setInitOptText(sf::Text& text, const sf::String& str, float xpos, float ypos) const;

	sf::RenderWindow& mywindow;      // Посилання на граф.вікно

public:

	SMenu(sf::RenderWindow& window, float menux, float menuy, int sizeFont, int step, std::vector<sf::String>& name);

	void draw();                     // Малювання меню

	void MoveUp_Left();              // Переміщення обраного пункту вгору-вліво

	void MoveDown_Right();           // Переміщення обраного пункту вниз-вправо

	// Колір пунктів меню
	void setColorTextMenu(sf::Color menColor, sf::Color ChoColor, sf::Color BordColor);

	void AlignMenu(int posx);        // Вирівнювання пунктів

	int getSelectedMenuNumber()      // Повертає номер вибраного пункту 
		const
	{
		return mainMenuSelected;
	}

};
