#pragma once

#include <main.h>
#include "../../vs12/glbase/boundingbox.h"

using namespace glm;


class Node
{
public:
	static void InitializePreLink(GLuint program);
	static void InitializePostLink(GLuint program);

	Node();
	void SetTransform(const mat4 &transform);
	void AddChild(Node *child);
	Node* GetParent();
protected:
	glm::mat4 fullTransform();

	mat4 _transform;
	std::vector<Node*> _children;
	Node* _parent;

	static GLint uniform_model, uniform_color;
	static GLint attribute_position, attribute_normal;
};

class Shape : public Node
{
public:
	Shape();
	virtual void Render();
	
	virtual ~Shape();
	BoundingBox GetBB() const;
protected:
	
	GLuint _vertexBuffer, _indexBuffer, _vao, _vao2;
	vec3 _color;
	BoundingBox bb;
};

class Box : public Shape
{
public:
	Box(vec3 size, vec3 color, mat4 init_transf = mat4());
	virtual void Render() override;

	
protected:
	std::array<VertexPositionNormal, 36> vertices;
	vec3 _size;
};	

class Cylinder : public Shape
{
public:
	static const int slices;
	Cylinder(double radius, double height, vec3 color, mat4& init_transf = mat4());
	virtual void Render() override;
protected:
	double _radius, _height;
	std::array<VertexPositionNormal, 1440> vertices;
};

class Sphere : public Shape
{
public:
	Sphere(double radius, vec3 color);
	
	virtual void Render() override;
	void Init(const mat4 &mat = mat4());
protected:
	double _radius, _height;
};


