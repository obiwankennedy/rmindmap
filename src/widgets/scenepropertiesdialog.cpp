#include "scenepropertiesdialog.h"
#include "ui_scenepropertiesdialog.h"

ScenePropertiesDialog::ScenePropertiesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScenePropertiesDialog)
{
    ui->setupUi(this);
}

ScenePropertiesDialog::~ScenePropertiesDialog()
{
    delete ui;
}
