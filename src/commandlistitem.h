#ifndef COMMANDLISTITEM_H
#define COMMANDLISTITEM_H

#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

class CommandListItem : public QWidget {
    Q_OBJECT
public:
    explicit CommandListItem(QWidget *parent = nullptr);

    QCheckBox* getCheckBoxCommandListName() const { return checkBoxCommandListName; }
    QLineEdit* getLineEditCommand() const { return lineEditCommand; }

private:
    QCheckBox *checkBoxCommandListName;
    QLineEdit *lineEditCommand;
    QCheckBox *checkBoxEnableHex;
    QCheckBox *checkBoxEnter;
    QPushButton *pushButtonCommandEnter;
    QLineEdit *lineEditDelaymS;
    QVBoxLayout *mainLayout;
    QHBoxLayout *innerLayout;
};

#endif // COMMANDLISTITEM_H
