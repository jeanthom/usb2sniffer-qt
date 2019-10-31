#ifndef CONFIGUREWINDOW_H
#define CONFIGUREWINDOW_H

#include <QDialog>
#include <QStringList>

#include "capture.h"
#include "ui_configurewindow.h"

class ConfigureWindow : public QDialog
{
	Ui::ConfigureWindow ui;
public:
    explicit ConfigureWindow(QWidget *parent = nullptr);

    void accept();
    void open();

    QStringList listAvailableDevices();
    void autoConfig();

    CaptureConfig m_config;
};

#endif // CONFIGUREWINDOW_H
