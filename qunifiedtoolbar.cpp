#include "qunifiedtoolbar.h"

#ifdef Q_OS_MACOS
#include "qunifiedtoolbar_cocoa.h"
#endif

QUnifiedToolBar::QUnifiedToolBar(QWidget* _parent) : QToolBar(_parent) {
	#ifdef Q_OS_MACOS
	hide();

	parent = _parent;
	macToolbar = new QMacToolBar(_parent);
	setNSToolbarEditable(macToolbar, false);
	#endif

	showText(false);
}

#ifdef Q_OS_MACOS
QAction* QUnifiedToolBar::addAction(QAction *action) {
	QMacToolBarItem *item = new QMacToolBarItem();
	item->setText(action->text());
	item->setIcon(action->icon());
	connect(item, &QMacToolBarItem::activated, action, &QAction::trigger);
	setNSToolbarItemEnabled(item, false);

	macToolbarItemList.append(item);

	return action;
}
#endif

void QUnifiedToolBar::populationEnded() {
	#ifdef Q_OS_MACOS
	macToolbar->setAllowedItems(macToolbarItemList);
	macToolbar->setItems(macToolbarItemList);
	parent->window()->winId();
	macToolbar->attachToWindow(parent->window()->windowHandle());
	#endif
}

void QUnifiedToolBar::showText(bool show) {
	#ifdef Q_OS_MACOS
	setNSToolbarShowText(macToolbar, show);
	#endif
}
