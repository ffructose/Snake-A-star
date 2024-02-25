#include "SnakeMenu.h"
#include <vector>


void SMenu::setInitOptText(sf::Text& text, const sf::String& str, float xpos, float ypos) const
{
	text.setFont(font);
	text.setFillColor(menu_text_color);
	text.setString(str);
	text.setCharacterSize(size_font);
	text.setPosition(xpos, ypos);
	text.setOutlineThickness(3);
	text.setOutlineColor(border_color);
}

void SMenu::AlignMenu(int MenuNum)
{

	if (MenuNum == 1)
	{
		for (int i = 0; i < max_menu; i++) {
			mainMenu[i].setPosition(mainMenu[i].getPosition().x, mainMenu[i].getPosition().y);
		}
	}
	else
	{
		mainMenu[0].setPosition(mainMenu[0].getPosition().x, mainMenu[0].getPosition().y);
		mainMenu[1].setPosition(mainMenu[0].getPosition().x + 350, mainMenu[0].getPosition().y);
	}
}

SMenu::SMenu(sf::RenderWindow& window, float menux, float menuy, int sizeFont, int step, std::vector<sf::String>& name)
	:menu_X(menux), menu_Y(menuy), menu_Step(step), max_menu(static_cast<int>(name.size())), size_font(sizeFont), mainMenu(name.size()), mywindow(window)
{
	font.loadFromFile("C://Users//Legion//Desktop//дурастіка//fonts//cool_font.ttf");

	for (int i = 0, ypos = static_cast<int>(menu_Y); i < max_menu; i++, ypos += menu_Step)
		setInitOptText(mainMenu[i], name[i], menu_X, static_cast<float>(ypos));
	mainMenuSelected = 0;
	mainMenu[mainMenuSelected].setFillColor(chose_text_color);
}

void SMenu::MoveUp_Left()
{
	mainMenuSelected--;

	if (mainMenuSelected >= 0) {
		mainMenu[mainMenuSelected].setFillColor(chose_text_color);
		mainMenu[mainMenuSelected + 1].setFillColor(menu_text_color);
	}
	else
	{
		mainMenu[0].setFillColor(menu_text_color);
		mainMenuSelected = max_menu - 1;
		mainMenu[mainMenuSelected].setFillColor(chose_text_color);
	}
}

void SMenu::MoveDown_Right()
{
	mainMenuSelected++;

	if (mainMenuSelected < max_menu) {
		mainMenu[mainMenuSelected - 1].setFillColor(menu_text_color);
		mainMenu[mainMenuSelected].setFillColor(chose_text_color);
	}
	else
	{
		mainMenu[max_menu - 1].setFillColor(menu_text_color);
		mainMenuSelected = 0;
		mainMenu[mainMenuSelected].setFillColor(chose_text_color);
	}

}

void SMenu::draw()
{
	for (int i = 0; i < max_menu; i++) mywindow.draw(mainMenu[i]);
}

void SMenu::setColorTextMenu(sf::Color menColor, sf::Color ChoColor, sf::Color BordColor)
{
	menu_text_color = menColor;
	chose_text_color = ChoColor;
	border_color = BordColor;

	for (int i = 0; i < max_menu; i++) {
		mainMenu[i].setFillColor(menu_text_color);
		mainMenu[i].setOutlineColor(border_color);
	}

	mainMenu[mainMenuSelected].setFillColor(chose_text_color);
}