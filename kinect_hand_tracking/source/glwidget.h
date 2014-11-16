


#ifndef GLWIDGET_H
#define GLWIDGET_H


#include <glew.h>
#include <freeglut.h>

#include <qtcore/qstring>
#include <qtcore/qpoint>

#include <qtopengl/qtopengl>

#include <qtgui/qcolor>
#include <qtgui/qfont>

#include <string>
#include <vector>
#include <utility>

#include "fps.h"


class GLWidget : public QGLWidget
{

	Q_OBJECT

public:

private:

	// General widget info.
	FPS				visFps;
	bool			dbgFlag = false;
	QPoint			mouWinPos;
	QPoint			mouGLPos;
	float			wndAR;
	bool			flagGrabKeyboard = false;
	QCursor			cursorClick = QCursor(Qt::CrossCursor);
	bool			grabScreenshot = false;
	QSizeF			screenSizeInch;
	QSize			screenResPix;

	// Font info.
	QFont			fontDebug;
	int				fontDebugSize = 10;
	QColor			fontDebugColor = QColor( 150.0f, 200.0f, 250.0f, 255.0f );
	QFont			fontMessage;
	int				fontMessageSize = 12;
	QColor			fontMessageColor = QColor( 204.0f, 204.0f, 204.0f, 255.0f );

	void initFonts();
	void drawAxis( float l, float w, bool negative );
	void drawGridXZ( float l, float w );
	void drawText();
	void grabWidgetScreenshot();
	
public:

    GLWidget( QWidget* parent = 0 );
    ~GLWidget();
	void exitWidget();
	void toggleDebugInfo();

public slots:

	void timerIdle();

signals:

protected:

    void initializeGL();
	void updateGL();
    void paintGL();
    void resizeGL( int w, int h );
    void mousePressEvent( QMouseEvent* e );
	void mouseReleaseEvent( QMouseEvent* e );
    void mouseMoveEvent( QMouseEvent* e );
	void wheelEvent( QWheelEvent* e );
	void keyPressEvent( QKeyEvent* e );
	void keyReleaseEvent( QKeyEvent* e );
	void enterEvent( QEvent* e );
	void leaveEvent( QEvent* e );

};


#endif


