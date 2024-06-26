#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QTextEdit>
#include <QKeyEvent>

class CustomTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    explicit CustomTextEdit(QWidget *parent = nullptr) : QTextEdit(parent) {}

protected:
    void keyPressEvent(QKeyEvent *event) override
    {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            emit enterPressed();
            // 可以在这里添加其他逻辑，例如阻止默认行为
            event->ignore();
        } else {
            QTextEdit::keyPressEvent(event);
        }
    }

signals:
    void enterPressed();
};

#endif // CUSTOMTEXTEDIT_H
