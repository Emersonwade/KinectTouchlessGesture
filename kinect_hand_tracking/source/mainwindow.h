


#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <qtwidgets/qaction>
#include <qtwidgets/qapplication>
#include <qtwidgets/qcheckbox>
#include <qtwidgets/qcombobox>
#include <qtwidgets/qgridlayout>
#include <qtwidgets/qgroupbox>
#include <qtwidgets/qhboxlayout>
#include <qtwidgets/qheaderview>
#include <qtwidgets/qlabel>
#include <qtwidgets/qlineedit>
#include <qtwidgets/qmainwindow>
#include <qtwidgets/qmenubar>
#include <qtwidgets/qpushbutton>
#include <qtwidgets/qradiobutton>
#include <qtwidgets/qsizepolicy>
#include <qtwidgets/qslider>
#include <qtwidgets/qstatusbar>
#include <qtwidgets/qtablewidget>
#include <qtwidgets/qtablewidgetitem>
#include <qtwidgets/qvboxlayout>

#include <qtcore/qstring>
#include <qtcore/qevent>

#include "glwidget.h"


class MainWindow : public QMainWindow 
{

	Q_OBJECT

public:

	// ...
    MainWindow();
	~MainWindow();

private slots:

	void exitApplication();
	void aboutApplication();
	void toggleDebug();
	void showUserGuide();
	void timerIdle();

signals:

private:

	QTimer*		tmrOpenGL;
	QTimer*		tmrMainwnd;
	
	// OpenGL widget.
	GLWidget*				wdgOpenGL;
	QSize					wdgOpenGLSizeMin = QSize( 800, 600 );
	QSize					wdgOpenGLSizeMax = QSize( 1600, 900 );
	
	// Widgets.
    QWidget*				wdgCentral;				// Central widget.

	// Menus.
    QMenu*					menuFile;
	QMenu*					menuView;
    QMenu*					menuHelp;
	QTextBrowser*			tutor;
	// GUI menu actions.
    QAction*				actExit;
	QAction*				actDebugView;
    QAction*				actAbout;
	QAction*				actShowUserGuide;
	// Layouts
	QGridLayout*			loGridCentral;			// Central widget layout.
	int						loGridPanelHSizeFixed = 300;
	QGridLayout*			loGridPanel;			// Side panel layout.

	// ...
	QPoint					wndPos = QPoint( 20, 20);				// Mainwindow position.
	QSize					wndSize = QSize( 1280, 800 );			// Mainwindow size.
	int						wndPanelWidth = 400;					// Mainwindow side panel width.
	QString					wndTitle = QString( "Fitts' Task 2D" );	// Mainwindow title.

	// Misc GUI functions.
	void createActions();
	void createMenu();
	void createPanel();
	void showPanel();
	void hidePanel();
	void setLayouts();

protected:

	void paintEvent( QPaintEvent* e );
	void resizeEvent( QResizeEvent* e );
	void moveEvent( QMoveEvent* e );
	void closeEvent( QCloseEvent* e );
	
};


#endif


