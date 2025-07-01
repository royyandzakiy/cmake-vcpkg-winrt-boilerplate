#include "../include/qt.h"

int run_qt(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QWidget window;
	window.setWindowTitle("Simple Qt Window");
	window.resize(300, 200);
	window.show();

	return app.exec();
}
