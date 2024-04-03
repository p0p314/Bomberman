#include "Gui.hpp"

Gui::Gui()
{

}

Gui::~Gui(){}

void Gui::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    if(_isStartingGame)
    {
        target.draw(*_background);
        target.draw(*_startCounter);
    }
    
    if(!_isEndGame)
    {
        target.draw(*_characterOneLives);
        target.draw(*_characterTwoLives);

    }
    else 
    {
        target.draw(*_background);
        target.draw(*_textOfEndGame);
    }

}

void Gui::init(sf::Font *font, int  windowSizeWidth, int windowSizeHeight)
{
    _windowHeight = windowSizeHeight;
    _windowWidth = windowSizeWidth;
    _background = new sf::RectangleShape(sf::Vector2f(windowSizeWidth, windowSizeHeight));
    _background->setFillColor(sf::Color(0,0,0,40));

    _characterOneLives = new sf::Text();
    _characterOneLives->setFont(*font);
    _characterOneLives->setString("Player 1 Lives: 3");
    _characterOneLives->setPosition(sf::Vector2f(50,windowSizeHeight - 50));
    _characterOneLives->setFillColor(sf::Color::Black);
    _characterOneLives->setCharacterSize(20);
   
    _characterTwoLives = new sf::Text();
    _characterTwoLives->setFont(*font);
    _characterTwoLives->setString("Player 2 Lives: 3");
    _characterTwoLives->setPosition(sf::Vector2f(windowSizeWidth - _characterTwoLives->getLocalBounds().width +97,
                                    windowSizeHeight - 50));
    _characterTwoLives->setFillColor(sf::Color::Black);
    _characterTwoLives->setCharacterSize(20);

    _startCounter = new sf::Text();
    _startCounter->setFont(*font);
    _startCounter->setString("");
    _startCounter->setCharacterSize(100);
    _startCounter->setFillColor(sf::Color::White);

    _textOfEndGame = new sf::Text();
    _textOfEndGame->setFont(*font);
    _textOfEndGame->setCharacterSize(40);
    _textOfEndGame->setFillColor(sf::Color::White);
}

void Gui::updateStats(std::vector<Personnage*> * characters)
{
    for(short i = 0; i < characters->size(); i++)
    {
        switch(i)
        {
            case 0 :
                _characterOneLives->setString((*characters)[i]->getName() +" : " + std::to_string((*characters)[i]->getLives()) + " vies");
                break;
            case 1 :
                _characterTwoLives->setString((*characters)[i]->getName() +" : " + std::to_string((*characters)[i]->getLives()) + " vies");
                break;
        }

        if(((*characters)[i]->getLives() == 0))
        {
            _isEndGame = true;
            _victoryCounter++;
            if(_victoryCounter == characters->size()) _textOfEndGame->setString("Egalite");
            else _textOfEndGame->setString((*characters)[i]->getName() + " gagne la partie");
            
            _textOfEndGame->setPosition(sf::Vector2f(_windowWidth/2 - _textOfEndGame->getLocalBounds().width/2,
                                             _windowHeight/2 -_textOfEndGame->getLocalBounds().height/2));
    
        }
    }

    
}

void Gui::updateStats(int startCounter)
{
    if(startCounter <0) _isStartingGame = false;
    if(startCounter == 0) _startCounter->setString("jouez");
    else
    {
        _startCounter->setString(std::to_string(startCounter));
    } 

     _startCounter->setPosition(sf::Vector2f(_windowWidth/2 - _startCounter->getLocalBounds().width/2,
                                               _windowHeight/2 - _startCounter->getLocalBounds().height/2));
       
}