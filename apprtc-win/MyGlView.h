#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_2_0>
#include <opencv2/opencv.hpp>
#include <QMutex>
#include <QMouseEvent>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class MyGlView : public QOpenGLWidget, protected QOpenGLFunctions_2_0
{
    Q_OBJECT
public:
    explicit MyGlView(QWidget *parent = 0);
    ~MyGlView();
    void setImage(const cv::Mat& image);
public slots:
    void onVideoFrame(void* buffer, int width, int height, int type);
protected:
    void initializeGL();                    // OpenGL initialization
    void paintGL();                         // OpenGL Rendering
    void resizeGL(int width, int height);   // Widget Resize Event

    void updateScene();

    void calcRenderParam();
    cv::Mat getRenderImage();

private:
    cv::Mat _orgImage; // original OpenCV image to be shown

    int _orgWidth;
    int _orgHeight;
    int _renderWidth;
    int _renderHeight;
    int _renderX;
    int _renderY;
    float _renderScale;
	QMutex _drawMutex;
};

