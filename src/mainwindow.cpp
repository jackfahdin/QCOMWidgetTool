#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "commandlistitem.h"
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMinimumSize(1000, 920);

    // 创建CommandListItem实例并添加到listWidgetCommanList
    for(int i = 0; i < 17; i++) {
        CommandListItem *commandItem = new CommandListItem(this);
        QListWidgetItem *newItem = new QListWidgetItem();
        newItem->setSizeHint(commandItem->sizeHint());
        ui->listWidgetCommanList->addItem(newItem);
        ui->listWidgetCommanList->setItemWidget(newItem, commandItem);
    }

    // 修改每个CommandListItem中的数据
    for (int i = 0; i < ui->listWidgetCommanList->count(); ++i) {
        QListWidgetItem *item = ui->listWidgetCommanList->item(i);
        CommandListItem *commandItem = dynamic_cast<CommandListItem*>(ui->listWidgetCommanList->itemWidget(item));
        if (commandItem) {
            commandItem->getCheckBoxCommandListName()->setText(QString("%1:").arg(i + 1));
            commandItem->getLineEditCommand()->setText("");
            // 根据需要修改其他组件的数据
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 重写resizeEvent
void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    QSize newSize = event->size();
    qDebug() << "当前窗口大小：" << newSize.width() << " x " << newSize.height();
}
