#include "GameState.h"

void GameState::resizeWindow()
{
	this->data->Resize(sf::VideoMode(
		static_cast<unsigned int>((this->fieldSize.x * this->grid) + this->padding *2),
		static_cast<unsigned int>((this->fieldSize.y * this->grid) + (this->padding *2) + this->topBarHeight)
	));

	//remember to fix data->percentSize cuz it doesnt change value properly after resizing
}

//Inits
void GameState::init()
{
	this->gameStarted = false;

	//width and height of single tile
	this->grid = 25;

	//margin
	this->padding = 5.f;


	this->topBarHeight = 50.f;

	//position of mine field
	this->fieldPosition = sf::Vector2f(this->padding, this->topBarHeight + this->padding);
}

void GameState::initGui()
{
	//Background stuff
	{
		this->BackGround.setPosition(sf::Vector2f(0.f, 0.f));
		this->BackGround.setSize(sf::Vector2f(
			static_cast<float>(this->data->resolution.width), 
			static_cast<float>(this->data->resolution.height)));
		this->BackGround.setFillColor(sf::Color(80, 80, 80, 255));
	}

	//Top Bar stuff
	{
		this->topBar.setPosition(sf::Vector2f(this->padding, this->padding));
		this->topBar.setSize(sf::Vector2f(static_cast<float>(this->data->resolution.width) - this->padding*2, this->topBarHeight));
		this->topBar.setFillColor(sf::Color(80, 80, 80, 255));
		this->topBar.setOutlineThickness(-3);
		this->topBar.setOutlineColor(sf::Color(60, 60, 60, 255));
	}

	//Selector stuff
	{
		this->selector.setSize(sf::Vector2f(this->grid, this->grid));
		this->selector.setFillColor(sf::Color(20, 20, 20, 30));
		this->selector.setOutlineThickness(-1);
		this->selector.setOutlineColor(sf::Color::Black);
	}

	// field gui

	//counter test
	this->counter = new gui::Counter(
		this->data->PercentSize(75.f, 3.f), 
		sf::Vector2f(this->data->PercentSizeX(20.f), 
			this->topBar.getSize().y * 0.75f), 
		this->data->PercentSizeY(8.f), this->data->font, 0);


	//restert button 
	if (this->restartButtonImage.loadFromFile("assets/leny.png"))
	{
		//REBUID GUI TO SET SIZE NOT BY PERCENT VALUE BY DEFAULT!!!
		this->restartButton = new gui::Button(&this->restartButtonImage,
			this->data->PercentSize(25.f, 3.f), sf::Vector2f(this->topBar.getSize().y * 2, this->topBar.getSize().y));
	}
	else
		std::cout << "Cannot load restertButtonImage :: GameState :: Init() \n";

	//Timer
	this->timer = new gui::Timer(
		this->data->PercentSize(5.f, 3.f), 
		sf::Vector2f(this->data->PercentSizeX(20.f), 
		this->topBar.getSize().y * 0.75f), 
		this->data->PercentSizeY(8.f), this->data->font);
}

//Constructors && Destructors
GameState::GameState(Data* data, sf::Vector2i fieldSize)
	:State(data), fieldSize(fieldSize)
{
	this->init();
	this->resizeWindow();
	this->initGui();

	this->minefield = new Minefield(this->grid, this->fieldSize, this->fieldPosition);
}

//Gameplay functions

void GameState::startGame()
{

}

void GameState::resetGame()
{
}



//

GameState::~GameState()
{
	delete this->minefield;
	delete this->counter;
	delete this->restartButton;
	delete this->timer;
}

void GameState::updateInput(const float& deltaTime)
{
	

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && this->getKeytime())
	{
		this->data->Resize(this->data->defaultResolution);
		this->endState();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && this->getKeytime())
	{
		this->counter->increase();
		this->timer->resetTimer();
		std::cout<<"\nXD" << this->counter->getValue();
		
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && this->getKeytime())
	{
		this->counter->increase();
		this->timer->switchActive();
	}

	if (this->minefield->contains(this->mousePosView))
	{
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->getKeytime())
		{
			this->minefield->openTile(this->mousePosGrid.x, (this->mousePosGrid.y - 2));
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && this->getKeytime())
		{

			this->minefield->setFlag(this->mousePosGrid.x, (this->mousePosGrid.y - 2));
		}
	}

}

void GameState::updateGui()
{
	//sets selector on tiles

	if(this->minefield->contains(this->mousePosView))
	{
		this->selector.setPosition(sf::Vector2f(
			this->mousePosGrid.x * this->grid + this->padding,
			this->mousePosGrid.y * this->grid + this->padding));
	}

	this->timer->update();
}

void GameState::update(const float& deltaTime)
{
	this->updateInput(deltaTime);
	this->updateButtons(deltaTime);
	this->updateMousePos(nullptr, this->grid);
	this->updateKeytime(deltaTime);
	this->updateGui();
}

void GameState::updateButtons(const float& deltaTime)
{
	this->restartButton->update(this->mousePosWindow);

	if (this->restartButton->pressed() && this->getKeytime())
	{
		std::cout << "\n RESTERT BUTTON PRESSED\n";
	}
}

void GameState::render(sf::RenderTarget* target)
{
	target->draw(this->BackGround);
	target->draw(this->topBar);
	this->minefield->render(target);
	
	//selector
	target->draw(this->selector);

	//counter
	this->counter->render(target);
	this->restartButton->render(target);
	this->timer->render(target);
}

