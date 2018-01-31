#pragma once
#include "../../utils/public/types.h"

#include <memory>

class MainWindow;
class Renderer;
class IGame;
class Sprite;
class IInputManager;

typedef std::unique_ptr< MainWindow > WndPtrU;
typedef std::unique_ptr< Renderer > RendererPtrU;

class Engine
{
public:
	Engine();
	~Engine();

	void Init();
	void Start( IGame* game );

	Sprite* CreateSprite();
	void SetInputManager( IInputManager* input );

private:
	WndPtrU			m_window;
	IGame*			m_game;
	RendererPtrU	m_renderer;
	Bool			m_quit;
};