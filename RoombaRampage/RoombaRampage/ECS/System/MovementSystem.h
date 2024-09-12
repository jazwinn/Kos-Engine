#ifndef MOVESYS_H
#define MOVESYS_H

#include "System.h"

class MovementSystem : public ISystem {

public:

	void Init() override;

	void Update() override;

};

#endif MOVESYS_H