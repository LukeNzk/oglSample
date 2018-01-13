#pragma once
#include "../../utils/public/types.h"
#include "../../engine/public/input.h"
#include "../../engine/public/game.h"

class ImageBuffer;

class Game final : public IGame, public IInputManager
{
public:
	Game();
	~Game() override;

	void LoadResources() override;
	void Tick( Float dt ) override;
	void DispatchEvent( ERIEventType type, void* data ) override;

private:
	ImageBuffer* m_renderTarget;
};
