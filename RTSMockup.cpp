// RTSMockup.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <string.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Unit.h"
#include "Point2D.h"
#include "Command.h"
#include <iostream>
using namespace std;

enum eGameState {GSM_MENU, GSM_LEVEL, GSM_END};
eGameState gameState;

enum DecisionState {Selecting,Commanding};

sf::RectangleShape selectionShape;
sf::Vector2i originalPoint;//SELECT LEFT CLICK POINT
sf::RenderWindow window(sf::VideoMode(1024, 768), "RTS Mockup");

void drawUnitVector(sf::RenderWindow * window, const std::vector<Unit>& list){
	for(unsigned int i=0;i<list.size();i++){
		(*window).draw(list[i].UnitShape);
	}
	
}

void updateSelection(){//TODO FINISH THIS FUNCTION
	sf::Vector2i LimitedMousePos = sf::Mouse::getPosition(window);
	if(LimitedMousePos.x<0 || LimitedMousePos.x > window.getSize().x){//if 

	}else{

	}
	int xDiff=(sf::Mouse::getPosition(window).x-originalPoint.x);
	int yDiff=(sf::Mouse::getPosition(window).y-originalPoint.y);
	if(xDiff<=0){// +x
		if(yDiff<=0){//+x +y Quad 1
			selectionShape.setPosition(originalPoint.x,sf::Mouse::getPosition(window).y);
			xDiff=sf::Mouse::getPosition(window).x-originalPoint.x;
			yDiff=originalPoint.y-sf::Mouse::getPosition(window).y;
			selectionShape.setSize(sf::Vector2f(xDiff,yDiff));
		}else{//+x -y Quad 4
			selectionShape.setSize(sf::Vector2f(sf::Mouse::getPosition(window).x-originalPoint.x,sf::Mouse::getPosition(window).y-originalPoint.y));
		}
	}else{
		if(yDiff<=0){// -x +y Quad 2
			selectionShape.setPosition(sf::Vector2f(sf::Mouse::getPosition(window)));
			selectionShape.setSize(sf::Vector2f(originalPoint.x-sf::Mouse::getPosition(window).x,originalPoint.y-sf::Mouse::getPosition(window).y));
		}else{// -x -y Quad 3
			selectionShape.setPosition(sf::Vector2f(sf::Mouse::getPosition(window).x,originalPoint.y));
			selectionShape.setSize(sf::Vector2f(originalPoint.x-sf::Mouse::getPosition(window).x,sf::Mouse::getPosition(window).y-originalPoint.y));
		}
	}
	printf("Original Point %d %d",originalPoint.x,originalPoint.y);
	printf("Selection Cords %d %d\n",selectionShape.getPosition().x,selectionShape.getPosition().y);
}

char * commandString(CommandEnum theCommand){
	switch(theCommand){
	case Attack:
		return "Attack";
	case Move:
		return "Move";
	case Halt:
		return "Halt";
	case Stop:
		return "Stop";
	}
}
Unit * unitAtMousePos(){//check for target unit at mouse posisiton
	return NULL;
}
void CommandSelectionUnits(CommandEnum theCommand, std::vector<Unit *> * playerSelection){
	for(unsigned int i=0;i<(*playerSelection).size();i++){
		if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)&&!sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)){
			printf("UnShifted %S Command",commandString(theCommand));
			(*(*playerSelection)[i]).unitCommands.empty();
		}
		printf("Shifted %S Command",commandString(theCommand));
		(*(*playerSelection)[i]).unitCommands.push_back(Command(theCommand,Point2D(sf::Mouse::getPosition(window)),unitAtMousePos()));
	}
}


bool mouseIsOnScreen(){
	if((sf::Mouse::getPosition().x-window.getPosition().x >-500) && (sf::Mouse::getPosition().x-window.getPosition().x <=window.getSize().x)){
			if((sf::Mouse::getPosition().y-window.getPosition().y >-500) && (sf::Mouse::getPosition().y-window.getPosition().y <=window.getSize().y)){
				return true;
			}
	}
	return false;
}

void mainGame()
{
	//Cheat protection:
	//Encrypt packets
	//or use Base64 and obsfuscate
	
	window.setFramerateLimit(61);
	std::vector<Unit> playerUnits;
	std::vector<Unit> enemyUnits;
	std::vector<Unit *> playerSelection;

	for(unsigned int i=0;i<5;i++){
		playerUnits.push_back(Unit(Point2D((i+1)*150+50,668),sf::Color::Green,10));
	}
	playerSelection.push_back(&playerUnits[0]);
	for(unsigned int i=0;i<3;i++){
		enemyUnits.push_back(Unit(Point2D((i+1)*150+180,100),sf::Color::Red,20));
	}
	//Allows the player to queue up commands.
	//Non Shifted Commands empty the list.
	//Same with Unit Selection

	DecisionState mouseCommandState = Selecting;//True if valid command key is pressed.
	bool leftMouseClickedLastCycle = false;//For box selection
	CommandEnum currentCommand;

	

	selectionShape.getPoint(0);


	selectionShape.setFillColor(sf::Color::White);
	selectionShape.setPosition(500.f,600.f);
	selectionShape.setSize(sf::Vector2f(30.f,30.f));

	bool selectionDrawState = false; //user is selecting if true

	//Click -> Size Box -> Unclick -> Collision Detect Box with Player Units
	//if(shift){add pointers to those Units to Selection Vector}
	//else {empty Selection Vector, add those}
	//Select, Command Key, Left Click

	//Valid Commands: Attack, Move, Halt, Stop
	//LEFT CLICK DOES TWO THINGS. SELECTION AND COMMANDING
	//RIGHT CLICK ONLY DOES MOVE COMMAND
	bool inWindow =false;
	//sf::ConvexShape selectRectangle;
	//selectRectangle.setPointCount(4);
	//for(unsigned int i=0;i<4;i++){
	//	selectRectangle.setPoint(i, sf::Vector2f(0,0));
	//}
	//string DisplayTest;
	//unsigned int counter=0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
		if(mouseIsOnScreen()){//IF MOUSE IS ON THE WINDOW
				if(sf::Mouse::isButtonPressed(sf::Mouse::Left)^leftMouseClickedLastCycle){//if Left Mouse Button state has changed since last cycle
					if(leftMouseClickedLastCycle==true){//if user has released left click button
						switch(mouseCommandState){
						case Selecting://Selection Change
							updateSelection();
							break;
						case Commanding:
							CommandSelectionUnits(currentCommand,&playerSelection);
							break;
						}
					}else{//if user has clicked left click button
						switch(mouseCommandState){
						case Selecting:
							selectionShape.setSize(sf::Vector2f(0,0));
							originalPoint=sf::Mouse::getPosition(window);
							updateSelection();
							break;
						case Commanding:
							CommandSelectionUnits(currentCommand,&playerSelection);
							break;
						}
					}
					leftMouseClickedLastCycle^=true;//Swaps leftMouseClickLastCycle state
				}else if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){//If Right mbutton clicked
					CommandSelectionUnits(Move,&playerSelection);
				}
		}
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left)==true &&leftMouseClickedLastCycle==true && mouseCommandState==Selecting){//If the player is still holding down the left click button while selecting
					updateSelection();
		}
        window.clear();
		window.draw(selectionShape);

		drawUnitVector(&window,playerUnits);
		drawUnitVector(&window,enemyUnits);
        window.display();
    }
}

void menu()
{

	sf::RectangleShape rectangle(sf::Vector2f(120, 50));
	rectangle.setSize(sf::Vector2f(400, 100));
	rectangle.setPosition(300,300);
	rectangle.setFillColor(sf::Color::Magenta);

	/*sf::Image image;

	if(!image.loadFromFile("MainMenu.png"))
	{
		cout << "error loading picture\n";
	}

	sf::Texture texture;
	texture.update(image);

	sf::Sprite sprite;
	sprite.setTexture(texture);*/

	while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
			sf::Vector2i point = sf::Mouse::getPosition(window);

			if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
				cout<< "click\n";
				if(point.x>=300&&point.x<=700)
				{
					if(point.y>=300&&point.y<=400)
					{
						gameState=GSM_LEVEL;
						break;
					}
				}
			}

        window.clear(sf::Color::Black);
		window.draw(rectangle);
		//window.draw(sprite);
        window.display();
    }
}


int _tmain(int argc, _TCHAR* argv[])
{
	gameState = GSM_MENU;

	for(;;){
		switch (gameState)
		{
		  case GSM_MENU :
		  {
			// Menu state
			  menu();
			/*if(menu()==1)
			{
				gameState = GSM_LEVEL;
			}*/
		  }
		  break;
		  case GSM_LEVEL :
		  {
			// Level state
			  mainGame();
		  }
		  break;
		  case GSM_END :
		  {
			// End state
		  }
		  break;
		}
	}
    return 0;
}
