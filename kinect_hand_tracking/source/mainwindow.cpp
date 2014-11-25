#include "mainwindow.h"

#include <windows.h>

#include <qtcore/qevent>

#include <qtwidgets/qframe>


// ...
MainWindow::MainWindow()
{
	// Set mainwindow icon and title.
	setWindowIcon( QIcon( "data/logo/qt_logo.png" ) );
	setWindowTitle(wndTitle);
	(void) statusBar();
	
	// Configure OpenGL widget.
	wdgOpenGL = new GLWidget(); // Constructor.
	wdgOpenGL->setFocusPolicy( Qt::ClickFocus ); // Focus policy.
	wdgOpenGL->setMinimumSize( wdgOpenGLSizeMin.width(), wdgOpenGLSizeMin.height() ); // Minimum size.
	wdgOpenGL->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding ); // Resize policy.
	wdgOpenGL->setFormat( QGLFormat( QGL::SampleBuffers ) ); // Antialiasing for big size text.
	// Create panels layouts actions and menus.
	createPanel();
	setLayouts();
	createActions();
	createMenu();
	// Configure central widget.
	wdgCentral = new QWidget;
	setCentralWidget(wdgCentral);
	wdgCentral->setLayout(loGridCentral);
	// Configure mainwindow position and size.
	move( wndPos.x(), wndPos.y() );
	resize( wndSize.width(), wndSize.height() );
	// Configure OpenGL widget timer.
	tmrOpenGL = new QTimer( (QObject*) wdgOpenGL );
	QTimer::connect( tmrOpenGL, SIGNAL( timeout() ), (QObject*) wdgOpenGL, SLOT( timerIdle() ) );
	tmrOpenGL->start(100);
	// Configure Mainwindow timer.
	//tmrMainwnd = new QTimer( (QObject*) this );
	//QTimer::connect( tmrMainwnd, SIGNAL( timeout() ), (QObject*) this, SLOT( timerIdle() ) );
	//tmrMainwnd->start(1);
	// ...
	showMaximized();
	//setWindowState( Qt::WindowMaximized );
	// ...
	tutor = new QTextBrowser();
	tutor->setFrameStyle( QFrame::Panel | QFrame::Sunken );
	tutor->setWindowTitle( wndTitle.append( " - Tutorial" ) );
	tutor->setSizePolicy( QSizePolicy::Fixed , QSizePolicy::Fixed );
	tutor->setFixedSize( 800, 600 );
	QStringList tu( QString( "data/tutorial" ) );
	tutor->setSearchPaths(tu);
	tutor->setSource( QUrl( "tutorial.html" ) );
}


// ...
MainWindow::~MainWindow() {}


// ...
void MainWindow::toggleDebug() { wdgOpenGL->toggleDebugInfo(); }


// ...
void MainWindow::exitApplication()
{
	wdgOpenGL->exitWidget();
	QApplication::exit(0);
}


// ...
void MainWindow::aboutApplication() { QMessageBox::about( this, wndTitle.append( " - About" ), QString( "Bla bla bla..." ) ); }


// ...
void MainWindow::createActions()
{
	// Action: exit.
	actExit = new QAction( "E&xit", this );
	actExit->setShortcut( tr( "Ctrl+X" ) );
	connect( actExit, SIGNAL( triggered() ), this, SLOT( exitApplication() ) );
	// Action: view debug info.
	actDebugView = new QAction( "&Debug Info", this );
	actDebugView->setShortcut( tr( "Ctrl+D" ) );
	connect( actDebugView, SIGNAL( triggered() ), this, SLOT( toggleDebug() ) );
	actDebugView->setDisabled(true);
	// Action: about.
	actAbout = new QAction( "&About", this );
	connect( actAbout, SIGNAL( triggered() ), this, SLOT( aboutApplication() ) );
	// Action: show user guide.
	actShowUserGuide = new QAction( "&Show User Guide", this );
	connect( actShowUserGuide, SIGNAL( triggered() ), this, SLOT( showUserGuide() ) );
}


// ...
void MainWindow::showUserGuide() { tutor->show(); }


// ...
void MainWindow::timerIdle() { repaint(); }


// ...
void MainWindow::createMenu()
{
	// File menu
	menuFile = menuBar()->addMenu( "&File" );
	//menuFile->addSeparator();
	menuFile->addAction(actExit);
	// View menu.
	menuView = menuBar()->addMenu( "&View" );
	menuView->addAction(actDebugView);
	// Help menu.
	menuHelp = menuBar()->addMenu( "&Help" );
	menuHelp->addAction(actAbout);
	menuHelp->addAction(actShowUserGuide);
}


// ...
void MainWindow::createPanel() {}


// ...
void MainWindow::showPanel() {}

// ...
void MainWindow::hidePanel() {}


// ...
void MainWindow::setLayouts()
{
	// Create a simple line-separator.
	QFrame* frm = new QFrame();
	frm->setFrameShape( QFrame::HLine );
	// Side panel layout.
	loGridPanel = new QGridLayout();
	loGridPanel->setContentsMargins( 0, 0, 0, 0 );
	loGridPanel->setSizeConstraint( QLayout::SetMinimumSize );
	// Central widget layout.
	loGridCentral = new QGridLayout();
	loGridCentral->setContentsMargins( 5, 5, 5, 5 );
	loGridCentral->addWidget( wdgOpenGL, 0, 0 );
	loGridCentral->addLayout( loGridPanel, 0, 1 );
}


// ...
void MainWindow::paintEvent( QPaintEvent* e ) {}


// ...
void MainWindow::resizeEvent( QResizeEvent* e ) {}


// ...
void MainWindow::moveEvent( QMoveEvent* e )
{
	wndPos.rx() = e->pos().x();
	wndPos.ry() = e->pos().y();
}


// ...
void MainWindow::closeEvent( QCloseEvent* e ) { exitApplication(); }


