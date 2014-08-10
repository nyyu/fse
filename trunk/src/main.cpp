#include "Window.hpp"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	app.setOrganizationName("nyyu");
	app.setApplicationName("fse");

	QTranslator qtTranslator;
	qtTranslator.load("qt_" + QLocale::system().name(), ":/translations/");
	app.installTranslator(&qtTranslator);

	QTranslator fseTranslator;
	fseTranslator.load("fse_" + QLocale::system().name(), ":/translations/");
	app.installTranslator(&fseTranslator);

	Window::getInstance()->show();

	return app.exec();
}
