#include "Window.hpp"

Window* Window::window = NULL;

Window* Window::getInstance()
{
	if(NULL == window)
		window = new Window;
	return window;
}

void Window::kill()
{
	delete window;
}

Window::Window()
{
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle(tr("Fragile Save Editor"));

	(void)statusBar();
	
	QMenu* menuFile = menuBar()->addMenu(tr("&File"));
	menuFile->addAction(tr("&Open..."), this, SLOT(open()), QKeySequence::Open);
	saveAct = menuFile->addAction(tr("&Save"), this, SLOT(save()), QKeySequence::Save);
	saveAct->setDisabled(true);
	saveAsAct = menuFile->addAction(tr("Save &As..."), this, SLOT(saveAs()), QKeySequence("Ctrl+Maj+S"));
	saveAsAct->setDisabled(true);
	separatorAct = menuFile->addSeparator();
	for (int i = 0; i < MaxRecentFiles; ++i)
	{
		recentFileActs[i] = new QAction(this);
		recentFileActs[i]->setVisible(false);
		connect(recentFileActs[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
		menuFile->addAction(recentFileActs[i]);
	}
	updateRecentFileActions();
	menuFile->addSeparator();
	menuFile->addAction(tr("&Quit"), this, SLOT(quit()), QKeySequence("Ctrl+Q"));
	
	QMenu* menuEdit = menuBar()->addMenu(tr("&Edit"));
	menuEdit->addAction(tr("&Find..."), this, SLOT(find()), QKeySequence::Find);
	menuEdit->addAction(tr("Find &Next"), this, SLOT(findNext()), QKeySequence::FindNext);
	
	
	QMenu* menuHelp = menuBar()->addMenu(tr("&Help"));
	menuHelp->addAction(tr("&About"), this, SLOT(about()));
	menuHelp->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));
	
	standardModel = new QStandardItemModel;
	treeView = new QTreeView;
	treeView->setModel(standardModel);
	treeView->header()->hide();
	treeView->hide();
	connect(treeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(selectionChangedSlot()));
	
	moneyWidget = new MoneyWidget;
	
	asteroidWidget = new AsteroidWidget;
	
	QLayout* layout = new QHBoxLayout;
	layout->addWidget(treeView);
	layout->addWidget(moneyWidget);
	layout->addWidget(asteroidWidget);
	
	QWidget* zoneCentrale = new QWidget;
	zoneCentrale->setLayout(layout);
	setCentralWidget(zoneCentrale);
	resize(450, 500);

	engine = Engine::getInstance();
}

Window::~Window()
{
	Engine::kill();
}

QTreeView* Window::getTreeView()
{
	return treeView;
}

QStandardItemModel* Window::getStandardModel()
{
	return standardModel;
}

void Window::closeEvent(QCloseEvent* event)
{
	event->ignore();
	quit();
}

void Window::quit()
{
	if(QMessageBox::question(this, tr("Fragile Save Editor"), tr("Are you sure you want to quit?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
	{
		kill();
		qApp->quit();
	}
}

void Window::about()
{
	QMessageBox::about(this, tr("Fragile Save Editor"), tr("FSE © Valentin."));
}

void Window::find()
{
	QString text = QInputDialog::getText(this, tr("Fragile Save Editor"), tr("Enter an asteroid name:"));
	itemsFound = standardModel->findItems(text, Qt::MatchRecursive | Qt::MatchContains);
	iif = -1;
	findNext();
}

void Window::findNext()
{
	iif++;
	if(iif < itemsFound.size())
		treeView->selectionModel()->setCurrentIndex(itemsFound.at(iif)->index(), QItemSelectionModel::SelectCurrent);
	else
	{
		QMessageBox::information(this, tr("Fragile Save Editor"), tr("No match."));
		itemsFound.clear();
	}
}

void Window::open()
{
	QString fileName = QFileDialog::getOpenFileName(this, QString(), QDir::currentPath(), "SAVEGAME.*");
	if(!fileName.isEmpty())
		loadFile(fileName);
}

void Window::loadFile(const QString& fileName)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	if(engine->charger(fileName))
	{
		QString name = strippedName(fileName);

		standardModel->clear();
		QStandardItem* saveItem = new QStandardItem(name);
		saveItem->setEditable(false);
		standardModel->invisibleRootItem()->appendRow(saveItem);

		foreach(QString name, engine->getAsteroidsNames())
		{
			QStandardItem* item = new QStandardItem(name);
			item->setEditable(false);
			saveItem->appendRow(item);
		}

		treeView->expandAll();
		treeView->show();

		asteroidWidget->hide();
		moneyWidget->setName(name);
		moneyWidget->setMoney(engine->getMoney());
		moneyWidget->show();

		saveAct->setDisabled(false);
		saveAsAct->setDisabled(false);

		setCurrentFile(fileName);
		statusBar()->showMessage(tr("File loaded"), 5000);
	}
	else
		statusBar()->showMessage(tr("File is not a savegame"), 5000);
	QApplication::restoreOverrideCursor();
}

void Window::save()
{
	saveFile(curFile);
}

void Window::saveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this, QString(), QDir::currentPath(), "SAVEGAME.*");
	if(!fileName.isEmpty())
		saveFile(fileName);
}

void Window::saveFile(const QString& fileName)
{
	QApplication::setOverrideCursor(Qt::WaitCursor);
	if(engine->sauver(fileName))
	{
		setCurrentFile(fileName);
		statusBar()->showMessage(tr("File saved"), 5000);
	}
	QApplication::restoreOverrideCursor();
}

void Window::selectionChangedSlot()
{
	const QModelIndex index = treeView->selectionModel()->currentIndex();
	QString selectedText = index.data(Qt::DisplayRole).toString();
	int hierarchyLevel = 1;
	QModelIndex seekRoot = index;
	while(seekRoot.parent() != QModelIndex())
	{
		seekRoot = seekRoot.parent();
		hierarchyLevel++;
	}
	
	if(hierarchyLevel == 1)
	{
		asteroidWidget->hide();
		moneyWidget->setMoney(engine->getMoney());
		moneyWidget->show();
	}
	else
	{
		moneyWidget->hide();
		asteroidWidget->setName(selectedText);
		asteroidWidget->setAsteroid(engine->getAsteroid(selectedText));
		asteroidWidget->show();
	}
}

void Window::openRecentFile()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
		loadFile(action->data().toString());
}

void Window::setCurrentFile(const QString &fileName)
{
	curFile = fileName;

	QSettings settings;
	QStringList files = settings.value("recentFileList").toStringList();
	files.removeAll(fileName);
	files.prepend(fileName);
	while (files.size() > MaxRecentFiles)
		files.removeLast();

	settings.setValue("recentFileList", files);

	foreach (QWidget *widget, QApplication::topLevelWidgets()) {
		Window *mainWin = qobject_cast<Window *>(widget);
		if (mainWin)
			mainWin->updateRecentFileActions();
	}
}

void Window::updateRecentFileActions()
{
	QSettings settings;
	QStringList files = settings.value("recentFileList").toStringList();

	int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

	for (int i = 0; i < numRecentFiles; ++i) {
		QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
		recentFileActs[i]->setText(text);
		recentFileActs[i]->setData(files[i]);
		recentFileActs[i]->setVisible(true);
	}
	for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
		recentFileActs[j]->setVisible(false);

	separatorAct->setVisible(numRecentFiles > 0);
}

QString Window::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}
