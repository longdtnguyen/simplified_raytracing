#include "Primitive.hpp"
#include "polyroots.hpp"
#include <vector>
#include <iostream>
using namespace std;
using namespace glm;
Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}

Cube::~Cube()
{
}
//------------------------------------
NonhierSphere::~NonhierSphere()
{
}

bool NonhierSphere::hit(const vec3 &ray,const vec3 &eye,double &hit_p,vec3 &normal) {
	double a = dot(ray - eye,ray - eye);
	double b = 2 *dot(ray - eye,eye - m_pos);
	double c =  dot(eye- m_pos, eye - m_pos) - m_radius*m_radius;
	double roots[2];
	size_t num_root = quadraticRoots(a,b,c,roots);
	//if (roots[0] >0 && roots[1] >0 && num_root != 0) cout << roots[0] << "  "<< roots[1] << endl;
	if (num_root != 0) {
		if (num_root == 1) {
			hit_p = roots[0];
		}else {
			if (roots[0] > 0 && roots[1] > 0) {
				hit_p = glm::min(roots[0],roots[1]);
			}else if(roots[0] <= 0 && roots[1] <= 0) {
				return false;
			}else {

				return false;
			}
		}
		//---calculating normal
		vec3 hit_me = eye + vec3((ray - eye).x*hit_p,(ray - eye).y*hit_p,(ray - eye).z*hit_p);
		normal = hit_me - m_pos;
		return true;
	}

	return false;

}
//---------------------------------
NonhierBox::~NonhierBox()
{
}

bool NonhierBox::hit(const vec3 &ray,const vec3 &eye,double &hit_point,vec3 &norm) {
	/* glm::vec3 m_pos;
  double m_size;*/
	vec3 p[6];
	vec3 n[6];
	double delta = (double)m_size/2;
	p[0] = vec3(m_pos.x + delta,m_pos.y + delta,m_pos.z);
	p[1] = vec3(m_pos.x + delta,m_pos.y + delta,m_pos.z +  2*delta);
	p[2] = vec3(m_pos.x ,m_pos.y + delta,m_pos.z + delta);
	p[3] = vec3(m_pos.x + 2*delta,m_pos.y + delta,m_pos.z + delta);
	p[4] = vec3(m_pos.x + delta,m_pos.y + 2*delta,m_pos.z + delta);
	p[5] = vec3(m_pos.x + delta,m_pos.y,m_pos.z + delta);

	n[0] = vec3(0,0,-1);
	n[1] = vec3(0,0,1);
	n[2] = vec3(-1,0,0);
	n[3] = vec3(1,0,0);
	n[4] = vec3(0,1,0);
	n[5] = vec3(0,-1,0);
	bool flag = false;
	
	double x1,x2,x3,y1,y2,y3,z1,z2,z3,R1,R2,R3,D,D1,D2,D3;
	double beta,gamma;
	double t = 9999;
	for (int i =0;i<6;i++) {
		if(i <= 1) {
			x1 = 1;
			y1 = 0;
			z1 = 0;

			x2 = 0;
			y2 = 1;
			z2 = 0;
		}else if(i <= 3) {
			x1 = 0;
			y1 = 1;
			z1 = 0;

			x2 = 0;
			y2 = 0;
			z2 = 1;
		}else {
			x1 = 1;
			y1 = 0;
			z1 = 0;

			x2 = 0;
			y2 = 0;
			z2 = 1;
		}

		x3 = eye.x - ray.x;
		y3 = eye.y - ray.y;
		z3 = eye.z - ray.z;

		R1 = eye.x - p[i].x;
		R2 = eye.y - p[i].y;
		R3 = eye.z - p[i].z;

		D = determinant(mat3(vec3(x1,y1,z1),vec3(x2,y2,z2),vec3(x3,y3,z3)));
		D1 = determinant(mat3(vec3(R1,R2,R3),vec3(x2,y2,z2),vec3(x3,y3,z3)));
		D2 = determinant(mat3(vec3(x1,y1,z1),vec3(R1,R2,R3),vec3(x3,y3,z3)));
		D3 = determinant(mat3(vec3(x1,y1,z1),vec3(x2,y2,z2),vec3(R1,R2,R3)));
		
		beta = D1/D;
		gamma = D2/D;
		if (abs(beta) <= m_size/2 && abs(gamma) <= m_size/2 && (beta + gamma) <= m_size) {
			flag = true;
			hit_point = glm::min(hit_point,D3/D);
			if (hit_point == D3/D) {
				norm = n[i];
			}
		}
	}	
	return flag;

}