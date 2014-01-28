#include "detailpanel.h"
#include "ui_detailpanel.h"

DetailPanel::DetailPanel(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DetailPanel)
{
    m_currentEdge = NULL;
    m_currentNode = NULL;
    ui->setupUi(this);

    connect(ui->m_nodeDescriptionEdit,SIGNAL(textChanged()),this,SLOT(updateCurrentNode()));
    connect(ui->m_nodeTextEdit,SIGNAL(textEdited(QString)),this,SLOT(updateCurrentNode()));
    connect(ui->m_bgColorChooser,SIGNAL(colorChanged(QColor)),this,SLOT(updateCurrentNode()));
    connect(ui->m_borderColorChooser,SIGNAL(colorChanged(QColor)),this,SLOT(updateCurrentNode()));


    connect(ui->m_edgeTextEdit,SIGNAL(textEdited(QString)),this,SLOT(updateCurrentEdge()));
    connect(ui->m_edgeArrowType,SIGNAL(currentIndexChanged(int)),this,SLOT(updateCurrentEdge()));


    connect(ui->m_packageText,SIGNAL(textEdited(QString)),this,SLOT(updateCurrentPackage()));
    connect(ui->m_packageLine,SIGNAL(currentIndexChanged(int)),this,SLOT(updateCurrentPackage()));

    connect(ui->m_xEdit,SIGNAL(valueChanged(int)),this,SLOT(updateCurrentPackage()));
    connect(ui->m_yEdit,SIGNAL(valueChanged(int)),this,SLOT(updateCurrentPackage()));


}

DetailPanel::~DetailPanel()
{
    delete ui;
}
void DetailPanel::setCurrentNode(Node* node)
{

    m_currentEdge = NULL;
    m_currentNode = NULL;
    m_currentPackage = NULL;
    ui->m_stackedWidget->setCurrentIndex(0);
    ui->m_nodeDescriptionEdit->setText(node->description());
    ui->m_nodeTextEdit->setText(node->getText());
    ui->m_bgColorChooser->setColor(node->bgColor());
    ui->m_borderColorChooser->setColor(node->color());

    m_currentNode=node;
}

void DetailPanel::setCurrentEdge(Edge* edge)
{

    m_currentEdge = NULL;
    m_currentNode = NULL;
    m_currentPackage = NULL;
    ui->m_stackedWidget->setCurrentIndex(1);
    ui->m_edgeTextEdit->setText(edge->getText());
    ui->m_edgeArrowType->setCurrentIndex((int)edge->getKind());
    m_currentEdge = edge;
}



void DetailPanel::updateCurrentNode()
{
    if(NULL!=m_currentNode)
    {
        m_currentNode->setDescription(ui->m_nodeDescriptionEdit->document()->toHtml());
        m_currentNode->setText(ui->m_nodeTextEdit->text());
        m_currentNode->setColor(ui->m_borderColorChooser->color());
        m_currentNode->setBgColor(ui->m_bgColorChooser->color());
    }
}

void DetailPanel::updateCurrentEdge()
{
    if(NULL!=m_currentEdge)
    {
        m_currentEdge->setText(ui->m_edgeTextEdit->text());
        m_currentEdge->setKind((Edge::EndKind)ui->m_edgeArrowType->currentIndex());
    }
}

void DetailPanel::updateCurrentPackage()
{
    if(NULL!=m_currentPackage)
    {
        m_currentPackage->setText(ui->m_packageText->text());
        m_currentPackage->setBorder((PackageItem::Border)ui->m_packageLine->currentIndex());
        m_currentPackage->setPos(ui->m_xEdit->value(),ui->m_yEdit->value());
    }
}
void DetailPanel::setCurrentPackage(PackageItem* p)
{
    m_currentEdge = NULL;
    m_currentNode = NULL;
    m_currentPackage = NULL;
    ui->m_stackedWidget->setCurrentIndex(2);
    ui->m_packageText->setText(p->getText());
    ui->m_xEdit->setValue(p->pos().x());
    ui->m_yEdit->setValue(p->pos().y());

    m_currentPackage = p;
}

void DetailPanel::setCurrentGenericItem(GenericMindMapItem* p)
{
    if(NULL!=dynamic_cast<PackageItem*>(p))
    {
        setCurrentPackage(dynamic_cast<PackageItem*>(p));
    }
    else if(NULL!=dynamic_cast<Node*>(p))
    {
        setCurrentNode(dynamic_cast<Node*>(p));
    }
    else if(NULL!=dynamic_cast<Edge*>(p))
    {
        setCurrentEdge(dynamic_cast<Edge*>(p));
    }
}
