#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_buttonSearch_clicked();

    void on_buttonGoSearch_clicked();

    void on_buttonPath_clicked();

    void on_buttonCloseAdd_clicked();

    void on_buttonCloseSearch_clicked();

    void on_buttonGoAdd_clicked();

    void on_resultsBrowser_anchorClicked(const QUrl &arg1);

    void on_buttonGoBack_clicked();

    void on_buttonCloseResult_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
