#include <glm/ext.hpp>
#include <ctime>
#include <cstdlib>
#include <stack>

#include "A4.hpp"
using namespace std;
using namespace glm;

std::stack<glm::mat4> mat_stack;
std::vector<GeometryNode *> vec_node;
vec3 vec_zero = vec3(0.0,0.0,0.0);
void flatenTree(SceneNode * node) {
	if (node->m_nodeType == NodeType::GeometryNode) {
		GeometryNode * geo_ptr = static_cast<GeometryNode *>(node);

		vec_node.push_back(geo_ptr);
	}
	for(SceneNode * child: node->children) {
		flatenTree(child);
	}
}

bool recurseCollision(SceneNode* node, const vec3 &eye, const vec3 &ray,
			   vec3 &kd,vec3 &ks,double &shiny,double &point,vec3 &norm) {
	mat_stack.push(mat_stack.top() * node->trans);
	if (node->m_nodeType == NodeType::GeometryNode) {
		GeometryNode * geo_ptr = static_cast<GeometryNode *>(node);

		vec_node.push_back(geo_ptr);
	}
	for(SceneNode * child: node->children) {
	}
}

bool collision(const vec3 &eye, const vec3 &ray,
			   vec3 &kd,vec3 &ks,double &shiny,double &point,vec3 &norm) {
	bool ret_val = false;
	double ret_point = 99;
	for (GeometryNode* geo_ptr:vec_node) {
		bool temp_hit= false;
		double hit_point = 999;
		vec3 normal;
		if(geo_ptr->m_primitive->m_type == 1) {//sphere
			NonhierSphere* nhsphere_ptr = static_cast<NonhierSphere *>(geo_ptr->m_primitive);
			temp_hit = nhsphere_ptr->hit(ray,eye,hit_point,normal);
		}
		else if(geo_ptr->m_primitive->m_type == 2) {//nhbox
			NonhierBox* box_ptr = static_cast<NonhierBox *>(geo_ptr->m_primitive);
			temp_hit = box_ptr->hit(ray,eye,hit_point,normal);
	
		}
		else if(geo_ptr->m_primitive->m_type == 3) {//mesh
			Mesh* mesh_ptr = static_cast<Mesh *>(geo_ptr->m_primitive);
			temp_hit = mesh_ptr->hit(ray,eye,hit_point,normal);
	
		}
		//FIRST BLOOD
		if (temp_hit) {
			if (!ret_val) {
				ret_val = true;
				PhongMaterial * mat_ptr = static_cast<PhongMaterial *>(geo_ptr->m_material);
				kd = mat_ptr->m_kd;
				ks = mat_ptr->m_ks;
				shiny = mat_ptr->m_shininess;
				ret_point = hit_point;
				norm = normal;
			
			}else {
				if (hit_point < ret_point) {
					PhongMaterial * mat_ptr = static_cast<PhongMaterial *>(geo_ptr->m_material);
					kd = mat_ptr->m_kd;
					ks = mat_ptr->m_ks;
					shiny = mat_ptr->m_shininess;
					ret_point = hit_point;
					norm = normal;
					
				}
			}
		}
	}
	if (ret_val) point = ret_point;
	return ret_val;
}
//------------------------------------------
//DONE NO TOUCHY
vec3 makeRay(uint x, uint y,double height, double width,double fovy,
			const vec3 &eye, const vec3 &view, const vec3 &up) {
	double d = eye.z - view.z;
	double big_h = 2*d*tan(radians(fovy/2));
	mat4 T1 = translate(mat4(),vec3(-width/2,-height/2,d));
	mat4 S2 = scale(mat4(),vec3(-big_h/height,-big_h/height,1));
	
	vec3 w = normalize(view - eye);
	vec3 u = normalize(cross(up,w));
	vec3 v = cross(w,u);

	mat4 R3 = mat4(vec4(u,0),vec4(v,0),vec4(w,0),vec4(0.0,0.0,0.0,1.0));
	mat4 T4 = translate(mat4(),eye);

	vec4 retP = T4 * R3 * S2 * T1 * vec4((double)x,(double)y,0.0,1.0);
	vec3 ret_vec = vec3(retP.x,retP.y,retP.z);
	return ret_vec;

}

//DONE NO TOUCHY
glm::vec3 directLight(const Light * light, const vec3 &hit_p , const vec3 &norm) {
	vec3 l = normalize(light->position - hit_p);

	double l_dot_n = glm::max(dot(l,norm),0.0f);
	double len = length(l);

	double t = l_dot_n / 
					  (light->falloff[0] + light->falloff[1]*len + 
					   light->falloff[2]*len*len);

	return light->colour * t;
}

glm::vec3 ggReflect(const Light * light, const vec3 &hit_p,const vec3 &norm) {
	vec3 l = normalize(light->position - hit_p);
	return -l  + 2*norm* (glm::max(dot(l,norm),0.0f));
}


//------------------_RAY TRACING ------------------------------
glm::vec3 rayColour(int depth, 
			   const vec3 &eye,
			   vec3 &ray,
	 		   const list<Light *> & lights, 
	 		   const vec3 &ambient) {
	vec3 col;
	vec3 ks,kd;
	double damping = 1.01f;
	double shiny_pikachu = 0;
	double t_inter = 99;	//intersection point
	vec3 norm;			//normal of the intersected
	if (depth < 2) {
		if (!collision(eye,ray,kd,ks,shiny_pikachu,t_inter,norm)) {
			col = vec_zero;
	 		
		//HIT HERE
		}else {
			norm = normalize(norm);
			col = kd*ambient;
			vec3 hit_p = eye + t_inter*(ray - eye);
			vec3 v = normalize(eye - hit_p);

			for (Light * light: lights) {

				double shadow_cut = 99;
				vec3 junk1,junk2,junk3;
				double junk;
				bool shadow_skip = false;
				vec3 shadow_ray = light->position - hit_p;
				// shadow_skip = collision(not_hit_p,shadow_ray,junk1,junk2,junk,shadow_cut,junk3);
				if (!shadow_skip) {
					if (kd != vec_zero) {
						col += kd *directLight(light,hit_p,norm);
					}
					
				}
				if (ks != vec_zero) {
						vec3 reflect = ggReflect(light,hit_p,norm);

						double pppp;
						// if (minCollision(hit_p,reflect,pppp)) {
						// 	col += ks*rayColour(depth+1,hit_p,reflect,lights,ambient);
						// }else {
							col += ks* pow(glm::max(dot(reflect,v),0.0f),shiny_pikachu)
							         * light->colour;
					//	}
						// vec3 pass = -reflect;
						// 
					}
			}


		}
	}
	return col;
}


//--------------------_RENDER SHIT-----------------------------------------
void A4_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,
		// Lighting parameters
		const glm::vec3 & ambient,
		const std::list<Light *> & lights
) {

  // Fill in raytracing code here...

 /* std::cout << "Calling A4_Render(\n" <<
		  "\t" << *root <<
          "\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
          "\t" << "eye:  " << glm::to_string(eye) << std::endl <<
		  "\t" << "view: " << glm::to_string(view) << std::endl <<
		  "\t" << "up:   " << glm::to_string(up) << std::endl <<
		  "\t" << "fovy: " << fovy << std::endl <<
          "\t" << "ambient: " << glm::to_string(ambient) << std::endl <<
		  "\t" << "lights{" << std::endl;

	for(const Light * light : lights) {
		std::cout << "\t\t" <<  *light << std::endl;
	}
	std::cout << "\t}" << std::endl;
	std:: cout <<")" << std::endl;*/
	srand (time(NULL));
	vec3 super[16];

	size_t h = image.height();
	size_t w = image.width();
	size_t progress = h/10;
	clock_t start_time = clock(); 
	mat_stack.push(glm::mat4());
	flatenTree(root);
	vec3 real_ret = vec_zero;
	for (uint y = 0; y < h; ++y) {
		
		for (uint x = 0; x < w; ++x) {
			vec3 my_ray = makeRay(x,y,(double)w,(double)h,fovy,eye,view,up);
			vec3 ret_col1 = rayColour(0,eye,my_ray,lights,ambient);
			real_ret += ret_col1;
			if (ret_col1 == vec_zero) {
				image(x, y, 0) = (x/(float)h - y/(float)w)/2;
				image(x, y, 1) = y/(float)w;
				image(x, y, 2) = x/(float)h;
				continue;
			}

			for (int i = 0;i <6;i++) {
				for (int j =0;j < 6;j++) {
					vec3 temp_ray = makeRay((double)x + (1.0f/6.0f)*i,(double)y + (1.0f/6.0f)*j,(double)w,(double)h,fovy,eye,view,up);
					real_ret += rayColour(0,eye,temp_ray,lights,ambient);
				}
			}
			
			vec3 ret_col = real_ret/ (37.0f);
			real_ret = vec_zero;
			image(x, y, 0) = glm::min(ret_col.x,1.0f);
			image(x, y, 1) = glm::min(ret_col.y,1.0f);
			image(x, y, 2) = glm::min(ret_col.z,1.0f);
		}


		//UI notification printing msg
		if (y%progress == 0) {
			cout << "Rendering: " << y/progress*10 << " %" << endl;
		}
		//----------------------------
	}

	//UI for time elapse
	clock_t end_time = clock(); 
	cout << "Operation took: " <<  
	    (double)(end_time - start_time)/ CLOCKS_PER_SEC << " seconds" << endl;
}
