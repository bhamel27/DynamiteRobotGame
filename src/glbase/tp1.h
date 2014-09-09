#pragma once
#include <main.h>
#include "core.h"
#include "scene.h"
#include "collisions.h"
#include "../../vs12/glbase/dynamite.h"

float RandomNumber(float Min, float Max);



class CoreTP1 : public Core
{
public:
	CoreTP1();
	
	virtual ~CoreTP1() override;

protected:
	virtual void Render(double dt) override;
	void ScissorsAnimation();
	void FusesAnimations();
	void ExplosionAnimation();
	void CheckCollisionsWithTruck();
	void CheckSpawningCollisions(int dynamite_to_check);
	void CreateDynamite();	
	void SpawnDynamites();
	void RenderDynamites(int dynamite_to_render);
	void Reset();
	void BlinkAnimation();
	

	Box plane;
	Box body;
	int random;
	Cylinder wheel_fl;
	Box wheel_fl_box1;
	Box wheel_fl_box2;
	Box wheel_fl_box3;
	Box wheel_fl_box4;
	Cylinder wheel_fr;
	Box wheel_fr_box1;
	Box wheel_fr_box2;
	Box wheel_fr_box3;
	Box wheel_fr_box4;
	Cylinder wheel_rl;
	Box wheel_rl_box1;
	Box wheel_rl_box2;
	Box wheel_rl_box3;
	Box wheel_rl_box4;
	Cylinder wheel_rr;
	Box wheel_rr_box1;
	Box wheel_rr_box2;
	Box wheel_rr_box3;
	Box wheel_rr_box4;
	Cylinder tower;
	Sphere sphere_tower;
	Cylinder cannon;
	Sphere sphere_cannon;
	Box scissor1;
	Box scissor2;	
	float posX;
	float posZ;
	float height;
	float fuse_height;
	std::vector<Dynamite> dynamites;
	std::vector<Node> nodes;	
};

