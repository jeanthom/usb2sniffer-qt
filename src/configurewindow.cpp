#include "configurewindow.h"

#include <QDirIterator>

ConfigureWindow::ConfigureWindow(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);

    ui.comboSpeed->clear();
    ui.comboSpeed->addItem("HS (High Speed)", CaptureSpeed::HS);
    ui.comboSpeed->addItem("FS (Full Speed)", CaptureSpeed::FS);
    ui.comboSpeed->addItem("LS (Low Speed)", CaptureSpeed::LS);

    m_config.speed = CaptureSpeed::FS;
}

QStringList ConfigureWindow::listAvailableDevices()
{
    QStringList devices;
    QStringList filter = {"ft60x*"};
    QDirIterator it("/dev", filter, QDir::System);
    while (it.hasNext()) {
        devices << it.next();
    }
    return devices;
}

void ConfigureWindow::open()
{
    int index;
    QString current = m_config.device;

    /* Refresh devices list */

    ui.comboDevice->clear();
    QStringList devices = listAvailableDevices();
    ui.comboDevice->addItems(devices);

    /* Restore last device selection if still available */

    if (devices.contains(current)) {
        ui.comboDevice->setCurrentText(current);
    }

    index = ui.comboSpeed->findData(m_config.speed);
    ui.comboSpeed->setCurrentIndex(index);

    QDialog::open();
}

void ConfigureWindow::accept()
{
    /* Update configuration object based on user input */

    m_config.device = ui.comboDevice->currentText();
    m_config.speed = ui.comboSpeed->currentData().toInt();

    QDialog::accept();
}

void ConfigureWindow::autoConfig()
{
    /* If no device is selected, find the first available */

    if (m_config.device.isEmpty()) {
        QStringList devices = listAvailableDevices();
        m_config.device = devices.value(0);
    }
}
