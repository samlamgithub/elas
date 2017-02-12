
#ifndef TANGO_POINT_CLOUD_POINT_CLOUD_DRAWABLE_H_
#define TANGO_POINT_CLOUD_POINT_CLOUD_DRAWABLE_H_

#include <jni.h>
#include <vector>

#include <tango-gl/util.h>

namespace hello_motion_tracking {

// PointCloudDrawable is responsible for the point cloud rendering.
class PointCloudDrawable {
 public:
  PointCloudDrawable();

  // Free all GL Resources, i.e, shaders, buffers.
  void DeleteGlResources();

  // Update current point cloud data.
  //
  // @param projection_mat: projection matrix from current render camera.
  // @param view_mat: view matrix from current render camera.
  // @param vertices: all vertices in this point cloud frame.
  void Render(glm::mat4 projection_mat, glm::mat4 view_mat,
              const std::vector<float>& vertices);

 private:
  // Vertex buffer of the point cloud geometry.
  GLuint vertex_buffers_;

  // Shader to display point cloud.
  GLuint shader_program_;

  // Handle to vertex attribute value in the shader.
  GLuint vertices_handle_;

  // Handle to the model view projection matrix uniform in the shader.
  GLuint mvp_handle_;
};
}  // namespace tango_point_cloud

#endif  // TANGO_POINT_CLOUD_POINT_CLOUD_DRAWABLE_H_
