#import <Cocoa/Cocoa.h>
#import <AppKit/AppKit.h>
#include <QMacToolBar>

void setNSToolbarItemEnabled(QMacToolBarItem *item, bool value) {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	NSToolbarItem *nativeItem = item->nativeToolBarItem();
	nativeItem.enabled = value;
	[pool release];
}

void setNSToolbarShowText(QMacToolBar *toolbar, bool show) {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	NSToolbar *nativeToolbar = toolbar->nativeToolbar();

	if (show) {
		nativeToolbar.displayMode = NSToolbarDisplayModeIconAndLabel;
	} else {
		nativeToolbar.displayMode = NSToolbarDisplayModeIconOnly;
	}

	[pool release];
}

void setNSToolbarEditable(QMacToolBar *toolbar, bool editable) {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

	NSToolbar *nativeToolbar = toolbar->nativeToolbar();
	nativeToolbar.allowsUserCustomization = editable;

	[pool release];
}