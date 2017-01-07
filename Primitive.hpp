#pragma once

#include <glm/glm.hpp>

class Primitive {
public:
  virtual ~Primitive();
  int m_type;
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
};

class Cube : public Primitive {
public:
  virtual ~Cube();
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const glm::vec3& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
    m_type = 1;
  }
  virtual ~NonhierSphere();
  bool hit(const glm::vec3 &ray,const glm::vec3 &eye,double &hit_point,glm::vec3 &normal);
private:
  glm::vec3 m_pos;
  double m_radius;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const glm::vec3& pos, double size)
    : m_pos(pos), m_size(size)
  {
    m_type = 2;
  }
  bool hit(const glm::vec3 &ray,const glm::vec3 &eye,double &hit_point, glm::vec3 &norm);
  virtual ~NonhierBox();

private:
  glm::vec3 m_pos;
  double m_size;
};
