#include "Common.h"
#include "TriangleManager.h"
#include "GLUtil.h"
#include "shader/camera.frag"
#include "shader/camera.vert"
/* GL_OES_EGL_image_external */
#ifndef GL_OES_EGL_image_external
#define GL_TEXTURE_EXTERNAL_OES                                 0x8D65
#endif
template<> TriangleManager* Singleton<TriangleManager>::msSingleton = nullptr;
TriangleManager::TriangleManager()
    : _widgetWidth(0)
    , _widgetHeight(0)
    , _sProgramPlay(0)
    , _vaoId(0)
    , _vboBuffer(0)
    , _positionLoc(-1)
    , _colorLoc(-1)
    , _textureUVLoc(-1)
    , _mvpMatrixLoc(-1)
    , _mvpMatrix(glm::mat4(1.0f))
    , _modeMatrix(glm::mat4(1.0f))
    , _viewMatrix(glm::mat4(1.0f))
    , _perspectiveM(glm::mat4(1.0f))
{
}

TriangleManager::~TriangleManager() {
}

void TriangleManager::onCreate() {
}

void TriangleManager::onResume() {
}

void TriangleManager::onPause() {
}

void TriangleManager::onStop() {
}

void TriangleManager::onDestroy() {
    glDeleteVertexArrays(1, &_vaoId);
    glDeleteBuffers(1, &_vboBuffer);
    glDeleteProgram(_sProgramPlay);
}

void TriangleManager::initGL(int widgetWidth, int widgetHeight) {
    _widgetWidth    = widgetWidth;
    _widgetHeight   = widgetHeight;
    if (CompileShaderProgram(camera_play_vert, camera_play_frag, &_sProgramPlay)) {
        _positionLoc	    = glGetAttribLocation(_sProgramPlay,    "a_Position");
        _colorLoc		    = glGetAttribLocation(_sProgramPlay,    "a_Color");
        _mvpMatrixLoc       = glGetUniformLocation(_sProgramPlay,   "u_MvpMatrix");
        _textureUVLoc       = glGetAttribLocation(_sProgramPlay,    "v_TextureUV");
        // 指定顶点属性数据 顶点位置/颜色/纹理坐标
        GLfloat vertices[]  = {
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// A
             0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// B
             0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,	// C
             0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,	// C
            -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,	// D
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// A

            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// E
            -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // H
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	// G
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	// G
             0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// F
            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// E

            -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,	// D
            -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // H
            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,	// E
            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,	// E
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// A
            -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,	// D

             0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// F
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	// G
             0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,  // C
             0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,  // C
             0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,	// B
             0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// F

             0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	// G
            -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // H
            -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,	// D
            -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,	// D
             0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,	// C
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	// G

            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// A
            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,	// E
             0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,	// F
             0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,	// F
             0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// B
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// A
        };
        glGenVertexArrays(1, &_vaoId);
        glBindVertexArray(_vaoId);
        glGenBuffers(1, &_vboBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _vboBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(
            _positionLoc,
            3,
            GL_FLOAT,
            GL_FALSE,
            8 * sizeof(GL_FLOAT),
            (GLvoid*)0
        );
        glEnableVertexAttribArray(_positionLoc);
        glVertexAttribPointer(
            _colorLoc,
            3,
            GL_FLOAT,
            GL_FALSE,
            8 * sizeof(GL_FLOAT),
            (GLvoid*)(3 * sizeof(GL_FLOAT))
        );
        glEnableVertexAttribArray(_colorLoc);
        glVertexAttribPointer(
            _textureUVLoc,
            2,
            GL_FLOAT,
            GL_FALSE,
            8 * sizeof(GL_FLOAT),
            (GLvoid*)(6 * sizeof(GL_FLOAT))
        );
        glEnableVertexAttribArray(_textureUVLoc);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    } else {
        LOGE("CompileShaderProgram===================");
    }
    glFrontFace(GL_CCW);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void TriangleManager::drawFrame() {
    glViewport(0, 0, _widgetWidth, _widgetHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(_vaoId);
    glUseProgram(_sProgramPlay);
    glUniformMatrix4fv(_mvpMatrixLoc, 1, GL_FALSE, glm::value_ptr(_mvpMatrix));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glUseProgram(0);
}

void TriangleManager::onChange(int widgetWidth, int widgetHeight) {
    _widgetWidth    = widgetWidth;
    _widgetHeight   = widgetHeight;
    _modeMatrix	    = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    _viewMatrix	    = glm::lookAt(glm::vec3(0, 3, 5), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
    _perspectiveM   = glm::perspective(glm::radians(45.0f), static_cast<float>(_widgetWidth) / static_cast<float>(_widgetHeight), 0.1f, 100.0f);
    _mvpMatrix		= _perspectiveM * _viewMatrix * _modeMatrix;
}

GLint TriangleManager::getCameraTextureId() {
    return 0;
}