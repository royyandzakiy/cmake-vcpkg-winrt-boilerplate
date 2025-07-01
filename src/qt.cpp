#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPalette>

int run_qt(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QWidget window;
	window.setWindowTitle("Simple Qt Window");
	window.resize(300, 200);

	// Set background to a nice red
	QPalette palette = window.palette();
	palette.setColor(QPalette::Window, QColor(220, 20, 60)); // Crimson red
	window.setAutoFillBackground(true);
	window.setPalette(palette);

	// Add label
	QLabel *label = new QLabel("Close me to continue the loop");
	label->setAlignment(Qt::AlignCenter);
	label->setStyleSheet("color: white; font-size: 16px; font-weight: bold;");

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(label);
	window.setLayout(layout);

	window.show();

	return app.exec();
}
