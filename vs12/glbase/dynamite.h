#include "../../src/glbase/tp1.h"

struct Dynamite{
	Dynamite(float x, float z, float height, float fuse_length) :
	fuse(0.02, fuse_length, vec3(212.0 / 255, 212.0 / 255, 212.0 / 255)),
	body(0.2, height, vec3(1.0, 0.0, 0.0)),
	body_translation(translate(glm::mat4(), glm::vec3(x, height / 2, z))),
	fuse_translation(translate(glm::mat4(), glm::vec3(0.0, height / 2 + fuse_length / 2, 0.0))),
	explosion_animation(false),
	dynamite_explosion_rotation_f(0.0f),
	dynamite_explosion_scaling_f(1.0f),
	d_to_d_collision_check(false),
	initialization(true),
	show(true),
	fuse_scale_factor(1.0),
	life_time(0),
	blink(999),
	can_cut(true)
	{
	}
	Cylinder body;
	Cylinder fuse;
	int life_time;
	int blink;
	bool d_to_d_collision_check;
	bool initialization;
	bool explosion_animation;
	bool show;
	bool can_cut;
	float x;
	float z;
	float height;
	float fuse_length;
	float dynamite_explosion_rotation_f;
	float dynamite_explosion_scaling_f;
	float fuse_scale_factor;
	mat4 body_translation;
	mat4 fuse_translation;
	mat4 explosion_rotation;
	mat4 explosion_scaling;
	mat4 fuse_scaling;
};