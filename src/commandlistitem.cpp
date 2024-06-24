#include "commandlistitem.h"

CommandListItem::CommandListItem(QWidget *parent) : QWidget(parent) {
    checkBoxCommandListName = new QCheckBox("01:", this);
    lineEditCommand = new QLineEdit(this);
    checkBoxEnableHex = new QCheckBox(this);
    checkBoxEnter = new QCheckBox(this);
    pushButtonCommandEnter = new QPushButton("01", this);
    lineEditDelaymS = new QLineEdit(this);

    innerLayout = new QHBoxLayout();

    // checkBoxCommandListName
    QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(checkBoxCommandListName->sizePolicy().hasHeightForWidth());
    checkBoxCommandListName->setSizePolicy(sizePolicy);
    checkBoxCommandListName->setMaximumSize(QSize(32, 16777215));

    // pushButtonCommandEnter
    sizePolicy.setHeightForWidth(pushButtonCommandEnter->sizePolicy().hasHeightForWidth());
    pushButtonCommandEnter->setSizePolicy(sizePolicy);
    pushButtonCommandEnter->setMaximumSize(QSize(40, 16777215));

    // lineEditDelaymS
    sizePolicy.setHeightForWidth(lineEditDelaymS->sizePolicy().hasHeightForWidth());
    lineEditDelaymS->setSizePolicy(sizePolicy);
    lineEditDelaymS->setMaximumSize(QSize(40, 16777215));

    innerLayout->addWidget(checkBoxCommandListName);
    innerLayout->addWidget(lineEditCommand);
    innerLayout->addWidget(checkBoxEnableHex);
    innerLayout->addWidget(checkBoxEnter);
    innerLayout->addWidget(pushButtonCommandEnter);
    innerLayout->addWidget(lineEditDelaymS);

    mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(innerLayout);
    setLayout(mainLayout);
}
