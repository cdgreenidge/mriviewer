#include <FL/Fl.H>
#include <FL/gl.H>
#include <FL/Fl_Gl_Window.H>
#include <boost/multi_array.hpp>
#include <iostream>
#include <limits>
#include <string>
#include "image.h"
#include "mri.h"
#include "model.h"
#include "utils.h"
#include "windows.h"
#include "main.h"

MriWindow::MriWindow(const int x, const int y, const int w, const int h,
                     const char *const i, const Mri mri, Model &model,
                     const Plane plane, const Labels labels)
    : Fl_Gl_Window(x, y, w, h, i),
      mri_(mri),
      model_(model),
      plane_(plane),
      image_(mri.width(plane), mri.height(plane)),
      labels_(labels),
      maxLength_(0.8f),
      bottom_(0.0f),
      left_(0.0f) {
  mri.fillImage(image_, model.slice(plane), model.t());
}

CoronalWindow::CoronalWindow(const int x, const int y, const int w, const int h,
                             const char *const i, const Mri mri, Model &model)
    : MriWindow(x, y, w, h, i, mri, model, CORONAL,
                Labels{"L", "S", "R", "I"}) {}

SagittalWindow::SagittalWindow(const int x, const int y, const int w,
                               const int h, const char *const i, const Mri mri,
                               Model &model)
    : MriWindow(x, y, w, h, i, mri, model, SAGITTAL,
                Labels{"A", "S", "P", "I"}) {}

AxialWindow::AxialWindow(const int x, const int y, const int w, const int h,
                         const char *const i, const Mri mri, Model &model)
    : MriWindow(x, y, w, h, i, mri, model, AXIAL, Labels{"L", "A", "R", "P"}) {}

int MriWindow::handle(int event) {
  float window_x;
  float window_y;
  float texture_x;
  float texture_y;
  float normalized_x;
  float normalized_y;
  size_t voxel_x;
  size_t voxel_y;
  Fl_Widget *children;

  switch (event) {
    case FL_DRAG:
    /* Fallthrough */
    case FL_PUSH:
      // Convert event coordinates to voxel numbers
      window_x = (2.0f * Fl::event_x() / w()) - 1;
      window_y = (2.0f * Fl::event_y() / h()) - 1;
      texture_x = window_x / getMriWidthCoord();  // between -1 and 1
      texture_y = window_y / getMriHeightCoord();
      normalized_x = clamp((texture_x + 1) / 2, 0, 1);
      normalized_y = 1.0f - clamp((texture_y + 1) / 2, 0, 1);
      // Note: the nextafterf calls make sure that we are in open intervals
      // [0, mri.height()) and [0, mri.width()) to avoid off-by-one errors
      voxel_x = static_cast<size_t>((left_ + model_.scale() * normalized_x) *
                                    nextafterf(mri_.width(plane_), 0));
      voxel_y = static_cast<size_t>((bottom_ + model_.scale() * normalized_y) *
                                    nextafterf(mri_.height(plane_), 0));
       model_.updatePush(voxel_x, voxel_y, plane_);
      break;
    default:
      return Fl_Gl_Window::handle(event);
  }

  // The MRI windows are the first 3 widgets in the top window. Redraw
  // them all.
  static_cast<MainWindow *>(window())->redrawMri();
  return 1;
}

void MriWindow::draw() {
  /* Initialize resources */
  GLint oldShader;
  glGetIntegerv(GL_CURRENT_PROGRAM, &oldShader);

  if (!context_valid()) {
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

    resources_.vertexShader = makeShader(GL_VERTEX_SHADER, "vertexShader.glsl");
    resources_.fragmentShader =
        makeShader(GL_FRAGMENT_SHADER, "fragmentShader.glsl");
    resources_.program =
        makeProgram(resources_.vertexShader, resources_.fragmentShader);
    resources_.attributes.position = static_cast<GLuint>(
        glGetAttribLocation(resources_.program, "position"));
    resources_.attributes.texcoord = static_cast<GLuint>(
        glGetAttribLocation(resources_.program, "texcoord"));
    resources_.uniforms.texture = static_cast<GLuint>(
        glGetUniformLocation(resources_.program, "texture"));
    resources_.uniforms.overlayColor =
        glGetUniformLocation(resources_.program, "overlayColor");
    resources_.uniforms.overlayAlpha =
        glGetUniformLocation(resources_.program, "overlayAlpha");
    resources_.texture = makeTexture(image_.width(), image_.height());
  }

  if (!valid()) {
    glViewport(0, 0, w(), h());
  }

  /* Load image data into texture */
  mri_.fillImage(image_, model_.slice(plane_), model_.t());
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, static_cast<GLsizei>(image_.width()),
                  static_cast<GLsizei>(image_.height()), GL_LUMINANCE, GL_FLOAT,
                  image_.data());

  /* Calculate texture coordinates */
  Crosshair crosshair = model_.crosshair(plane_);
  GLfloat dx = 1.0f / image_.width();
  GLfloat centerX = (crosshair.x + 0.5f) * dx;
  GLfloat dy = 1.0f / image_.height();
  GLfloat centerY = (crosshair.y + 0.5f) * dy;
  GLfloat right = centerX + model_.scale() / 2.0f;
  GLfloat top = centerY + model_.scale() / 2.0f;
  GLfloat left = centerX - model_.scale() / 2.0f;
  GLfloat bottom = centerY - model_.scale() / 2.0f;

  /* Correct for the box being out of bounds */
  GLfloat delta = 0.0f;
  if ((delta = right - 1.0f) > 0) {
    right -= delta;
    left -= delta;
  } else if ((delta = -left) > 0) {
    left += delta;
    right += delta;
  }

  if ((delta = top - 1.0f) > 0) {
    top -= delta;
    bottom -= delta;
  } else if ((delta = -bottom) > 0) {
    bottom += delta;
    top += delta;
  }

  // Save the MRI texture coords so callbacks can access them
  bottom_ = bottom;
  left_ = left;

  GLfloat width = getMriWidthCoord();
  GLfloat height = getMriHeightCoord();
  GLfloat crosshairX = getCrosshairXCoord(left);
  GLfloat crosshairY = getCrosshairYCoord(bottom);

  GLfloat vertexBufferData[] = {
      /* MRI Quad */
      /* Positions  Texture coordinates */
      -width, -height, left, bottom, /* Bottom left */
      width, -height, right, bottom, /* Bottom right */
      -width, height, left, top,     /* Top left */
      width, height, right, top,     /* Top right */
      /* Horizontal crosshair line, with 8 bytes of padding */
      -width, crosshairY, 0.0f, 0.0f, width, crosshairY, 0.0f, 0.0f,
      /* Vertical crosshair line, with 8 bytes of padding */
      crosshairX, -height, 0.0f, 0.0f, crosshairX, height, 0.0f, 0.0f};

  glDeleteBuffers(1, &resources_.vertexBuffer);
  resources_.vertexBuffer =
      makeBuffer(GL_ARRAY_BUFFER, vertexBufferData, sizeof(vertexBufferData));
  glDeleteBuffers(1, &resources_.elementBuffer);
  static const GLushort elementBufferData[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
  resources_.elementBuffer =
      makeBuffer(GL_ARRAY_BUFFER, elementBufferData, sizeof(elementBufferData));

  /* RENDER */
  glUseProgram(resources_.program);
  glBindBuffer(GL_ARRAY_BUFFER, resources_.vertexBuffer);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  /* Render MRI */
  glVertexAttribPointer(resources_.attributes.position, /* Attribute */
                        2,                              /* Size */
                        GL_FLOAT,                       /* Type */
                        GL_FALSE,                       /* Normalized? */
                        sizeof(GLfloat) * 4,            /* Stride */
                        static_cast<void *>(0));        /* Offset */
  glEnableVertexAttribArray(resources_.attributes.position);
  glVertexAttribPointer(resources_.attributes.texcoord, /* Attribute */
                        2,                              /* Size */
                        GL_FLOAT,                       /* Type */
                        GL_FALSE,                       /* Normalized? */
                        sizeof(GLfloat) * 4,            /* Stride */
                                                        /* Offset */
                        reinterpret_cast<void *>(2 * sizeof(GLfloat)));
  glEnableVertexAttribArray(resources_.attributes.texcoord);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resources_.elementBuffer);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, resources_.texture);
  glUniform1i(static_cast<GLint>(resources_.uniforms.texture), 0);
  glUniform4f(resources_.uniforms.overlayColor, 0, 0, 0, 0);
  glUniform1f(resources_.uniforms.overlayAlpha, 0.0f);
  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT,
                 static_cast<void *>(0));

  /* Rennder crosshairs */
  glUniform4f(resources_.uniforms.overlayColor, 1, 0, 0, 1);
  glUniform1f(resources_.uniforms.overlayAlpha, 1.0f);
  glDrawElements(GL_LINES, 4, GL_UNSIGNED_SHORT,
                 reinterpret_cast<void *>(sizeof(GLushort) * 4));
  glDisableVertexAttribArray(resources_.attributes.position);

  /* Draw axis labels */
  glUseProgram(static_cast<GLuint>(oldShader));
  float LABEL_PADDING = 0.90f;
  gl_draw_centered(labels_.right, LABEL_PADDING * 1.0f, 0.0f);
  gl_draw_centered(labels_.top, 0.0f, LABEL_PADDING * 1.0f);
  gl_draw_centered(labels_.left, LABEL_PADDING * -1.0f, 0.0f);
  gl_draw_centered(labels_.bottom, 0.0f, LABEL_PADDING * -1.0f);

  return;
}

GLuint MriWindow::makeShader(GLenum type, const char *filename) {
  std::string str = readBinFile(filename);
  GLuint shader = glCreateShader(type);
  const char *source = str.data();
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::" << filename << "::COMPILATION_FAILED"
              << infoLog << std::endl;
  }
  return shader;
}

GLuint MriWindow::makeProgram(GLuint vertexShader, GLuint fragmentShader) {
  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  GLint success;
  GLchar infoLog[512];
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    std::cout << "ERROR::PROGRAM::LINK_FAILED" << infoLog << std::endl;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return program;
}

GLuint MriWindow::makeTexture(size_t bufWidth, size_t bufHeight) {
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, static_cast<GLsizei>(bufWidth),
               static_cast<GLsizei>(bufHeight), 0, GL_LUMINANCE, GL_FLOAT,
               NULL);
  return texture;
}

GLfloat MriWindow::getMriWidthCoord() {
  GLfloat width = maxLength_;

  /* Correct for a non-square texture */
  if (image_.height() >= image_.width()) {
    width *= image_.width() / static_cast<GLfloat>(image_.height());
  }

  /* Correct for a non-square window */
  GLfloat widthHeightRatio = w() / static_cast<GLfloat>(h());
  GLfloat heightWidthRatio = h() / static_cast<GLfloat>(w());
  if (widthHeightRatio > heightWidthRatio) {
    width *= heightWidthRatio;
  }
  return width;
}

GLfloat MriWindow::getMriHeightCoord() {
  GLfloat height = maxLength_;

  /* Correct for a non-square texture */
  if (image_.width() > image_.height()) {
    height *= image_.height() / static_cast<GLfloat>(image_.width());
  }

  /* Correct for a non-square window */
  GLfloat widthHeightRatio = w() / static_cast<GLfloat>(h());
  GLfloat heightWidthRatio = h() / static_cast<GLfloat>(w());
  if (widthHeightRatio <= heightWidthRatio) {
    height *= widthHeightRatio;
  }

  return height;
}

GLfloat MriWindow::getCrosshairXCoord(GLfloat left) {
  float widthCoord = getMriWidthCoord();
  float start = -widthCoord;
  float dx = 2 * widthCoord / (image_.width() * model_.scale());
  Crosshair crosshair = model_.crosshair(plane_);
  return start + (crosshair.x - (left * image_.width()) + 0.5f) * dx;
}

GLfloat MriWindow::getCrosshairYCoord(GLfloat bottom) {
  float heightCoord = getMriHeightCoord();
  float start = -heightCoord;
  float dy = 2 * heightCoord / (image_.height() * model_.scale());
  Crosshair crosshair = model_.crosshair(plane_);
  return start + (crosshair.y - (bottom * image_.height()) + 0.5f) * dy;
}

GLuint MriWindow::makeBuffer(GLenum target, const void *bufferData,
                             GLsizei bufferSize) {
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(target, buffer);
  glBufferData(target, bufferSize, bufferData, GL_STATIC_DRAW);
  return buffer;
}

void MriWindow::gl_draw_centered(const char *string, float x, float y) {
  int pixelWidth;
  int pixelHeight;
  gl_measure(string, pixelWidth, pixelHeight);

  float width = pixelWidth / (w() / 2.0f);
  float height = pixelHeight / (h() / 2.0f);

  /* I haven't the foggiest idea why the height needs to be divided by
   * four instead of two. */
  gl_draw(string, x - width / 2.0f, y - height / 4.0f);
  return;
}
