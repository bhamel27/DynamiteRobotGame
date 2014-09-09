#include "scene.h"

#pragma region NODE
int x = 0;
GLint Node::uniform_model = -1, Node::uniform_color = -1;
GLint Node::attribute_position = 1, Node::attribute_normal = 2;

void Node::InitializePreLink(GLuint program)
{
	glBindAttribLocation(program, attribute_position, "in_position");
	glBindAttribLocation(program, attribute_normal, "in_normal");
}

void Node::InitializePostLink(GLuint program)
{
	uniform_model = glGetUniformLocation(program, "model");
	uniform_color = glGetUniformLocation(program, "color");
}

Node::Node()
: _transform(), _children(), _parent(nullptr)
{

}

void Node::SetTransform(const mat4 &transform)
{
	_transform = transform;
}



void Node::AddChild(Node *child)
{
	_children.push_back(child);
	child->_parent = this;
}

Node* Node::GetParent()
{
	return _parent;
}

glm::mat4 Node::fullTransform()
{
	if (_parent == nullptr)

		return _transform;
	else
		return _parent->fullTransform() * _transform;
}


#pragma endregion

#pragma region SHAPE
Shape::Shape()
{}
void Shape::Render()
{
	glUniformMatrix4fv(uniform_model, 1, GL_FALSE, glm::value_ptr(fullTransform()));
	bb.SetTransform(fullTransform());
	glUniform3fv(uniform_color, 1, glm::value_ptr(_color));

}

BoundingBox Shape::GetBB() const {
	return bb;
}

Shape::~Shape()
{
	if (_vertexBuffer != BAD_BUFFER)
		glDeleteBuffers(1, &_vertexBuffer);

	if (_indexBuffer != BAD_BUFFER)
		glDeleteBuffers(1, &_indexBuffer);
}

#pragma endregion

#pragma region BOX
Box::Box(vec3 size, vec3 color, mat4 init_transf) : _size(size)
{

	_vertexBuffer = _indexBuffer = BAD_BUFFER;
	_color = color;
	vertices =
	{ {
		{ vec3(0, 0, 0), vec3(0, -1, 0) },
		{ vec3(1, 0, 0), vec3(0, -1, 0) },
		{ vec3(0, 0, 1), vec3(0, -1, 0) },

		{ vec3(0, 0, 1), vec3(0, -1, 0) },
		{ vec3(1, 0, 0), vec3(0, -1, 0) },
		{ vec3(1, 0, 1), vec3(0, -1, 0) },


		{ vec3(1, 0, 0), vec3(1, 0, 0) },
		{ vec3(1, 1, 0), vec3(1, 0, 0) },
		{ vec3(1, 0, 1), vec3(1, 0, 0) },

		{ vec3(1, 0, 1), vec3(1, 0, 0) },
		{ vec3(1, 1, 0), vec3(1, 0, 0) },
		{ vec3(1, 1, 1), vec3(1, 0, 0) },


		{ vec3(1, 1, 0), vec3(0, 1, 0) },
		{ vec3(0, 1, 1), vec3(0, 1, 0) },
		{ vec3(1, 1, 1), vec3(0, 1, 0) },

		{ vec3(0, 1, 0), vec3(0, 1, 0) },
		{ vec3(0, 1, 1), vec3(0, 1, 0) },
		{ vec3(1, 1, 0), vec3(0, 1, 0) },


		{ vec3(0, 1, 1), vec3(-1, 0, 0) },
		{ vec3(0, 1, 0), vec3(-1, 0, 0) },
		{ vec3(0, 0, 1), vec3(-1, 0, 0) },

		{ vec3(0, 1, 0), vec3(-1, 0, 0) },
		{ vec3(0, 0, 0), vec3(-1, 0, 0) },
		{ vec3(0, 0, 1), vec3(-1, 0, 0) },


		{ vec3(0, 0, 1), vec3(0, 0, 1) },
		{ vec3(1, 0, 1), vec3(0, 0, 1) },
		{ vec3(0, 1, 1), vec3(0, 0, 1) },

		{ vec3(1, 0, 1), vec3(0, 0, 1) },
		{ vec3(1, 1, 1), vec3(0, 0, 1) },
		{ vec3(0, 1, 1), vec3(0, 0, 1) },


		{ vec3(0, 0, 0), vec3(0, 0, -1) },
		{ vec3(0, 1, 0), vec3(0, 0, -1) },
		{ vec3(1, 0, 0), vec3(0, 0, -1) },

		{ vec3(1, 0, 0), vec3(0, 0, -1) },
		{ vec3(0, 1, 0), vec3(0, 0, -1) },
		{ vec3(1, 1, 0), vec3(0, 0, -1) }
	} };

	for (unsigned int x = 0; x < 36; x++){
		vertices[x].position = (vertices[x].position - 0.5f) * _size;
		glm::vec4 v4(vertices[x].position, 1);
		v4 = (init_transf*v4);
		vec3 v3(v4);
		vertices[x].position = v3;
	}
	bb = BoundingBox(GetAABBFromVertices(vertices));



	// Create Vertex Array Objects
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// Generate Vertex Buffer
	glGenBuffers(1, &_vertexBuffer);

	// Fill Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	// Set Vertex Attributes
	glEnableVertexAttribArray(attribute_position);
	glVertexAttribPointer(attribute_position, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionNormal), (const GLvoid*)0);
	glEnableVertexAttribArray(attribute_normal);
	glVertexAttribPointer(attribute_normal, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionNormal), (const GLvoid*)(0 + sizeof(vec3)));

	glBindVertexArray(0);

	debugGLError();
}

void Box::Render()
{
	Shape::Render();
	
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}


#pragma endregion

#pragma region CYLINDER
const int Cylinder::slices = 360;
Cylinder::Cylinder(double radius, double height, vec3 color, mat4& init_transf) : _radius(radius), _height(height)
{
	_vertexBuffer = _indexBuffer = BAD_BUFFER;
	_color = color;

	// Middle vertex of top circle
	vertices[0] = { vec3(0, _height / 2, 0), vec3(0, 1, 0) };
	// Middle vertex of bottom circle
	vertices[slices] = { vec3(0, -(_height / 2), 0), vec3(0, -1, 0) };

	for (int i = 1; i < slices; i++)

	{
		double theta = 2 * glm::pi<double>() / slices - i;
		//vertices of top circle
		vertices[i] = { vec3(sin(theta)*_radius, _height / 2, cos(theta)*_radius), vec3(0, 1, 0) };
		//vertices of bottom circle
		vertices[i + slices] = { vec3(sin(theta)*_radius, -(_height / 2), cos(theta)*_radius), vec3(0, -1, 0) };
		//vertices of sides
		vertices[2 * i + slices * 2] = { vec3(sin(theta)*_radius, _height / 2, cos(theta)*_radius), glm::normalize(vec3(sin(theta)*_radius, 0, cos(theta)*_radius)) };
		vertices[2 * i + (slices * 2 + 1)] = { vec3(sin(theta)*_radius, -(_height / 2), cos(theta)*_radius), glm::normalize(vec3(sin(theta)*_radius, 0, cos(theta)*_radius)) };
	}
	for (unsigned int x = 0; x < 1440; x++){
		glm::vec4 v4(vertices[x].position, 1);
		glm::vec4 v4n(vertices[x].normal, 1);
		v4 = (init_transf*v4);
		v4n = (init_transf*v4n);
		vec3 v3(v4);
		vec3 v3n(v4n);
		vertices[x].position = v3;
		vertices[x].normal = v3n;
	}

	bb = BoundingBox(GetAABBFromVertices(vertices));

	// Create Vertex Array Object
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// Generate Vertex Buffer
	glGenBuffers(1, &_vertexBuffer);

	// Fill Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	// Set Vertex Attributes
	glEnableVertexAttribArray(attribute_position);
	glVertexAttribPointer(attribute_position, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionNormal), (const GLvoid*)0);
	glEnableVertexAttribArray(attribute_normal);
	glVertexAttribPointer(attribute_normal, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionNormal), (const GLvoid*)(0 + sizeof(vec3)));

	glBindVertexArray(0);

	debugGLError();
}

void Cylinder::Render()
{
	Shape::Render();
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, slices);
	glDrawArrays(GL_TRIANGLE_FAN, slices, slices);
	glDrawArrays(GL_TRIANGLE_STRIP, slices * 2, slices * 2);
}

#pragma endregion
#pragma region SPHERE

Sphere::Sphere(double radius, vec3 color) : _radius(radius)
{
	_vertexBuffer = _indexBuffer = BAD_BUFFER;
	_color = color;

	const int nStacks = 40;
	const int nSlices = 40;

	std::array<VertexPositionNormal,9360> vertices;
	int n = 0;

	for (int stack = 0; stack < nStacks; stack++)
	{
		// Vertical angle between the stacks
		double theta1 = ((double)(stack) / nStacks) * glm::pi<double>();
		double theta2 = ((double)(stack + 1) / nStacks) * glm::pi<double>();

		for (int slice = 0; slice < nSlices; slice++)
		{
			// Horizontal angle between the slices
			double phi1 = ((double)(slice) / nSlices) * 2 * glm::pi<double>();
			double phi2 = ((double)(slice + 1) / nSlices) * 2 * glm::pi<double>();

			// Top stack
			if (stack == 0)
			{
				// One triangle per slice, connected to the top vertex
				vec3 vertex1 = vec3(_radius*glm::sin(theta1)*glm::cos(phi1), _radius*glm::sin(theta1)*glm::sin(phi1), _radius*glm::cos(theta1));
				vec3 vertex2 = vec3(_radius*glm::sin(theta2)*glm::cos(phi2), _radius*glm::sin(theta2)*glm::sin(phi2), _radius*glm::cos(theta2));
				vec3 vertex3 = vec3(_radius*glm::sin(theta2)*glm::cos(phi1), _radius*glm::sin(theta2)*glm::sin(phi1), _radius*glm::cos(theta2));
				vec3 normal = -glm::normalize(glm::cross(vertex2 - vertex1, vertex3 - vertex1));
				vertices[n] = { vertex1, normal };
				vertices[n + 1] = { vertex2, normal };
				vertices[n + 2] = { vertex3, normal };
				n += 3;
			}
			// Bottom stack
			else if (stack + 1 == nStacks)
			{
				// One triangle per slice, connected to the bottom vertex
				vec3 vertex1 = vec3(_radius*glm::sin(theta2)*glm::cos(phi2), _radius*glm::sin(theta2)*glm::sin(phi2), _radius*glm::cos(theta2));
				vec3 vertex2 = vec3(_radius*glm::sin(theta1)*glm::cos(phi1), _radius*glm::sin(theta1)*glm::sin(phi1), _radius*glm::cos(theta1));
				vec3 vertex3 = vec3(_radius*glm::sin(theta1)*glm::cos(phi2), _radius*glm::sin(theta1)*glm::sin(phi2), _radius*glm::cos(theta1));
				vec3 normal = -glm::normalize(glm::cross(vertex2 - vertex1, vertex3 - vertex1));
				vertices[n] = { vertex1, normal };
				vertices[n + 1] = { vertex2, normal };
				vertices[n + 2] = { vertex3, normal };
				n += 3;
			}
			else
			{
				// Two triangles per slice, forming a rectangle
				// First triangle
				vec3 vertex1 = vec3(_radius*glm::sin(theta1)*glm::cos(phi1), _radius*glm::sin(theta1)*glm::sin(phi1), _radius*glm::cos(theta1));
				vec3 vertex2 = vec3(_radius*glm::sin(theta1)*glm::cos(phi2), _radius*glm::sin(theta1)*glm::sin(phi2), _radius*glm::cos(theta1));
				vec3 vertex3 = vec3(_radius*glm::sin(theta2)*glm::cos(phi1), _radius*glm::sin(theta2)*glm::sin(phi1), _radius*glm::cos(theta2));
				vec3 normal1 = -glm::normalize(glm::cross(vertex2 - vertex1, vertex3 - vertex1));
				vertices[n] = { vertex1, normal1 };
				vertices[n + 1] = { vertex2, normal1 };
				vertices[n + 2] = { vertex3, normal1 };

				// Second triangle
				vec3 vertex4 = vec3(_radius*glm::sin(theta1)*glm::cos(phi2), _radius*glm::sin(theta1)*glm::sin(phi2), _radius*glm::cos(theta1));
				vec3 vertex5 = vec3(_radius*glm::sin(theta2)*glm::cos(phi2), _radius*glm::sin(theta2)*glm::sin(phi2), _radius*glm::cos(theta2));
				vec3 vertex6 = vec3(_radius*glm::sin(theta2)*glm::cos(phi1), _radius*glm::sin(theta2)*glm::sin(phi1), _radius*glm::cos(theta2));
				vec3 normal2 = -glm::normalize(glm::cross(vertex2 - vertex1, vertex3 - vertex1));
				vertices[n + 3] = { vertex4, normal2 };
				vertices[n + 4] = { vertex5, normal2 };
				vertices[n + 5] = { vertex6, normal2 };
				n += 6;
			}
		}
	}
	
	bb = BoundingBox(GetAABBFromVertices(vertices));
	// Create Vertex Array Object
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// Generate Vertex Buffer
	glGenBuffers(1, &_vertexBuffer);

	// Fill Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	// Set Vertex Attributes
	glEnableVertexAttribArray(attribute_position);
	glVertexAttribPointer(attribute_position, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionNormal), (const GLvoid*)0);
	glEnableVertexAttribArray(attribute_normal);
	glVertexAttribPointer(attribute_normal, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionNormal), (const GLvoid*)(0 + sizeof(vec3)));

	glBindVertexArray(0);

	debugGLError();

}
void Sphere::Init(const mat4& mat){

}
void Sphere::Render()
{
	Shape::Render();
	glBindVertexArray(_vao);
	glDrawArrays(GL_TRIANGLES, 0, 9360);

}
#pragma endregion
