#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupStyles();
    setupAnimations();
    updateButtonText();

    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->listWidget->setFocusPolicy(Qt::NoFocus);
}

void MainWindow::setupStyles()
{
    this->setStyleSheet(R"(
        QMainWindow {
            background-color: #121212;
        }

        QListWidget {
            background-color: #1E1E1E;
            border: 1px solid #444;
            color: #E0E0E0;
            font-size: 14px;
            outline: 0;
        }
        QListWidget::item {
            background-color: #252525;
            border-bottom: 1px solid #333;
            padding: 12px;
            margin: 4px;
            border-radius: 6px;
            min-height: 40px;
        }
        QListWidget::item:hover {
            background-color: #2D2D2D;
        }
        QListWidget::item:selected {
            background-color: #3A3A3A;
            color: white;
        }
        QListWidget::item:checked {
            color: #A0A0A0;
            text-decoration: line-through;
        }

        QLineEdit {
            background-color: #252525;
            color: #E0E0E0;
            border: 1px solid #444;
            border-radius: 6px;
            padding: 10px;
            font-size: 14px;
            selection-background-color: #3A3A3A;
        }

        QPushButton {
            background-color: #333;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 10px 20px;
            font-size: 14px;
            min-width: 100px;
        }
        QPushButton:hover {
            background-color: #444;
        }
        QPushButton:pressed {
            background-color: #2A2A2A;
        }

        QScrollBar:vertical {
            background: #1E1E1E;
            width: 12px;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background: #444;
            min-height: 30px;
            border-radius: 6px;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
    )");
}

void MainWindow::setupAnimations()
{
    for (QPushButton* btn : findChildren<QPushButton*>()) {
        btn->installEventFilter(this);
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (auto* btn = qobject_cast<QPushButton*>(watched)) {
        if (event->type() == QEvent::MouseButtonPress) {
            QPropertyAnimation* pressAnim = new QPropertyAnimation(btn, "geometry");
            pressAnim->setDuration(80);
            pressAnim->setStartValue(btn->geometry());
            pressAnim->setEndValue(btn->geometry().adjusted(0, 0, 0, 0)); // Без смещения
            pressAnim->start(QPropertyAnimation::DeleteWhenStopped);
            btn->setStyleSheet(
                "QPushButton {"
                "   background-color: #2A2A2A;"
                "   border: 1px solid #444;"
                "}"
                );
        }
        else if (event->type() == QEvent::MouseButtonRelease) {
            btn->setStyleSheet(
                "QPushButton {"
                "   background-color: #333;"
                "   border: none;"
                "}"
                );
        }
    }
    return QMainWindow::eventFilter(watched, event);
}
void MainWindow::performSort()
{
    QList<QPair<QString, bool>> tasks;

    for(int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem* item = ui->listWidget->item(i);
        tasks.append({item->text(), item->checkState() == Qt::Checked});
    }

    std::sort(tasks.begin(), tasks.end(), [this](const auto& a, const auto& b) {
        return m_showCompletedFirst ? (a.second && !b.second) : (!a.second && b.second);
    });

    ui->listWidget->clear();
    for(const auto& task : tasks) {
        QListWidgetItem* item = new QListWidgetItem(task.first);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(task.second ? Qt::Checked : Qt::Unchecked);
        item->setSizeHint(QSize(0, 40));
        ui->listWidget->addItem(item);
    }
}

void MainWindow::updateButtonText()
{
    ui->sortButton->setText(m_showCompletedFirst
                                ? "Выполненные "
                                : "Невыполненные");
}

void MainWindow::on_addButton_clicked()
{
    QString task = ui->lineEdit->text().trimmed();
    if(!task.isEmpty()) {
        QListWidgetItem* item = new QListWidgetItem(task);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        item->setSizeHint(QSize(0, 40));
        ui->listWidget->addItem(item);
        ui->lineEdit->clear();
    }
}

void MainWindow::on_deleteButton_clicked()
{
    qDeleteAll(ui->listWidget->selectedItems());
}

void MainWindow::on_sortButton_clicked()
{
    m_showCompletedFirst = !m_showCompletedFirst;
    performSort();
    updateButtonText();
}

MainWindow::~MainWindow()
{
    delete ui;
}
