#include <main.h>
#include "../../src/glbase/scene.h"
#pragma once
class Collisions{
public:
	//Detection par axis aligned bounding boxes
	static bool AABBDetection(const Shape  &shape1, const Shape &shape2);
	//Detection par oriented bounding boxes, algorithme SAT (Separating Axis Theorem)
	static bool OBBDetection(const Shape& shape1, const Shape& shape2);
};