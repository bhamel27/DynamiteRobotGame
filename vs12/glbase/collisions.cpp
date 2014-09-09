#include "collisions.h"


#pragma region COLLISIONS
bool Collisions::AABBDetection(const Shape& shape1, const Shape& shape2){

	std::array<vec3, 2> b1 = shape1.GetBB().GetAABB();
	std::array<vec3, 2> b2 = shape2.GetBB().GetAABB();

	return(b1[0].x > b2[1].x &&
		b1[1].x < b2[0].x &&
		b1[0].y > b2[1].y &&
		b1[1].y < b2[0].y &&
		b1[0].z > b2[1].z &&
		b1[1].z < b2[0].z);

}
bool Collisions::OBBDetection(const Shape& shape1, const Shape& shape2){

	//HYPERPLANE SEPARATION THEOREM (HST)
	//algorithme emprunte du livre 
	//
	//*****************************************************************************************************
	//Ericson, Christer, "Real-Time Collision Detection", Sony Computer Entertainment America, 2005, p.103
	//*****************************************************************************************************
	//
	//Lalgorithme consiste a utiliser les axes des boites 
	//orientees ainsi que leurs produits vectoriels pour
	//projeter leurs points sur les 15 axes obtenus (3+3+3x3) 
	//et determiner si il y a un espace entre 
	//les points projetes, et donc entre les deux boites.
	//S'il y a un espace, on peut tout de suite retourner faux 
	//puisque cela qu'un plan peut s'infiltrer entre les deux boites
	//et donc qu'il n'y a pas de collisions.
	//
	//Pour plus d'information: http://en.wikipedia.org/wiki/Hyperplane_separation_theorem

	BoundingBox a = shape1.GetBB();
	BoundingBox b = shape2.GetBB();
	float ra, rb;
	mat3 R, AbsR;
	// Compute rotation matrix expressing b in a’s coordinate frame
	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			R[i][j] = dot(a.u[i], b.u[j]);
		}

	}


	// Compute translation vector t
	vec3 t = b.c - a.c;
	// Bring translation into a’s coordinate frame
	//ERRATA
	t = vec3(dot(t, a.u[0]), dot(t, a.u[1]), dot(t, a.u[2]));

	//t = vec3(dot(t, a.u[0]), dot(t, a.u[2]), dot(t, a.u[2]));

	// Compute common subexpressions. Add in an epsilon term to
	// counteract arithmetic errors when two edges are parallel and
	// their cross product is (near) null (see text for details)
	for (int i = 0; i < 3; i++)
	for (int j = 0; j < 3; j++)
		AbsR[i][j] = abs(R[i][j]) + epsilon<double>();
	// Test axes L = A0, L = A1, L = A2
	for (int i = 0; i < 3; i++) {
		ra = a.e[i];
		rb = b.e[0] * AbsR[i][0] + b.e[1] * AbsR[i][1] + b.e[2] * AbsR[i][2];
		if (abs(t[i]) > ra + rb) return 0;
	}
	// Test axes L = B0, L = B1, L = B2
	for (int i = 0; i < 3; i++) {
		ra = a.e[0] * AbsR[0][i] + a.e[1] * AbsR[1][i] + a.e[2] * AbsR[2][i];
		rb = b.e[i];
		if (abs(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb) return 0;
	}
	// Test axis L = A0 x B0
	ra = a.e[1] * AbsR[2][0] + a.e[2] * AbsR[1][0];
	rb = b.e[1] * AbsR[0][2] + b.e[2] * AbsR[0][1];
	if (abs(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb) return 0;
	// Test axis L = A0 x B1
	ra = a.e[1] * AbsR[2][1] + a.e[2] * AbsR[1][1];
	rb = b.e[0] * AbsR[0][2] + b.e[2] * AbsR[0][0];
	if (abs(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb) return 0;
	// Test axis L = A0 x B2
	ra = a.e[1] * AbsR[2][2] + a.e[2] * AbsR[1][2];
	rb = b.e[0] * AbsR[0][1] + b.e[1] * AbsR[0][0];
	if (abs(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb) return 0;
	// Test axis L = A1 x B0
	ra = a.e[0] * AbsR[2][0] + a.e[2] * AbsR[0][0];
	rb = b.e[1] * AbsR[1][2] + b.e[2] * AbsR[1][1];
	if (abs(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb) return 0;
	// Test axis L = A1 x B1
	ra = a.e[0] * AbsR[2][1] + a.e[2] * AbsR[0][1];
	rb = b.e[0] * AbsR[1][2] + b.e[2] * AbsR[1][0];
	if (abs(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb) return 0;
	// Test axis L = A1 x B2
	ra = a.e[0] * AbsR[2][2] + a.e[2] * AbsR[0][2];
	rb = b.e[0] * AbsR[1][1] + b.e[1] * AbsR[1][0];
	if (abs(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb) return 0;
	// Test axis L = A2 x B0
	ra = a.e[0] * AbsR[1][0] + a.e[1] * AbsR[0][0];
	rb = b.e[1] * AbsR[2][2] + b.e[2] * AbsR[2][1];
	if (abs(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb) return 0;
	// Test axis L = A2 x B1
	ra = a.e[0] * AbsR[1][1] + a.e[1] * AbsR[0][1];
	rb = b.e[0] * AbsR[2][2] + b.e[2] * AbsR[2][0];
	if (abs(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb) return 0;
	// Test axis L = A2 x B2
	ra = a.e[0] * AbsR[1][2] + a.e[1] * AbsR[0][2];
	rb = b.e[0] * AbsR[2][1] + b.e[1] * AbsR[2][0];
	if (abs(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb) return 0;
	// Since no separating axis is found, the OBBs must be intersecting
	return 1;

}
#pragma endregion

