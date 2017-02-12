#include <sstream>

#include "hello_motion_tracking/point_cloud_drawable.h"

namespace {
const std::string kPointCloudVertexShader =
    "precision mediump float;\n"
    "precision mediump int;\n"
    "attribute vec4 vertex;\n"
    "uniform mat4 mvp;\n"
    "varying vec4 v_color;\n"
    "void main() {\n"
    "  gl_Position = mvp*vertex;\n"
    "  v_color = vertex;\n"
    "}\n";
const std::string kPointCloudFragmentShader =
    "precision mediump float;\n"
    "precision mediump int;\n"
    "varying vec4 v_color;\n"
    "void main() {\n"
    "  gl_FragColor = vec4(v_color);\n"
    "}\n";

}  // namespace

namespace hello_motion_tracking {

PointCloudDrawable::PointCloudDrawable() {
  shader_program_ = tango_gl::util::CreateProgram(
      kPointCloudVertexShader.c_str(), kPointCloudFragmentShader.c_str());

  mvp_handle_ = glGetUniformLocation(shader_program_, "mvp");
  vertices_handle_ = glGetAttribLocation(shader_program_, "vertex");
  glGenBuffers(1, &vertex_buffers_);
}

void PointCloudDrawable::DeleteGlResources() {
  if (vertex_buffers_) {
    glDeleteBuffers(1, &vertex_buffers_);
  }
  if (shader_program_) {
    glDeleteShader(shader_program_);
  }
}

void PointCloudDrawable::Render(glm::mat4 projection_mat, glm::mat4 view_mat,
                                const std::vector<float>& vertices) {
  glUseProgram(shader_program_);
  mvp_handle_ = glGetUniformLocation(shader_program_, "mvp");

  // Calculate model view projection matrix.
  glm::mat4 mvp_mat = projection_mat * view_mat;
  glUniformMatrix4fv(mvp_handle_, 1, GL_FALSE, glm::value_ptr(mvp_mat));

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers_);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(),
               vertices.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(vertices_handle_);
  glVertexAttribPointer(vertices_handle_, 3, GL_FLOAT, GL_FALSE,
                        4 * sizeof(float), nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glDrawArrays(GL_POINTS, 0, vertices.size() / 4);

  glUseProgram(0);
  tango_gl::util::CheckGlError("Pointcloud::Render()");
}

}  // namespace hello_motion_tracking
