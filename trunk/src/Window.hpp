#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QtGui>

#include "Engine.hpp"
#include "MoneyWidget.hpp"
#include "AsteroidWidget.hpp"

class Window : public QMainWindow
{
		Q_OBJECT

	public:
		static Window* getInstance();
		static void kill();
		QTreeView* getTreeView();
		QStandardItemModel* getStandardModel();

	protected:
		void closeEvent(QCloseEvent* event);

	private slots:
		void open();
		void save();
		void saveAs();
		void about();
		void find();
		void findNext();
		void selectionChangedSlot();
		void quit();
		void openRecentFile();

	private:
		Window();
		~Window();
		void loadFile(const QString& fileName);
		void saveFile(const QString& fileName);
		void setCurrentFile(const QString& fileName);
		void updateRecentFileActions();
		QString strippedName(const QString& fullFileName);

		static Window* window;
		Engine* engine;
		QTreeView* treeView;
		QStandardItemModel* standardModel;
		MoneyWidget* moneyWidget;
		AsteroidWidget* asteroidWidget;
		QList<QStandardItem*> itemsFound;
		int iif;
		QAction* saveAct;
		QAction* saveAsAct;
		QAction* separatorAct;
		enum { MaxRecentFiles = 5 };
		QAction* recentFileActs[MaxRecentFiles];
		QString curFile;
};

#endif //WINDOW_HPP
