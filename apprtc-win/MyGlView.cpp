#include "MyGlView.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <QMutexLocker>

MyGlView::MyGlView(QWidget *parent) :
    QOpenGLWidget(parent)
{
    _orgWidth = -1;
    _orgHeight = -1;
}

MyGlView::~MyGlView()
{
    makeCurrent();
}

void MyGlView::initializeGL()
{
    makeCurrent();
    initializeOpenGLFunctions();
    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
}

void MyGlView::resizeGL(int width, int height)
{
    makeCurrent();
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, -height, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    calcRenderParam();
    updateScene();
}

void MyGlView::updateScene()
{
    if (this->isVisible()) update();
}

void MyGlView::paintGL()
{
    QMutexLocker locker(&_drawMutex);
    makeCurrent();

    glClear(GL_COLOR_BUFFER_BIT);
    if (!_orgImage.empty())
    {
        cv::Mat renderImage = getRenderImage();
        glLoadIdentity();
        glPushMatrix();
        
        glRasterPos2i(_renderX, _renderY);
        glPixelZoom(1, -1);
        glBindBuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
        //glDrawPixels(renderImage.cols, renderImage.rows, GL_RGBA, GL_UNSIGNED_BYTE, renderImage.data);
        glDrawPixels(renderImage.cols, renderImage.rows, GL_RGBA, GL_UNSIGNED_BYTE, renderImage.data);
        
        glPopMatrix();
        glFlush();
    }
}

cv::Mat MyGlView::getRenderImage()
{
    cv::Mat renderImage;
    cv::resize(_orgImage, renderImage, cv::Size(_renderWidth, _renderHeight));
    return renderImage;
}

void MyGlView::calcRenderParam()
{
    float ratio = 1.0;
    ratio = (float)_orgWidth /(float)_orgHeight;

    _renderWidth = this->size().width();
    _renderHeight = floor(_renderWidth / ratio);

    if (_renderHeight > this->size().height())
    {
        _renderHeight = this->size().height();
        _renderWidth = floor(_renderHeight * ratio);
    }

    _renderScale = (double)_renderWidth / _orgImage.cols;
    _renderX = floor((this->size().width() - _renderWidth) / 2);
    _renderY = -floor((this->size().height() - _renderHeight) / 2);
}

void MyGlView::setImage(const cv::Mat& image)
{
    QMutexLocker locker(&_drawMutex);
    _orgImage = image;
    if (image.empty())
    {
        //_orgImage = image;
        _orgWidth = -1;
        _orgHeight = -1;
        updateScene();
        return;
    }
    
    if (_orgWidth != _orgImage.cols || _orgHeight != _orgImage.rows)
    {
        _orgWidth = _orgImage.cols;
        _orgHeight = _orgImage.rows;
        calcRenderParam();
    }
    updateScene();
}

void MyGlView::onVideoFrame(void* buffer, int width, int height, int type)
{
    cv::Mat rgbaImg;
    if (NULL != buffer)
    {
        //cv::Mat img(height + height / 2, width, CV_8UC1, (unsigned char*)buffer);
        //cvtColor(img, rgbaImg, cv::COLOR_YUV2RGBA_YV12);
        //cvtColor(img, rgbaImg, cv::COLOR_GRAY2RGBA);
        cv::Mat img(height, width, CV_8UC4, (unsigned char*)buffer);
        rgbaImg = img.clone();
        //cvtColor(img, rgbaImg, cv::COLOR_2RGBA);
    }
    //printf("MyGlView::onVideoFrame:%p \n", buffer);
    setImage(rgbaImg);
}