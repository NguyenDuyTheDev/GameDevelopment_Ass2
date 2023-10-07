/*This source code copyrighted by Lazy Foo' Productions 2004-2023
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <cmath>
#include <time.h>
#include "LTimer.h"

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//Button constants
const int BUTTON_WIDTH = 125;
const int BUTTON_HEIGHT = 50;
const int TOTAL_BUTTONS = 4;

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

//Texture wrapper class
class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

//The mouse button
class LButton
{
public:
	//Initializes internal variables
	LButton(std::string init_button_text= "Play", int init_xPos = 0, int init_yPos = 0);

	//Sets top left position
	void setPosition(int x, int y);
	void setText(std::string nextButtonText);
	void setScreenToSwitch(int screenNewId);

	//Handles mouse event
	void handleEvent(SDL_Event* e);
	bool isPressed();

	//Shows button sprite
	void render();

private:
	//Top left position
	SDL_Point mPosition;
	LTexture gButtonTextTexture;

	//Currently used global sprite
	LButtonSprite mCurrentSprite;
	std::stringstream buttonText;

	int screenToSwitch = 2;
};

class ScoreCounter
{
public:
	//Initializes variables
	ScoreCounter();

	//actions
	void reset();
	void plusScore(int pId);
	int	getHigherScorePlayer();
	int getVictoryPlayer();
	int getStage();

	//render
	void render();

private:
	//The clock time when the timer started
	LTexture gWinnerTexture;
	int p1Score, p2Score;
};

//The dot that will move around on the screen
class Dot
{
public:
	//The dimensions of the dot
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;

	//Maximum axis velocity of the dot
	static const int DOT_VEL = 5;

	//Initializes the variables
	Dot();

	//Takes key presses and adjusts the dot's velocity
	void handleEvent(SDL_Event& e);

	//Moves the dot and checks collision
	void reset();
	void move();

	//checks collision
	bool collide(SDL_Rect& wall);
	void isOutsideMap();
	bool isCollideGoal(SDL_Rect& wall);
	void setIsRooling(bool dotState);

	//Shows the dot on the screen
	void render();

private:
	//The X and Y offsets of the dot
	int mPosX, mPosY;
	bool isRooling = true;

	//The velocity of the dot
	int mVelX, mVelY;

	//Dot's collision box
	SDL_Rect mCollider;
};

class PBar
{
public:
	//Maximum axis velocity of the dot
	static const int BAR_VEL = 20;

	//Initializes the variables
	PBar(int init_player, int init_barId, int init_mPosX, int init_mPosY);

	//Takes key presses and adjusts the bar's velocity
	void handleEvent(SDL_Event& e);

	//Moves the bar
	void move();
	void setPos(int new_mPosX, int new_mPosY);
	void reset();

	//Checks Collision with dot and 4 edge
	void collide(Dot& dot);

	//Shows the bar on the screen
	void render();

private:
	//The dimensions of the bar
	int BAR_WIDTH = 40;
	int BAR_HEIGHT = 100;

	//Player
	int player;
	int barId;

	//The X and Y offsets of the bar
	int mPosX, mPosY;

	//The velocity of the bar
	int mVelX, mVelY;

	//Bar's collision box
	SDL_Rect mCollider;
	LTexture onBarTexture;
	LTexture offBarTexture;

	//is Disable
	bool isDisable = false;
};

class Goal
{
public:
	//The dimensions of the bar
	static const int GOAL_WIDTH = 40;
	static const int GOAL_HEIGHT = 300;

	//Initializes the variables
	Goal(int init_player, int init_mPosX, int init_mPosY);

	//Checks Collision with dot and 4 edge
	bool collide(Dot& dot, ScoreCounter& scoreCounter);

	//Shows the bar on the screen
	void render();

private:
	//Player
	int player;

	//The X and Y offsets of the bar
	int mPosX, mPosY;

	//Bar's collision box
	SDL_Rect mCollider;
};

class Wall
{
public:
	//The dimensions of the bar
	//static const int WALL_WIDTH = 40;
	//static const int WALL_HEIGHT = 300;

	//Initializes the variables
	Wall(int init_width, int init_height, int init_mPosX, int init_mPosY);

	//Checks Collision with dot and 4 edge
	void collide(Dot& dot);

	//Shows the bar on the screen
	void render();

private:
	//The dimensions of the bar
	int WALL_WIDTH;
	int WALL_HEIGHT;

	//The X and Y offsets of the bar
	int mPosX, mPosY;

	//Bar's collision box
	SDL_Rect mCollider;
};

class MainMenu {
public:
	MainMenu();
	void render();
	void handleEvent(SDL_Event* e);

private:
	LTexture gTitleTexture;
	LButton gStartButton;
	LButton gAdvanceButton;
	LButton gBotButton;
	LButton gExitButton;
};

class ResultMenu {
public:
	ResultMenu();
	void render();
	void handleEvent(SDL_Event* e);

private:
	LTexture gTitleTexture;
	LButton gRestartButton;
	LButton gMainmenuButton;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

void resetGame(ScoreCounter* scorecounter);

//Frees media and shuts down SDL
void close();

//Box collision detector
bool checkCollision(SDL_Rect a, SDL_Rect b);

//Screen Id
int screenId = 1;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gDotTexture;

//Globally used font
TTF_Font* gFont = NULL;

//Rendered texture
LTexture gTextTexture;

//Rendered Time texture
LTexture gPromptTextTexture;
LTexture gTimeTextTexture;
LTexture gNewStateCountdownTextTexture;
LTexture gFPSTextTexture;
LTexture gBackGroundTexture;

ScoreCounter scoreCounter;

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return mTexture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

LButton::LButton(std::string init_button_text, int init_xPos, int init_yPos)
{
	mPosition.x = init_xPos;
	mPosition.y = init_yPos;

	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;

	buttonText.str(init_button_text);
	SDL_Color textColor = { 0, 0, 0, 255 };
	gButtonTextTexture.loadFromRenderedText(buttonText.str().c_str(), textColor);
}

void LButton::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::setText(std::string nextButtonText)
{
	buttonText.str(nextButtonText);
	SDL_Color textColor = { 0, 0, 0, 255 };
	gButtonTextTexture.loadFromRenderedText(buttonText.str().c_str(), textColor);
}

void LButton::setScreenToSwitch(int screenNewId)
{
	screenToSwitch = screenNewId;
}

void LButton::handleEvent(SDL_Event* e)
{
	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if (x < mPosition.x)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (x > mPosition.x + gButtonTextTexture.getWidth())
		{
			inside = false;
		}
		//Mouse above the button
		else if (y < mPosition.y)
		{
			inside = false;
		}
		//Mouse below the button
		else if (y > mPosition.y + gButtonTextTexture.getHeight())
		{
			inside = false;
		}

		//Mouse is outside button
		if (!inside)
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch (e->type)
			{
			case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;

			case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				screenId = screenToSwitch;

				if (screenToSwitch == 2) {
					resetGame(&scoreCounter);
				}

				break;

			case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
				break;
			}
		}
	}
}

bool LButton::isPressed()
{
	return mCurrentSprite == BUTTON_SPRITE_MOUSE_DOWN;
}

void LButton::render()
{
	SDL_Color textColor = { 0, 0, 0, 0 };
	switch (mCurrentSprite)
	{
		case BUTTON_SPRITE_MOUSE_OUT:
			textColor = { 0, 0, 0, 255 };
			break;
		case BUTTON_SPRITE_MOUSE_OVER_MOTION:
			textColor = { 255, 0, 0, 255 };
			break;
		case BUTTON_SPRITE_MOUSE_DOWN:
			textColor = { 0, 255, 0, 255 };
			break;
		case BUTTON_SPRITE_MOUSE_UP:
			textColor = { 0, 0, 255, 255 };
			break;
		case BUTTON_SPRITE_TOTAL:
			textColor = { 0, 0, 0, 125 };
			break;
	}

	gButtonTextTexture.loadFromRenderedText(buttonText.str().c_str(), textColor);
	gButtonTextTexture.render(mPosition.x, mPosition.y);
}

Dot::Dot()
{
	//Initialize the offsets
	mPosX = SCREEN_WIDTH / 2;
	mPosY = SCREEN_HEIGHT / 2;

	//Set collision box dimension
	mCollider.w = DOT_WIDTH;
	mCollider.h = DOT_HEIGHT;

	//Initialize the velocity
	srand(time(NULL));
	mVelX = rand() % 6 + 5;
	mVelY = rand() % 6 + 5;

	if (rand() % 100 % 1 <= 50) mVelX *= -1;
	if (rand() % 100 % 1 <= 50) mVelY *= -1;
}

void Dot::handleEvent(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY -= DOT_VEL; break;
		case SDLK_DOWN: mVelY += DOT_VEL; break;
		case SDLK_LEFT: mVelX -= DOT_VEL; break;
		case SDLK_RIGHT: mVelX += DOT_VEL; break;
		}
	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY += DOT_VEL; break;
		case SDLK_DOWN: mVelY -= DOT_VEL; break;
		case SDLK_LEFT: mVelX += DOT_VEL; break;
		case SDLK_RIGHT: mVelX -= DOT_VEL; break;
		}
	}
}

void Dot::reset()
{
	//Initialize the offsets
	mPosX = SCREEN_WIDTH / 2;
	mPosY = SCREEN_HEIGHT / 2;

	//Set collision box dimension
	mCollider.w = DOT_WIDTH;
	mCollider.h = DOT_HEIGHT;
	
	//Initialize the velocity
	int stage = scoreCounter.getStage();
	mVelX = rand() % std::min(5 + stage, 15) + 5;
	mVelY = rand() % std::min(5 + stage, 15) + 5;

	int higherScorePlayer = scoreCounter.getHigherScorePlayer();
	switch (higherScorePlayer)
	{
	case 1:
		mVelX *= -1;
		break;
	case 2:
		mVelX *= 1;
		break;
	default:
		if (rand() % 100 % 1 <= 50) mVelX *= -1;
		break;
	}

	if (rand() % 100 % 1 <= 50) mVelY *= -1;
}

void Dot::move()
{
	if (isRooling == false) {
		return;
	}
	//Move the dot left or right
	mPosX += mVelX;
	mCollider.x = mPosX;

	//Move the dot up or down
	mPosY += mVelY;
	mCollider.y = mPosY;
}

bool Dot::collide(SDL_Rect& wall) {
	bool isCollide = false;
	//If the dot collided or went too far to the left or right
	if ((mPosX < 0) || (mPosX + DOT_WIDTH > SCREEN_WIDTH) || (checkCollision(mCollider, wall)))
	{
		//Move back
		mPosX -= mVelX;
		mCollider.x = mPosX;
		mVelX *= -1;
		isCollide = true;
	}
	//If the dot collided or went too far up or down
	if ((mPosY < 100) || (mPosY + DOT_HEIGHT > SCREEN_HEIGHT) || (checkCollision(mCollider, wall)))
	{
		//Move back
		mPosY -= mVelY;
		mCollider.y = mPosY;
		mVelY *= -1;
		isCollide = true;
	}
	return isCollide;
}

void Dot::isOutsideMap() {
	while ((mPosX < 0) || (mPosX + DOT_WIDTH > SCREEN_WIDTH))
	{
		//Move back
		mPosX -= mVelX;
		mCollider.x = mPosX;
		//mVelX *= -1;
	}
	//If the dot collided or went too far up or down
	while ((mPosY < 100) || (mPosY + DOT_HEIGHT > SCREEN_HEIGHT))
	{
		//Move back
		mPosY -= mVelY;
		mCollider.y = mPosY;
		//mVelY *= -1;
	}
}

bool Dot::isCollideGoal(SDL_Rect& wall) {
	return checkCollision(mCollider, wall);
}

void Dot::setIsRooling(bool dotState) {
	isRooling = dotState;
}


void Dot::render()
{
	//Show the dot
	gDotTexture.render(mPosX, mPosY);
}

PBar::PBar(int init_player, int init_barId, int init_mPosX, int init_mPosY)
{
	//player
	player = init_player;
	barId = init_barId;

	//Initialize the offsets
	mPosX = init_mPosX;
	mPosY = init_mPosY;

	//Set collision box dimension
	mCollider.w = BAR_WIDTH;
	mCollider.h = BAR_HEIGHT;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;

	onBarTexture.loadFromFile("image/paddleBlu.png");
	offBarTexture.loadFromFile("image/paddleRed.png");

	mCollider.x = init_mPosX;
	mCollider.y = init_mPosY;
	mCollider.w = onBarTexture.getWidth();

	if (barId == 2)
	{
		mCollider.h = onBarTexture.getHeight() * 2;
	}
	else
	{
		mCollider.h = onBarTexture.getHeight();
	}
}

void PBar::handleEvent(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN)
	{
		switch (e.key.keysym.sym)
		{
			case SDLK_1: 
				if (player == 1 && barId == 1) {
					isDisable = false;
				} else if (player == 1 && barId == 2) {
					isDisable = true;
				}
				break;
			case SDLK_2:
				if (player == 1 && barId == 1) {
					isDisable = true;
				}
				else if (player == 1 && barId == 2) {
					isDisable = false;
				}
				break;
			case SDLK_3:
				if (player == 1 && barId == 1) {
					isDisable = false;
				}
				else if (player == 1 && barId == 2) {
					isDisable = false;
				}
				break;
			case SDLK_8:
				if (player == 2 && barId == 1) {
					isDisable = false;
				}
				else if (player == 2 && barId == 2) {
					isDisable = true;
				}
				break;
			case SDLK_9:
				if (player == 2 && barId == 1) {
					isDisable = true;
				}
				else if (player == 2 && barId == 2) {
					isDisable = false;
				}
				break;
			case SDLK_0:
				if (player == 2 && barId == 1) {
					isDisable = false;
				}
				else if (player == 2 && barId == 2) {
					isDisable = false;
				}
				break;
		}
	}
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		if (player == 1) {
			//Adjust the velocity
			switch (e.key.keysym.sym)
			{
				case SDLK_w: mVelY -= BAR_VEL; break;
				case SDLK_s: mVelY += BAR_VEL; break;
				//case SDLK_LEFT: mVelX -= BAR_VEL; break;
				//case SDLK_RIGHT: mVelX += BAR_VEL; break;
			}
		}
		else {
			//Adjust the velocity
			switch (e.key.keysym.sym)
			{
				case SDLK_UP: mVelY -= BAR_VEL; break;
				case SDLK_DOWN: mVelY += BAR_VEL; break;
				//case SDLK_LEFT: mVelX -= BAR_VEL; break;
				//case SDLK_RIGHT: mVelX += BAR_VEL; break;
			}
		}
	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		if (player == 1) {
			//Adjust the velocity
			switch (e.key.keysym.sym)
			{
				case SDLK_w: mVelY += BAR_VEL; break;
				case SDLK_s: mVelY -= BAR_VEL; break;
				//case SDLK_LEFT: mVelX -= BAR_VEL; break;
				//case SDLK_RIGHT: mVelX += BAR_VEL; break;
			}
		}
		else {
			//Adjust the velocity
			switch (e.key.keysym.sym)
			{
				case SDLK_UP: mVelY += BAR_VEL; break;
				case SDLK_DOWN: mVelY -= BAR_VEL; break;
				//case SDLK_LEFT: mVelX -= BAR_VEL; break;
				//case SDLK_RIGHT: mVelX += BAR_VEL; break;
			}
		}
	}
}

void PBar::move()
{
	if (isDisable) return;

	//Move the dot left or right
	mPosX += mVelX;
	mCollider.x = mPosX;

	//Move the dot up or down
	mPosY += mVelY;
	mCollider.y = mPosY;
}

void PBar::setPos(int new_mPosX, int new_mPosY) {
	//Move the dot left or right
	mPosX = new_mPosX;
	mCollider.x = mPosX;

	//Move the dot up or down
	mPosY = new_mPosY;
	mCollider.y = mPosY;
}

void PBar::reset()
{
	mVelX = 0;
	mVelY = 0;
}

void PBar::collide(Dot& dot) {
	bool isCollide = dot.collide(mCollider);
	//If the bar collided or went too far up or down
	if ((mPosY < 100) || (mPosY + mCollider.h > SCREEN_HEIGHT) || isCollide)
	{
		//Move back
		mPosY -= mVelY;
		mCollider.y = mPosY;
	}
}

void PBar::render()
{
	if (isDisable) {
		//SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		offBarTexture.render(mCollider.x, mCollider.y);
		if (barId == 2) {
			offBarTexture.render(mCollider.x, mCollider.y + offBarTexture.getHeight());
		}
	}
	else {
		//SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
		onBarTexture.render(mCollider.x, mCollider.y);
		if (barId == 2) {
			onBarTexture.render(mCollider.x, mCollider.y + onBarTexture.getHeight());
		}
	}
	//SDL_RenderDrawRect(gRenderer, &mCollider);
}

Goal::Goal(int init_player, int init_mPosX, int init_mPosY)
{
	//player
	player = init_player;

	//Initialize the offsets
	mPosX = init_mPosX;
	mPosY = init_mPosY;

	//Set collision box dimension
	mCollider.w = GOAL_WIDTH;
	mCollider.h = GOAL_HEIGHT;

	mCollider.x = init_mPosX;
	mCollider.y = init_mPosY;
	mCollider.w = GOAL_WIDTH;
	mCollider.h = GOAL_HEIGHT;
}

bool Goal::collide(Dot& dot, ScoreCounter& scoreCounter) {
	if (dot.isCollideGoal(mCollider)) {
		if (player == 1) {
			scoreCounter.plusScore(2);
		}
		if (player == 2) {
			scoreCounter.plusScore(1);
		}
		dot.reset();
		return true;
	}
	return false;
}

void Goal::render()
{
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
	SDL_RenderDrawRect(gRenderer, &mCollider);
}

Wall::Wall(int init_width, int init_height, int init_mPosX, int init_mPosY)
{
	WALL_HEIGHT = init_height;
	WALL_WIDTH = init_width;

	//Initialize the offsets
	mPosX = init_mPosX;
	mPosY = init_mPosY;

	//Set collision box dimension
	mCollider.w = WALL_WIDTH;
	mCollider.h = WALL_HEIGHT;

	mCollider.x = init_mPosX;
	mCollider.y = init_mPosY;
	mCollider.w = WALL_WIDTH;
	mCollider.h = WALL_HEIGHT;
}

void Wall::collide(Dot& dot) {
	dot.collide(mCollider);
}

void Wall::render()
{
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
	SDL_RenderDrawRect(gRenderer, &mCollider);
}

ScoreCounter::ScoreCounter()
{
	p1Score = 0;
	p2Score = 0;
}

void ScoreCounter::reset()
{
	p1Score = 0;
	p2Score = 0;
}

void ScoreCounter::plusScore(int pId)
{
	if (pId == 1) {
		p1Score += 1;
	}
	if (pId == 2) {
		p2Score += 1;
	}
}

int ScoreCounter::getHigherScorePlayer() {
	if (p1Score > p2Score) {
		return 1;
	}
	if (p1Score < p2Score) {
		return 2;
	}
	return 0;
}

int ScoreCounter::getVictoryPlayer() {
	if (p1Score == 3) {
		return 1;
	}
	if (p2Score == 3) {
		return 2;
	}
	return 0;
}

int ScoreCounter::getStage() {
	return std::min(p1Score, p2Score) + 1;
}

void ScoreCounter::render()
{
	if (screenId == 2) {
		//In memory text stream
		std::stringstream scoreText;

		scoreText.str("");
		scoreText << p1Score << " : " << p2Score;

		//Render text
		SDL_Color textColor = { 0, 0, 0, 255 };
		gTextTexture.loadFromRenderedText(scoreText.str().c_str(), textColor);
		gTextTexture.render((SCREEN_WIDTH - gTextTexture.getWidth()) / 2, 25);
	}
	else if (screenId == 3) {
		//In memory text stream
		SDL_Color textColor = { 0, 0, 0, 255 };

		std::stringstream scoreText;
		scoreText.str("");
		scoreText << p1Score << " : " << p2Score;

		//Render text
		gTextTexture.loadFromRenderedText(scoreText.str().c_str(), textColor);
		gTextTexture.render((SCREEN_WIDTH - gTextTexture.getWidth()) / 2, 250);

		std::stringstream winnerText;
		winnerText.str("");
		winnerText << "Player " << this->getVictoryPlayer() << " win";

		gWinnerTexture.loadFromRenderedText(winnerText.str().c_str(), textColor);
		gWinnerTexture.render((SCREEN_WIDTH - gWinnerTexture.getWidth()) / 2, 300);
	}
}

MainMenu::MainMenu()
{
	SDL_Color textColor = { 0, 0, 0, 255 };

	std::stringstream title;
	title.str("Ping Pong Remastered");
	if (!gTitleTexture.loadFromRenderedText(title.str().c_str(), textColor))
	{
		printf("Unable to render time texture!\n");
	}

	int centerX = (SCREEN_WIDTH / 2 - gTitleTexture.getWidth() / 2);
	gStartButton.setPosition(centerX, 300);
	gStartButton.setText("Standard Mode");
	gStartButton.setScreenToSwitch(2);

	gAdvanceButton.setPosition(centerX, 350);
	gAdvanceButton.setText("Expert Mode");
	gAdvanceButton.setScreenToSwitch(2);

	gBotButton.setPosition(centerX, 400);
	gBotButton.setText("Bot Mode");
	gBotButton.setScreenToSwitch(2);

	gExitButton.setPosition(centerX, 500);
	gExitButton.setText("Exit");
	gExitButton.setScreenToSwitch(0);
}

void MainMenu::render()
{
	int centerX = (SCREEN_WIDTH / 2 - gTitleTexture.getWidth() / 2);
	gTitleTexture.render(centerX, 200);
	gStartButton.render();
	//gAdvanceButton.render();
	gBotButton.render();
	gExitButton.render();
}

void MainMenu::handleEvent(SDL_Event* e)
{
	gStartButton.handleEvent(e);
	gAdvanceButton.handleEvent(e);
	gBotButton.handleEvent(e);
	gExitButton.handleEvent(e);
}

ResultMenu::ResultMenu()
{
	SDL_Color textColor = { 0, 0, 0, 255 };

	std::stringstream title;
	title.str("Result");
	if (!gTitleTexture.loadFromRenderedText(title.str().c_str(), textColor))
	{
		printf("Unable to render time texture!\n");
	}

	int centerX = (SCREEN_WIDTH / 2);
	gRestartButton.setPosition(centerX - 125, 400);
	gRestartButton.setText("New Game");
	gRestartButton.setScreenToSwitch(2);

	gMainmenuButton.setPosition(centerX - 250, 450);
	gMainmenuButton.setText("Back to Main Menu");
	gMainmenuButton.setScreenToSwitch(1);
}

void ResultMenu::render()
{
	int centerX = (SCREEN_WIDTH / 2);
	gTitleTexture.render(centerX - gTitleTexture.getWidth() / 2, 150);
	gRestartButton.render();
	gMainmenuButton.render();
}

void ResultMenu::handleEvent(SDL_Event* e)
{
	gRestartButton.handleEvent(e);
	gMainmenuButton.handleEvent(e);
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Initialize PNG loading
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			success = false;
		}

		//Initialize SDL_ttf
		if (TTF_Init() == -1)
		{
			printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
			success = false;
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load press texture
	if (!gDotTexture.loadFromFile("image/ball.png"))
	{
		printf("Failed to load dot texture!\n");
		success = false;
	}

	if (!gBackGroundTexture.loadFromFile("image/groundGrass_mown1.png"))
	{
		printf("Failed to load dot texture!\n");
		success = false;
	}

	//Open the font
	gFont = TTF_OpenFont("font/Cartos.ttf", 50);
	if (gFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

	return success;
}

void resetGame(ScoreCounter* scorecounter)
{
	scoreCounter.reset();
}

void close()
{
	//Free loaded images
	gDotTexture.free();

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool checkCollision(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;
			bool isIngame = false;
			bool isInitialGame = false;
			bool isNewStage = false;

			MainMenu mainmenu;
			ResultMenu resultmenu;

			//Event handler
			SDL_Event e;

			//Set text color as black
			SDL_Color textColor = { 0, 0, 0, 255 };

			//The application timer
			LTimer timer;
			LTimer countdownTimer;

			//The frames per second timer
			LTimer fpsTimer;

			//In memory text stream
			std::stringstream timeText;
			std::stringstream countdownTimeText;

			//In memory text stream
			std::stringstream fpsTimeText;

			//Start counting frames per second
			int countedFrames = 0;
			fpsTimer.start();
			timer.start();
			countdownTimer.start();

			//The dot that will be moving around on the screen
			Dot dot;

			PBar p1_bar1_obj(1, 1, 50, SCREEN_HEIGHT / 2 - 50);
			PBar p1_bar2_obj(1, 2, SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 2 - 50);
			PBar p2_bar1_obj(2, 1, SCREEN_WIDTH - 100, SCREEN_HEIGHT / 2 - 50);
			PBar p2_bar2_obj(2, 2, SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 - 50);

			Goal p1Goal(1, 0, SCREEN_HEIGHT / 2 - 150);
			Goal p2Goal(2, SCREEN_WIDTH - 40, SCREEN_HEIGHT / 2 - 150);

			Wall topWall(SCREEN_WIDTH, 100, 0, 0);

			//While application is running
			while (!quit)
			{
				if (screenId == 0) {
					quit = true;
				}
				else if (screenId == 1) {
					while (SDL_PollEvent(&e) != 0)
					{
						if (e.type == SDL_QUIT)
						{
							quit = true;
						}

						mainmenu.handleEvent(&e);
					}

					//Clear screen
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(gRenderer);

					mainmenu.render();
				}
				else if (screenId == 2) {
					if (isInitialGame == true)
					{
						p1_bar1_obj.setPos(50, SCREEN_HEIGHT / 2 - 50);
						p1_bar2_obj.setPos(SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 2 - 50);
						p2_bar1_obj.setPos(SCREEN_WIDTH - 100, SCREEN_HEIGHT / 2 - 50);
						p2_bar2_obj.setPos(SCREEN_WIDTH / 2 + 300, SCREEN_HEIGHT / 2 - 50);
						p1_bar1_obj.reset();
						p1_bar2_obj.reset();
						p2_bar1_obj.reset();
						p2_bar2_obj.reset();
						dot.reset();
						timer.start();
						isInitialGame = false;
						scoreCounter.reset();
						countdownTimer.start();
					}
					if (countdownTimer.getTicks() >= 1000 / 60)
					{
						dot.setIsRooling(false);
					}
					if (countdownTimer.getTicks() >= 3000)
					{
						countdownTimer.stop();
						dot.setIsRooling(true);
					}

					//Handle events on queue
					while (SDL_PollEvent(&e) != 0)
					{
						//User requests quit
						if (e.type == SDL_QUIT)
						{
							quit = true;
						}
						//Reset start time on return keypress
						else if (e.type == SDL_KEYDOWN)
						{
							if (e.key.keysym.sym == SDLK_ESCAPE)
							{
								screenId = 1;
								isInitialGame = true;
							}
						}

						p1_bar1_obj.handleEvent(e);
						p1_bar2_obj.handleEvent(e);
						p2_bar1_obj.handleEvent(e);
						p2_bar2_obj.handleEvent(e);
					}

					//Action 
					//Calculate and correct fps
					float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
					if (avgFPS > 2000000)
					{
						avgFPS = 0;
					}

					int victory = scoreCounter.getVictoryPlayer();
					if (victory == 1 || victory == 2) {
						isInitialGame = true;
						timer.stop();
						screenId = 3;
					} 

					//Move the dot and check collision
					dot.move();
					p1_bar1_obj.move();
					p1_bar2_obj.move();
					p2_bar1_obj.move();
					p2_bar2_obj.move();

					p1_bar1_obj.collide(dot);
					p1_bar2_obj.collide(dot);
					p2_bar1_obj.collide(dot);
					p2_bar2_obj.collide(dot);
					if (p1Goal.collide(dot, scoreCounter) 
						|| p2Goal.collide(dot, scoreCounter)) {
						countdownTimer.start();
					}

					//Clear screen
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(gRenderer);

					//Render Background
					for (int background_x = 0; background_x < SCREEN_WIDTH; background_x += gBackGroundTexture.getWidth())
					{
						for (int background_y = 100; background_y < SCREEN_HEIGHT; background_y += gBackGroundTexture.getHeight())
						{
							gBackGroundTexture.render(background_x, background_y);
						}
					}

					//Render wall
					p1_bar1_obj.render();
					p1_bar2_obj.render();
					p2_bar1_obj.render();
					p2_bar2_obj.render();
					p1Goal.render();
					p2Goal.render();
					topWall.render();

					//Set text to be rendered
					timeText.str("");
					timeText << (std::floor(timer.getTicks() / 1000.f)) << "s";

					//Set text to be rendered
					fpsTimeText.str("");
					fpsTimeText << std::floor(avgFPS) << " FPS";

					countdownTimeText.str("");
					countdownTimeText << std::ceil((4000 - countdownTimer.getTicks()) / 1000);

					//Render text
					if (!gTimeTextTexture.loadFromRenderedText(timeText.str().c_str(), textColor))
					{
						printf("Unable to render time texture!\n");
					}

					gFPSTextTexture.loadFromRenderedText(fpsTimeText.str().c_str(), textColor);
					gNewStateCountdownTextTexture.loadFromRenderedText(countdownTimeText.str().c_str(), textColor);

					//Render dot
					dot.render();

					//Render current frame
					scoreCounter.render();

					//Render textures
					gTimeTextTexture.render((SCREEN_WIDTH - gTimeTextTexture.getWidth()), (gTimeTextTexture.getHeight()));
					gFPSTextTexture.render((SCREEN_WIDTH - gFPSTextTexture.getWidth()), 0);

					if ((countdownTimer.getTicks() != 0 && countdownTimer.getTicks() < 3000) || ( timer.getTicks() != 0  && timer.getTicks() < 3000))
					{
						gNewStateCountdownTextTexture.render(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
					}
				}
				else if (screenId == 3)
				{
					while (SDL_PollEvent(&e) != 0)
					{
						if (e.type == SDL_QUIT)
						{
							quit = true;
						}
						resultmenu.handleEvent(&e);
					}

					//Clear screen
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
					SDL_RenderClear(gRenderer);

					scoreCounter.render();
					resultmenu.render();
				}

				//Update screen
				SDL_RenderPresent(gRenderer);
				++countedFrames;
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
