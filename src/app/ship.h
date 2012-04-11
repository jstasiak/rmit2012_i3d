#ifndef SHIP_H
#define SHIP_H

#include "obj.h"

class Ship {
private:
	OBJMesh* mesh;
public:
	Ship();
	void draw() const;
	virtual ~Ship();
};

#endif // SHIP_H