


#include <qtcore/qevent>
#include <qtcore/qbytearray>
#include <qtcore/qstringlist>
#include <qtcore/qfile>
#include <qtcore/qxmlstreamreader>
#include <qtcore/qtextstream>

#include <qtgui/qfontdatabase>
#include <qtgui/qmouseevent>
#include <qtgui/qcursor>

#include <set>
#include <ctime>
#include <cassert>
#include <cmath>

#include "glwidget.h"


// ...
void GLWidget::initFonts()
{
	//int fontId = QFontDatabase::addApplicationFont( "data/font/open_sans_condensed_light.ttf" );
	// ...
	int fontDebugId = QFontDatabase::addApplicationFont( "data/font/anonymous_pro.ttf" );
	QString fontDebugFamily = QFontDatabase::applicationFontFamilies(fontDebugId).at(0);
	fontDebug = QFont( fontDebugFamily, fontDebugSize, QFont::Normal );
	// ...
	//int fontMessageId = QFontDatabase::addApplicationFont( "data/font/adobe_myriad_pro_condensed.otf" );
	int fontMessageId = QFontDatabase::addApplicationFont( "data/font/roboto_thin.ttf" );
	QString fontMessageFamily = QFontDatabase::applicationFontFamilies(fontMessageId).at(0);
	fontMessage = QFont( fontMessageFamily, fontMessageSize, QFont::Normal );
}


// Draw 3D axis.
void GLWidget::drawAxis( float l, float w, bool negative )
{
	// Saving OpenGL attributes: color.
	glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT ); // See: http://www.opengl.org/sdk/docs/man2/xhtml/glPushAttrib.xml
		// Set line width.
		glLineWidth(w);
		// Draw axis using the proper color.
		glBegin(GL_LINES);
			glColor3f( 1.0f, 0.0f, 0.0f );
			glVertex3f( 0.0f, 0.0f, 0.0f );
			glVertex3f( l, 0.0f, 0.0f );
			glColor3f( 0.0f, 1.0f, 0.0f );
			glVertex3f( 0.0f, 0.0f, 0.0f );
			glVertex3f( 0.0f, l, 0.0f );
			glColor3f( 0.0f, 0.0f, 1.0f );
			glVertex3f( 0.0f, 0.0f, 0.0f );
			glVertex3f( 0.0f, 0.0f, l );
		glEnd();
		if(negative) {
			// Draw negative axis using the proper color.
			glBegin(GL_LINES);
				glColor3f( 0.5f, 0.0f, 0.0f );
				glVertex3f( 0.0f, 0.0f, 0.0f );
				glVertex3f( -l, 0.0f, 0.0f );
				glColor3f( 0.0f, 0.5f, 0.0f );
				glVertex3f( 0.0f, 0.0f, 0.0f );
				glVertex3f( 0.0f, -l, 0.0f );
				glColor3f( 0.0f, 0.0f, 0.5f );
				glVertex3f( 0.0f, 0.0f, 0.0f );
				glVertex3f( 0.0f, 0.0f, -l );
			glEnd(); }
	// Restore OpenGL attributes previously saved with the last glPushAttrib command.
	glPopAttrib();
}


// ...
void GLWidget::drawGridXZ( float l, float w )
{
	// Saving OpenGL attributes: color.
	glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT ); // See: http://www.opengl.org/sdk/docs/man2/xhtml/glPushAttrib.xml
		// Set line width.
		glLineWidth(w);
		// Draw XZ grid using the proper color.
		glBegin(GL_LINES);
			// Draw Y lines.
			glColor3f( 0.0f, 0.8f, 0.0f );
			glVertex3i( 0, 0, 0 );
			glVertex3i( 0, l/2, 0 );
			// Draw Z lines.
			for( int i = -l/2; i <= l/2; i++ ) {
				if( i == 0 ) {
					glColor3f( 0.5f, 0.5f, 0.5f );
					glVertex3i( i, 0, -l/2 );
					glVertex3i( i, 0, 0 );
					glColor3f( 0.0f, 0.0f, 0.8f );
					glVertex3i( i, 0, 0 );
					glVertex3i( i, 0, l/2 );
				}
				else {
					glColor3f( 0.5f, 0.5f, 0.5f );
					glVertex3i( i, 0, -l/2 );
					glVertex3i( i, 0, l/2 ); } }
			// Draw X lines.
			for( int j = -l/2; j <= l/2; j++ ) {
				if( j == 0 ) { 
					glColor3f( 0.5f, 0.5f, 0.5f ); 
					glVertex3i( -l/2, 0, j );
					glVertex3i( 0, 0, j );
					glColor3f( 0.8f, 0.0f, 0.0f );
					glVertex3i( 0, 0, j );
					glVertex3i( l/2, 0, j ); }
				else { 
					glColor3f( 0.5f, 0.5f, 0.5f );
					glVertex3i( -l/2, 0, j );
					glVertex3i( l/2, 0, j ); } }
		glEnd();
	// Restore OpenGL attributes previously saved with the last glPushAttrib command.
	glPopAttrib();
}


// Display text on screen.
void GLWidget::drawText()
{
	qglColor(fontDebugColor);
	// Visualization Fps info.
	renderText( width() - 120, 30, QString( "VIS" ), fontDebug );
	renderText( width() - 90, 30, QString( "%1" ).arg( visFps.getFps(), 0, 'f', 1 ), fontDebug );
	renderText( width() - 50, 30, QString( "Fps" ), fontDebug );
}


// ...
void GLWidget::grabWidgetScreenshot()
{
	if(grabScreenshot) {
		QImage img = grabFrameBuffer(true);
		if( !( img.isNull() ) ) { bool res = img.save( "data/screenshot.bmp", "BMP", -1 ); }
		grabScreenshot = false; }
}


// ...
GLWidget::GLWidget( QWidget* parent ) : QGLWidget( QGLFormat( QGL::DoubleBuffer | QGL::Rgba | QGL::DepthBuffer ), parent )
{
	// Makes this widget the current widget for OpenGL operations (e.g. OpenGL rendering context = widget rendering context).
	makeCurrent();
	// Init custom fonts.
	initFonts();
}


// ...
GLWidget::~GLWidget() {}


// ...
void GLWidget::exitWidget() {}


// ...
void GLWidget::toggleDebugInfo() { dbgFlag = !dbgFlag; }


// ...
void GLWidget::timerIdle()
{
	// Force repainting.
	repaint();
}


// OpenGL initialization.
void GLWidget::initializeGL()
{
	glewInit(); // glewInit() needs an OpenGL context!
	// Shading model settings.
	glShadeModel(GL_FLAT);
	// Blending settings.
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	// Depth test settings.
	glEnable(GL_DEPTH_TEST);
	// Line settings.
	glLineWidth(1.0f);
	//glEnable( GL_LINE_SMOOTH );
	//glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	// Point settings.
	glPointSize(1.0f);
	glEnable(GL_POINT_SMOOTH);
	glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
	// Polygon settings.
	glEnable(GL_POLYGON_SMOOTH);
	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST ); 
}


// ...
void GLWidget::updateGL() { repaint(); }


// ...
void GLWidget::paintGL()
{
	// Update FPS counter.
	visFps.add( clock() );
	
	// GLWidget mode == GLWidgetMode::NDI_3D.
	// Clearing buffers.
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	// VIEWPORT 1.
	glViewport( 0, 0, (GLsizei) width(), (GLsizei) height() );
	// Projection for viewport 1.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 60.0, (GLfloat) width() / (GLfloat) height(), 0.1, 100.0 );
	// Modelview for viewport 1.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt( 0.5, 0.5, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
	// User translation.
	// ...
	// User rotation.
	// ...
	// User scaling.
	// ...
	// ...
	glPushMatrix();
		// Toggle reference system.
		glDisable(GL_BLEND);
		drawAxis( 2.0f, 5.0f, true );
		drawGridXZ( 10.0f, 1.0f );
		// Configure marker points rendering: blending, point size, color.
		glEnable(GL_BLEND);
	glPopMatrix();
	// VIEWPORT 2.
	glViewport( 0, 0, 100, 100 );
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 60.0f, 1.0, 1.0f, 5.0f );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef( 0.0f, 0.0f, -2.0f );
	// Replicate user rotation.
	// ...
	// ...
	// Reference system icon.
	drawAxis( 1.0f, 2.0f, false );
	
	// Draw text info on the screen.
	drawText();
	// Double buffering active by default!
	// If the grab screenshot flag is on acquire and save a screenshot.
	grabWidgetScreenshot();
}


// ...
void GLWidget::resizeGL( int w, int h )
{
	wndAR = float(w) / float(h);
	repaint();
}


// ...
void GLWidget::mousePressEvent( QMouseEvent* e )
{
	// Repaints the widget immediately.
	repaint();
}


// ...
void GLWidget::mouseReleaseEvent( QMouseEvent* e )
{
	// Repaints the widget immediately.
	repaint();
}


// ...
void GLWidget::mouseMoveEvent( QMouseEvent* e )
{ 
	// Repaints the widget immediately.
	repaint();
}


// ...
void GLWidget::wheelEvent( QWheelEvent* e ) { repaint(); }


// ...
void GLWidget::keyPressEvent( QKeyEvent* e )
{
	// ...
	repaint();
}


// ...
void GLWidget::keyReleaseEvent( QKeyEvent* e ) {}


// ....
void GLWidget::enterEvent( QEvent* event ) {}


// ...
void GLWidget::leaveEvent( QEvent* e ) {}


