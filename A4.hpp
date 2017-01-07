#pragma once

#include <glm/glm.hpp>

#include "SceneNode.hpp"
#include "GeometryNode.hpp"
#include "PhongMaterial.hpp"
#include "Mesh.hpp"
#include "Light.hpp"
#include "Image.hpp"
#include "polyroots.hpp"
#include <vector>
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
);
//trarvese the tree, check the collision
void checkCollision(SceneNode * node,const glm::vec3 &eye,const glm::vec3 &ray
					,glm::vec3 &kd,glm::vec3  &ks,glm::vec3 &ke,bool &hit,double &point,glm::vec3 &normal);
void flatenTree(SceneNode * node);
bool collision(const glm::vec3 &eye,const glm::vec3 &ray,
			   glm::vec3 &kd,glm::vec3 &ks,
			   double &shiny,double &point,glm::vec3 &norm);
bool recurseCollision(SceneNode* node,const glm::vec3 &eye,const glm::vec3 &ray,
			   		  glm::vec3 &kd,glm::vec3 &ks,
			   		  double &shiny,double &point,glm::vec3 &norm);
glm::vec3 rayColour(int depth, 
	         		const glm::vec3 &eye, 
	         		glm::vec3 &ray,
	 		   		const std::list<Light *> & lights, 
	 		   		const glm::vec3 &ambient);

glm::vec3 directLight(const Light * light, 
	                  const glm::vec3 &hit_p , 
	                  const glm::vec3 &norm);

glm::vec3 ggReflect(const Light * light, 
	                const glm::vec3 &hit_p , 
	                const glm::vec3 &norm);
//make ray
glm::vec3 makeRay(uint x, uint y,double h, double w,double fovy,
			const glm::vec3 &eye, const glm::vec3 &view, const glm::vec3 &up);

