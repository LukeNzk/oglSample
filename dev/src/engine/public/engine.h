#pragma once
#include "../../utils/public/types.h"

#include <memory>

class CWindow;
class Renderer;
class IGame;
class Sprite;
class IInputManager;

typedef std::unique_ptr< CWindow > WndPtrU;
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
	void SetOverlayData( Uint32 width, Uint32 height, const void* data ) const;

private:
	WndPtrU			m_window;
	IGame*			m_game;
	Sprite*			m_overlaySprite;
	RendererPtrU	m_renderer;
	Bool			m_quit;
};