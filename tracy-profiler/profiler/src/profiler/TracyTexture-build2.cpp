// Compile TracyTexture.cpp with the OpenGL additions upstream makes to
// ImGui's embedded loader (cmake/imgui-loader.patch), which the packaged
// loader does not provide: GL_LINEAR_MIPMAP_LINEAR and
// glCompressedTexImage2D() (see also buildfile).
//
#include <backends/imgui_impl_opengl3_loader.h>

#ifndef GL_LINEAR_MIPMAP_LINEAR
#  define GL_LINEAR_MIPMAP_LINEAR 0x2703
#endif

typedef void (APIENTRYP PFNGLCOMPRESSEDTEXIMAGE2DPROC) (
  GLenum target, GLint level, GLenum internalformat,
  GLsizei width, GLsizei height, GLint border,
  GLsizei imageSize, const void* data);

static void
tracy_glCompressedTexImage2D (GLenum target, GLint level,
                              GLenum internalformat,
                              GLsizei width, GLsizei height, GLint border,
                              GLsizei imageSize, const void* data)
{
  static const PFNGLCOMPRESSEDTEXIMAGE2DPROC f (
    reinterpret_cast<PFNGLCOMPRESSEDTEXIMAGE2DPROC> (
      imgl3wGetProcAddress ("glCompressedTexImage2D")));

  f (target, level, internalformat, width, height, border, imageSize, data);
}

#define glCompressedTexImage2D tracy_glCompressedTexImage2D
#include "TracyTexture.cpp"
#undef glCompressedTexImage2D
