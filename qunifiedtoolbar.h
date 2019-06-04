#ifndef QUNIFIEDTOOLBAR_H
#define QUNIFIEDTOOLBAR_H

#include <QToolBar>

#ifdef Q_OS_MACOS
#include <QMacToolBar>
#endif

class QUnifiedToolBar : public QToolBar {
	Q_OBJECT

public slots:
	#ifdef Q_OS_MACOS
	//void actionChanged(QAction *action);
	#endif
	
public:
	QUnifiedToolBar(QWidget *parent = nullptr);

	#ifdef Q_OS_MACOS
	QAction* addAction(QAction* action);
	#endif
	void populationEnded();
	void showText(bool show);
private:
	#ifdef Q_OS_MACOS
	QWidget *parent;
	QMacToolBar *macToolbar;
	QList<QMacToolBarItem *> macToolbarItemList;
	#endif
};

#endif