#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void on_addButton_clicked();
    void on_deleteButton_clicked();
    void on_sortButton_clicked();

private:
    Ui::MainWindow *ui;
    bool m_showCompletedFirst = true;

    void setupStyles();
    void setupAnimations();
    void performSort();
    void updateButtonText();
};
#endif
