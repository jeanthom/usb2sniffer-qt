#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QDialog>
#include "ui_aboutwindow.h"

class AboutWindow : public QDialog
{
    Q_OBJECT

    Ui::AboutWindow ui;

public:
    explicit AboutWindow(QWidget *parent = nullptr);
};

#endif // ABOUTWINDOW_H
