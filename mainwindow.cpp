#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::on_addButton_clicked()
{
    QString task = ui->lineEdit->text();
    if (!task.isEmpty()) {
        QListWidgetItem *item = new QListWidgetItem(task);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        ui->listWidget->addItem(item);
        ui->lineEdit->clear();
    }
}

void MainWindow::on_deleteButton_clicked()
{
    qDeleteAll(ui->listWidget->selectedItems());
}

// void MainWindow::on_sortButton_clicked()
// {
//     ui->listWidget->sortItems(sortOrder ? Qt::DescendingOrder : Qt::AscendingOrder);
//     sortOrder = !sortOrder;
// }
