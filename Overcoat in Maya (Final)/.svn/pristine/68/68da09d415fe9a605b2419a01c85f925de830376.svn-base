#include <iostream>
#include <string>
#include "vec.h"
#include <vector>


class PointProjection{
private:
	vec3 persCam;
	vec3 viewDir; 
public:
	PointProjection();
	PointProjection(vec3 persCam, vec3 viewDir);
	void depthOrder(vector<vector<vec3>> &splineSets);
	void ThreeDtoTwoD (); 
};
