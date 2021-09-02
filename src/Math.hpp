#ifndef MATH_HPP
#define MATH_HPP

#include <cstdint>
#include <tuple>
#include <vector>

#include <glm/glm.hpp>

// Create rotation matrix.
glm::mat4 getRotationX(float angle);
glm::mat4 getRotationY(float angle);
glm::mat4 getRotationZ(float angle);
glm::mat4 getRotation(const glm::vec3& ori_x,
                      const glm::vec3& ori_y,
                      const glm::vec3& ori_z);
// Create scaling matrix.
glm::mat4 getScale(const glm::vec3& u);

// Create translation matrix.
glm::mat4 getTranslation(const glm::vec3& u);

// Create View transform.
glm::mat4 getView(const glm::vec3& camera_pos, const glm::vec3& up, const glm::vec3& target);

// Simple factorial.
uint64_t factorial(int n);

// Bernstein polynomial and its derivative.
float bernstein(int n, int i, float x);
float d_bernstein(int n, int i, float x);

// Compute the position and normal vectors at surface point (u,v).
std::tuple<glm::vec3, glm::vec3> bezierSurfaceSample(const std::vector<glm::vec3>& control_points,
                                                     int rows,
                                                     int cols,
                                                     float u,
                                                     float v);

#endif // MATH_HPP
