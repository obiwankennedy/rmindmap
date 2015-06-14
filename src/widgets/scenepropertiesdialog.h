#ifndef SCENEPROPERTIESDIALOG_H
#define SCENEPROPERTIESDIALOG_H

#include <QDialog>

namespace Ui {
class ScenePropertiesDialog;
}

class ScenePropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScenePropertiesDialog(QWidget *parent = 0);
    ~ScenePropertiesDialog();

private:
    Ui::ScenePropertiesDialog *ui;
};

#endif // SCENEPROPERTIESDIALOG_H
