#pragma once
#include <SFML/Graphics.hpp>


class SMenu
{

	float menu_X, menu_Y;		     // ���������� ���� �� X, Y
	int menu_Step;                   // ³������ �� �������� ����
	int max_menu;                    // ʳ������ ������ ����
	int size_font;                   // ����� ������
	int mainMenuSelected;            // ����� ��������� ������ ���� 
	sf::Font font;                   // ����� ������ ���� 
	std::vector<sf::Text> mainMenu;  // ����� ����, ��������� �����

	sf::Color menu_text_color;       // ���� ������ ������ ����
	sf::Color chose_text_color;      // ���� ��������� ������ ����
	sf::Color border_color;          // ���� ��������� ������ ������ ����


	// ������������ ������ ����
	void setInitOptText(sf::Text& text, const sf::String& str, float xpos, float ypos) const;

	sf::RenderWindow& mywindow;      // ��������� �� ����.����

public:

	SMenu(sf::RenderWindow& window, float menux, float menuy, int sizeFont, int step, std::vector<sf::String>& name);

	void draw();                     // ��������� ����

	void MoveUp_Left();              // ���������� �������� ������ �����-����

	void MoveDown_Right();           // ���������� �������� ������ ����-������

	// ���� ������ ����
	void setColorTextMenu(sf::Color menColor, sf::Color ChoColor, sf::Color BordColor);

	void AlignMenu(int posx);        // ����������� ������

	int getSelectedMenuNumber()      // ������� ����� ��������� ������ 
		const
	{
		return mainMenuSelected;
	}

};
