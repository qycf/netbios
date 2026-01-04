#pragma once
#include "widgets.h"
#include "setting.h"

class c_gui {
public:
	void render();

private:
	void Aim();
	void Pawn();
	void Item();
	void Map();
	void Settting();
};

extern c_gui gui;