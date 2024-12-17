#include <QApplication>
#include <QWebEngineView>
#include <QCommandLineParser>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QIcon>
#include "Page.h"

int main(int argc, char **argv) {
	QApplication app(argc, argv);
	app.setApplicationVersion("1.2.0");

	QCommandLineParser cl;
	cl.setApplicationDescription(QCoreApplication::translate("main", "OpenBosniana OS Welcome Runner"));
	cl.addHelpOption();
	cl.addVersionOption();
	cl.addPositionalArgument("startpage", QCoreApplication::translate("main", "Start page"));
	QCommandLineOption size("s", QCoreApplication::translate("size", "Initial size"), "size", "870x580");
	QCommandLineOption icon("i", QCoreApplication::translate("icon", "Application icon"), "icon", "/usr/share/icons/openbosniana.svg");
	QCommandLineOption c("c", QCoreApplication::translate("main", "Ignored, for compatibility with old version"));
	QCommandLineOption title("t", QCoreApplication::translate("main", "Application title"), "title", "OpenBosniana OS Welcome Runner");
	cl.addOptions(QList<QCommandLineOption>() << size << icon << c << title);
	cl.process(app);

	QString t = cl.value(title);
	if (!t.isEmpty())
		app.setApplicationName(t);

	unsigned int w = 870, h = 580; // Default size values
	QString s = cl.value(size);
	if (s.contains('x')) {
		w = s.section('x', 0, 0).toUInt();
		h = s.section('x', 1, 1).toUInt();
	}

	QString arg = cl.positionalArguments().empty() ? QDir::currentPath() : cl.positionalArguments().at(0), dir, startpage;

	QFileInfo fi(arg);
	if (fi.isDir()) {
		dir = arg;
		if (QFile::exists(dir + "/index.sh.html"))
			startpage = dir + "/index.sh.html";
		else
			startpage = dir + "/index.sh.htm";
	} else {
		dir = fi.absolutePath();
		startpage = fi.fileName();
	}

	QWebEngineView *l = new QWebEngineView(static_cast<QWidget*>(nullptr));
	l->setWindowIcon(QIcon(cl.value(icon)));
	Page *p = new Page(dir, l);
	l->setPage(p);
	p->open(startpage);
	l->resize(w, h);
	l->setMinimumSize(870, 580);
	l->show();
	app.exec();
	delete l;
}
