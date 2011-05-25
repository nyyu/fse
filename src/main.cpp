#include "Window.hpp"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setOrganizationName("nyyu");
	app.setApplicationName("fse");

	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

	QTranslator qtTranslator;
	qtTranslator.load("qt_" + QLocale::system().name(), ":/translations/");
	app.installTranslator(&qtTranslator);
	
	QTranslator fseTranslator;
	fseTranslator.load("fse_" + QLocale::system().name(), ":/translations/");
	app.installTranslator(&fseTranslator);

	Window::getInstance()->show();

	return app.exec();
}
