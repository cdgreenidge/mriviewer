/* Required headers:
 * - FL/Fl.H
 * - FL/gl.H
 * - FL/Fl_Gl_Window.H
 * - boost/multi_array.hpp
 * - image.h
 * - slice.h
 * - model.h
 * - mri.h
 */

struct Labels {
  const char *const right;
  const char *const top;
  const char *const left;
  const char *const bottom;
};

class MriWindow : public Fl_Gl_Window {
 public:
  MriWindow(const int x, const int y, const int w, const int h,
            const char *const i, const Mri mri, Model &model, const Plane plane,
            const Labels labels);

 protected:
  Mri mri_;
  Model &model_;
  Plane plane_;
  Image image_;
  Labels labels_;

  float maxLength_;
  float bottom_;
  float left_;
  struct {
    GLuint vertexBuffer;
    GLuint elementBuffer;
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint program;
    struct {
      GLuint position;
      GLuint texcoord;
    } attributes;
    struct {
      GLuint texture;
      GLint overlayColor;
      GLint overlayAlpha;
    } uniforms;
    GLuint texture;
  } resources_;

  /* Event handling */
  int handle(int event);

  /* Rendering */
  void draw();
  GLuint makeShader(GLenum type, const char *filename);
  GLuint makeProgram(GLuint vertexShader, GLuint fragmentShader);
  GLuint makeTexture(size_t bufWidth, size_t bufHeight);
  GLfloat getMriWidthCoord();
  GLfloat getMriHeightCoord();
  GLfloat getCrosshairXCoord(GLfloat left);
  GLfloat getCrosshairYCoord(GLfloat bottom);
  GLuint makeBuffer(GLenum target, const void *bufferData, GLsizei bufferSize);
  void gl_draw_centered(const char *string, float x, float y);
};

class CoronalWindow : public MriWindow {
 public:
  CoronalWindow(const int x, const int y, const int w, const int n,
                const char *const i, const Mri mri, Model &model);
};

class SagittalWindow : public MriWindow {
 public:
  SagittalWindow(const int x, const int y, const int w, const int n,
                 const char *const i, const Mri mri, Model &model);
};

class AxialWindow : public MriWindow {
 public:
  AxialWindow(const int x, const int y, const int w, const int n,
              const char *const i, const Mri mri, Model &model);
};
