#include "boundingbox.h"
BoundingBox::BoundingBox(){
	set = 0;
}
BoundingBox::BoundingBox(const std::array<vec3, 2>& arr){
	set = 1;
	
	//lorsque l'on est en face du plan y z
	//f: front / r:rear
	//t: top / b:bottom
	//r: right / l:left
	vec3 corner_ftr{ arr[0].x, arr[0].y, arr[0].z };//0
	vec3 corner_rbl{ arr[1].x, arr[1].y, arr[1].z };//1
	vec3 corner_fbr{ corner_ftr.x, corner_rbl.y, corner_ftr.z };//2
	vec3 corner_ftl{ corner_ftr.x, corner_ftr.y, corner_rbl.z };//3
	vec3 corner_fbl{ corner_ftr.x, corner_rbl.y, corner_rbl.z };//4
	vec3 corner_rtr{ corner_rbl.x, corner_ftr.y, corner_ftr.z };//5
	vec3 corner_rtl{ corner_rbl.x, corner_ftr.y, corner_rbl.z };//6
	vec3 corner_rbr{ corner_rbl.x, corner_rbl.y, corner_ftr.z };//7

	init_points = { { corner_ftr, corner_rbl, corner_fbr, corner_ftl, corner_fbl, corner_rtr, corner_rtl, corner_rbr } };
	points = init_points;
	SetCenter();
	SetNormals();
	SetExtents();
	SetAABB();
}bool BoundingBox::IsSet(){
	return set;
}

void BoundingBox::SetAABB(){
	auto resultX = std::minmax_element(points.begin(), points.end(), [](const vec3& lhs, const vec3& rhs) {
		return lhs.x < rhs.x;
	});
	auto resultY = std::minmax_element(points.begin(), points.end(), [](const vec3& lhs, const vec3& rhs) {
		return lhs.y < rhs.y;
	});
	auto resultZ = std::minmax_element(points.begin(), points.end(), [](const vec3& lhs, const vec3& rhs) {
		return lhs.z < rhs.z;
	});

	vec3 corner_max{ resultX.second->x, resultY.second->y, resultZ.second->z };
	vec3 corner_min{ resultX.first->x, resultY.first->y, resultZ.first->z };

	
	aabb[0] = corner_max;
	aabb[1] = corner_min;
}

std::array<vec3, 2> BoundingBox::GetAABB() const{
	return aabb;
}

void BoundingBox::SetTransform(mat4& mat){

	for (int i = 0; i < 8; i++){
		vec4 v4(init_points[i], 1);
		v4 = (mat*v4);
		vec3 v3(v4);
		points[i] = v3;
	}

	SetCenter();
	SetNormals();
	SetExtents();
	SetAABB();
}

void BoundingBox::SetNormals(){
	u.at(0) = normalize(points.at(4) - points.at(1)); //axe des x
	u.at(1) = normalize(points.at(6) - points.at(1)); //axe des y
	u.at(2) = normalize(points.at(7) - points.at(1)); //axe des z
	
	
}
void BoundingBox::SetCenter(){
	auto resultX = std::minmax_element(points.begin(), points.end(), [](const vec3& lhs, const vec3& rhs) {
		return lhs.x < rhs.x;
	});
	auto resultY = std::minmax_element(points.begin(), points.end(), [](const vec3& lhs, const vec3& rhs) {
		return lhs.y < rhs.y;
	});
	auto resultZ = std::minmax_element(points.begin(), points.end(), [](const vec3& lhs, const vec3& rhs) {
		return lhs.z < rhs.z;
	});
	c.x = resultX.first->x + (resultX.second->x - resultX.first->x) / 2;
	c.y = resultY.first->y + (resultY.second->y - resultY.first->y) / 2;
	c.z = resultZ.first->z + (resultZ.second->z - resultZ.first->z) / 2;
}
void BoundingBox::SetExtents(){
	e.at(0) = glm::length(points.at(4) - points.at(1))/2; //fbl-rbl
	e.at(1) = glm::length(points.at(6) - points.at(1))/2; //axe des y
	e.at(2) = glm::length(points.at(7) - points.at(1))/2; //axe des z
}

void BoundingBox::print(std::string name){
	std::cout << name << std::endl;
	std::cout << "center is " << c.x << "," << c.y << "," << c.z << "\n";
	std::cout << "normals are \n" << 
		u[0].x << "," << u[0].y << "," << u[0].z <<"\n" << 
		u[1].x << "," << u[1].y << "," << u[1].z <<"\n" <<
		u[2].x << "," << u[2].y << "," << u[2].z <<"\n";
	float scal = dot(u[0], u[1]);
	std::cout << "first dot product " << scal << "\n";

	scal = dot(u[0], u[2]);
	std::cout << "second dot product " << scal << "\n";

	scal = dot(u[1], u[2]);
	std::cout << "third dot product " << scal << "\n";

	std::cout << "extents are " << e[0] << "," << e[1] << "," << e[2] << "\n";
	for (int i = 0; i < 8; i++){

		printf("%1.2f , %1.2f, %1.2f\n", points[i].x, points[i].y, points[i].z);
	}
	printf("\n");

}
BoundingBox::~BoundingBox(){

}