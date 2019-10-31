#ifndef FILTERWINDOW_H
#define FILTERWINDOW_H

#include <QDialog>

#include "ui_filterwindow.h"
#include "usbproxy.h"

class FilterWindow : public QDialog
{
    Q_OBJECT

    Ui::FilterWindow ui;
    USBProxyFilter m_filter;

public:
    explicit FilterWindow(QWidget *parent = nullptr);

    void accept();
    void open();

    const USBProxyFilter *getFilter() const;
};

#endif // FILTERWINDOW_H
