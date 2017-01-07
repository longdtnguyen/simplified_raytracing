#include <iostream>
#include <fstream>

#include <glm/ext.hpp>

// #include "cs488-framework/ObjFileDecoder.hpp"
#include "Mesh.hpp"
#include "polyroots.hpp"
using namespace std;
using namespace glm;

Mesh::Mesh( const std::string& fname )
	: m_vertices()
	, m_faces()
{
	std::string code;
	double vx, vy, vz;
	size_t s1, s2, s3;
	m_type = 3;
	float min_val_x = 9999;
	float max_val_x = -9999; 
	float min_val_y = 9999;
	float max_val_y = -9999; 
	float min_val_z = 9999;
	float max_val_z = -9999; 
	std::ifstream ifs( fname.c_str() );
	while( ifs >> code ) {
		if( code == "v" ) {
			ifs >> vx >> vy >> vz;
			min_val_x = glm::min(min_val_x,(float)vx);
		
			min_val_y = glm::min(min_val_y,(float)vy);
			min_val_z = glm::min(min_val_z,(float)vz);

			max_val_x = glm::max(max_val_x,(float)vx);
			max_val_y = glm::max(max_val_y,(float)vy);
			max_val_z = glm::max(max_val_z,(float)vz);


			m_vertices.push_back( glm::vec3( vx, vy, vz ) );
		} else if( code == "f" ) {
			ifs >> s1 >> s2 >> s3;
			m_faces.push_back( Triangle( s1 - 1, s2 - 1, s3 - 1 ) );
		}
	}
	double lx = abs(max_val_x - min_val_x);
	double ly = abs(max_val_y - min_val_y);
	double lz = abs(max_val_z - min_val_z);
	if (lx > ly && lx> lz) {
		radius = lx/2;
	}else if(ly > lx && ly > lz) {
		radius = ly/2;
	}else{
		radius = lz/2;
	}
	center = vec3((max_val_x + min_val_x)/2,(max_val_y + min_val_y)/2,(max_val_z + min_val_z)/2);
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  out << "mesh {";
  /*
  
  for( size_t idx = 0; idx < mesh.m_verts.size(); ++idx ) {
  	const MeshVertex& v = mesh.m_verts[idx];
  	out << glm::to_string( v.m_position );
	if( mesh.m_have_norm ) {
  	  out << " / " << glm::to_string( v.m_normal );
	}
	if( mesh.m_have_uv ) {
  	  out << " / " << glm::to_string( v.m_uv );
	}
  }

*/
  out << "}";
  return out;
}


//------------------------------------

bool Mesh::hit(const vec3 &ray, const vec3 &eye,double &hit_point,vec3 &norm) {
	//BOUNDING BOX
	bool ret_flag = false;
	bool bound = false;
	double x1,x2,x3,y1,y2,y3,z1,z2,z3,R1,R2,R3,D,D1,D2,D3;
	double beta,gamma;

	double a = dot(ray - eye,ray - eye);
	double b = 2 *dot(ray - eye,eye - center);
	double c =  dot(eye- center, eye - center) - radius*radius;
	double roots[2];
	size_t num_root = quadraticRoots(a,b,c,roots);
	//if (roots[0] >0 && roots[1] >0 && num_root != 0) cout << roots[0] << "  "<< roots[1] << endl;
	if (num_root != 0) {		
		bound = true;
	}

	if (bound) {
		for (Triangle tri:m_faces) {
			vec3 p0 = m_vertices.at(tri.v1);
			vec3 p1 = m_vertices.at(tri.v2);
			vec3 p2 = m_vertices.at(tri.v3);

			x1 = p1.x - p0.x;
			x2 = p2.x - p0.x;
			x3 = eye.x - ray.x;

			y1 = p1.y - p0.y;
			y2 = p2.y - p0.y;
			y3 = eye.y - ray.y;

			z1 = p1.z - p0.z;
			z2 = p2.z - p0.z;
			z3 = eye.z - ray.z;

			R1 = eye.x - p0.x;
			R2 = eye.y - p0.y;
			R3 = eye.z - p0.z;

			D = determinant(mat3(vec3(x1,y1,z1),vec3(x2,y2,z2),vec3(x3,y3,z3)));
			D1 = determinant(mat3(vec3(R1,R2,R3),vec3(x2,y2,z2),vec3(x3,y3,z3)));
			D2 = determinant(mat3(vec3(x1,y1,z1),vec3(R1,R2,R3),vec3(x3,y3,z3)));
			D3 = determinant(mat3(vec3(x1,y1,z1),vec3(x2,y2,z2),vec3(R1,R2,R3)));
			
			beta = D1/D;
			gamma = D2/D;

			if (beta >= 0 && gamma >=0 && (beta + gamma) <= 1) {
				ret_flag = true;
				hit_point = glm::min(hit_point,D3/D);
				if (hit_point == D3/D) {
					norm = glm::cross((p1 - p0),(p2 - p0));
				}
			}
		}

	}
	//return ret_flag || bound;
	return ret_flag;
}