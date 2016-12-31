#include <QPainter>
#include <QGraphicsProxyWidget>

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <math.h>
#include <QtCore/qmath.h>

#include "edge.h"
#include "items/genericmindmapitem.h"
#include "graphwidget.h"



static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;
static const int Arrow_Size = 8;

Edge::Edge()
    : m_arrowSize(Arrow_Size),m_source(nullptr),m_dest(nullptr)
{
    init();
}

Edge::Edge(EdgableItem* sourceNode,EdgableItem* destNode)
    : m_arrowSize(Arrow_Size),m_source(sourceNode),m_dest(destNode)
{
    init();
   // m_source = sourceNode;
   // m_dest = destNode;
    m_source->addEdge(this);
    m_dest->addEdge(this);
    adjust();

}
Edge::Edge(EdgableItem *sourceNode)
    : m_arrowSize(Arrow_Size),m_source(sourceNode),m_dest(nullptr)
{
    init();


    m_dest = NULL;

    if(sourceNode!=NULL)
    {
        m_source = sourceNode;
        m_source->addEdge(this);
        adjust();
    }
}
void Edge::init()
{

    m_type = Straight;

    m_showTanPoints= false;
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setCacheMode(NoCache);



    m_arrowHead = BOTH;
    m_proxyw =NULL;
    m_tempedit = new QLineEdit();
    m_tempedit->setFrame(false);
    m_tempedit->setEnabled(true);
    m_tempedit->setFocus();
    connect(m_tempedit,SIGNAL(textChanged(QString)),this,SLOT(setText(QString)));
    connect(m_tempedit,SIGNAL(editingFinished()),m_tempedit,SLOT(hide()));
    connect(m_tempedit,SIGNAL(textChanged(QString)),this,SLOT(updatePainting()));
}

EdgableItem *Edge::sourceNode() const
{
    return m_source;
}

EdgableItem *Edge::destNode() const
{
    return m_dest;
}

void Edge::adjust()
{
    if (!m_source)
        return;

    QLineF line;

    if(!m_dest)
        line=QLineF(mapFromItem(m_source,m_source->middlePoint()), mapFromItem(m_dest, 0,0));
    else
        line= QLineF(mapFromItem(m_source,m_source->middlePoint()), mapFromItem(m_dest, m_dest->middlePoint()));

    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.))
    {
        qreal dx = line.dx();
        qreal dy = line.dy();
        if(abs(dx)*1.3>abs(dy))
        {
            dy=0;
        }
        else
        {
            dx=0;
        }
         QPointF middlePoint = mapFromItem(m_source,m_source->middlePoint());
        if(dx==0)
        {
            sourcePoint.setX(middlePoint.x());
            if(dy<0)
            {
                sourcePoint.setY(middlePoint.y()-m_source->boundingRect().height()/2);
            }
            else
            {
               sourcePoint.setY(middlePoint.y()+m_source->boundingRect().height()/2);
            }
        }
        else//dy == 0
        {
            sourcePoint.setY(middlePoint.y());
            if(dx>0)
            {
                sourcePoint.setX(middlePoint.x() + m_source->boundingRect().width()/2);
            }
            else
            {
               sourcePoint.setX(middlePoint.x() - m_source->boundingRect().width()/2);
            }
        }
        line.setP1(sourcePoint);
        if(NULL!=m_dest)
        {

            QPointF middlePoint = mapFromItem(m_dest,m_dest->middlePoint());

            if(dx==0)
            {
                destPoint.setX(middlePoint.x());
                if(dy<0)
                {
                    destPoint.setY(middlePoint.y() + m_dest->boundingRect().height()/2);
                }
                else
                {
                    destPoint.setY(middlePoint.y() - m_dest->boundingRect().height()/2);
                }
            }
            else
            {
                if(dx<0)
                {
                    destPoint.setX(middlePoint.x() + m_dest->boundingRect().width()/2);
                }
                else
                {
                    destPoint.setX(middlePoint.x() - m_dest->boundingRect().width()/2);
                }
                destPoint.setY(middlePoint.y());
            }
        }
    }
    else
    {
        sourcePoint = line.p1();
        if(NULL!=m_dest)
        {
            destPoint = line.p1();
        }
    }





}
void  Edge::setDestination(EdgableItem *destNode)
{
    m_dest = destNode;
    m_dest->addEdge(this);
    adjust();

}
void  Edge::setSource(EdgableItem *srcNode)
{
    m_source = srcNode;
    m_source->addEdge(this);
    adjust();

}
QRectF Edge::boundingRect() const
{
    if (!m_source)
        return QRectF();



    qreal penWidth = 1;
    qreal extra = (penWidth + m_arrowSize) / 2.0;

    QRectF  myRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),destPoint.y() - sourcePoint.y()));
    myRectF=myRectF.normalized();
    myRectF=myRectF.adjusted(-extra, -extra, extra, extra);

    if(myRectF.width()<m_textRect.width()*2)
    {
        myRectF.adjust(0,0,m_textRect.width(),0);
    }
    if(myRectF.height()<m_textRect.height())
    {
        myRectF.adjust(0,-m_textRect.height(),0,0);
    }

    return myRectF;
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem * option, QWidget *)
{
    if (!m_source)
        return;

    if(destPoint.isNull())
    {
        destPoint = sourcePoint;
    }

    /*    AB.AC = ABx*ACx + ABy*ACy + ABz*ACz
    ||AB|| = sqrt(ABx²+ABy²+ABz²)
    ||AC|| = sqrt(ACx²+ACy²+ACz²)
*/


    QPointF pointC= sourcePoint;
    pointC.setX(pointC.x()+10);

    QPointF AB(sourcePoint.x()-destPoint.x(),sourcePoint.y()-destPoint.y());
    QPointF AC(sourcePoint.x()-pointC.x(),sourcePoint.y()-pointC.y());

    qreal abac = AB.x()*AC.x()+AB.y()*AC.y();
    qreal ABdist=sqrt(AB.x()*AB.x()+AB.y()*AB.y());
    qreal ACdist=sqrt(AC.x()*AC.x()+AC.y()*AC.y());


    qreal angleVector = qAcos(abac/(ABdist*ACdist));

    QPointF srcMiddlePoint = mapFromItem(m_source,m_source->middlePoint());
    if((srcMiddlePoint.x()>sourcePoint.x())&&(srcMiddlePoint.y()==sourcePoint.y()))//if(angleVector<=Pi/2)//if source is on left of the destination.
    {

        double distx = abs(destPoint.x()-sourcePoint.x())/2;
        distx*=(angleVector > 1) ? 1 : angleVector;
        m_sourceTanPoint.setX(sourcePoint.x()-distx);
        m_sourceTanPoint.setY(sourcePoint.y());

    }
    else if((srcMiddlePoint.x()<sourcePoint.x())&&(srcMiddlePoint.y()==sourcePoint.y())) //(angleVector>=3*Pi/4)//if source is on right of the destination.
    {

        double distx = abs(sourcePoint.x()-destPoint.x())/2;
        distx*=(angleVector > 1) ? 1 : angleVector;
        m_sourceTanPoint.setX(sourcePoint.x()+distx);
        m_sourceTanPoint.setY(sourcePoint.y());
    }
    else if((srcMiddlePoint.y()>sourcePoint.y())&&(srcMiddlePoint.x()==sourcePoint.x()))//if(angleVector<=Pi/2)//if source is on left of the destination.
    {

        double disty = (destPoint.y()-sourcePoint.y())/2;
        disty*=(angleVector > 1) ? 1 : angleVector;
        m_sourceTanPoint.setX(sourcePoint.x());
        m_sourceTanPoint.setY(sourcePoint.y()+disty);

    }
    else if((srcMiddlePoint.y()<sourcePoint.y())&&(srcMiddlePoint.x()==sourcePoint.x())) //(angleVector>=3*Pi/4)//if source is on right of the destination.
    {

        double disty = (sourcePoint.y()-destPoint.y())/2;
        disty*=(angleVector > 1) ? 1 : angleVector;
        m_sourceTanPoint.setX(sourcePoint.x());
        m_sourceTanPoint.setY(sourcePoint.y()-disty);
    }
    QPointF destMiddlePoint;
    if(m_dest==NULL)
    {
        destMiddlePoint = destPoint;
    }
    else
    {
        destMiddlePoint = mapFromItem(m_dest,m_dest->middlePoint());

    }

    if((m_dest!=NULL)&&(destMiddlePoint.y()>destPoint.y())&&(destMiddlePoint.x()==destPoint.x()))//if source is below of the destination.
    {
        double disty = (sourcePoint.y()-destPoint.y())/2;


        disty*=(angleVector > 1) ? 1 : angleVector;

        m_destTanPoint.setX(destPoint.x());
        m_destTanPoint.setY(destPoint.y()+disty);
    }
    else if((m_dest!=NULL)&&(destMiddlePoint.y()<destPoint.y())&&(destMiddlePoint.x()==destPoint.x()))//if source is above of the destination.
    {


        double disty = (destPoint.y()-sourcePoint.y())/2;
        disty*=(angleVector > 1) ? 1 : angleVector;

        m_destTanPoint.setX(destPoint.x());
        m_destTanPoint.setY(destPoint.y()-disty);
    }
    else if((m_dest!=NULL)&&(destMiddlePoint.x()>destPoint.x())&&(destMiddlePoint.y()==destPoint.y()))//if source is below of the destination.
    {

        double distx = abs(sourcePoint.x()-destPoint.x())/2;

        distx*=(angleVector > 1) ? 1 : angleVector;

        m_destTanPoint.setX(destPoint.x()-distx);
        m_destTanPoint.setY(destPoint.y());
    }
    else if((m_dest!=NULL)&&(destMiddlePoint.x()<destPoint.x())&&(destMiddlePoint.y()==destPoint.y()))//if source is above of the destination.
    {

        double distx = abs(destPoint.y()-sourcePoint.y())/2;
        distx*=(angleVector > 1) ? 1 : angleVector;

        m_destTanPoint.setX(destPoint.x()+distx);
        m_destTanPoint.setY(destPoint.y());
    }





    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;


    // Draw the line itself
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    painter->save();
    painter->setPen(QPen(Qt::white, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QFont font;
    QFontMetrics info(font);

    QRect textRect = info.boundingRect(QString("__%1__").arg(m_text));
    textRect.translate(line.pointAt(0.5).toPoint());
    textRect.translate(-textRect.width()/2,0);
    painter->drawText(textRect,Qt::AlignCenter,m_text);
    //if(!m_text.isEmpty())
    {
        painter->fillRect(textRect,Qt::black);
    }
    painter->restore();

    /// TEST of BOUNDING RECT
    if(option->state & QStyle::State_Selected)
    {
        qreal penWidth = 1;
        qreal extra = (penWidth + m_arrowSize) / 2.0;

        painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawRect(QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
                                                     destPoint.y() - sourcePoint.y()))
                          .normalized()
                          .adjusted(-extra, -extra, extra, extra));
    }
    /// END OF TEST of BOUNDING RECT




    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    // Draw the arrows
    QLineF lineSRC;
    QLineF lineDEST;

    if(Curve == m_type)
    {
        lineSRC.setPoints(sourcePoint, m_sourceTanPoint);
        lineDEST.setPoints(m_destTanPoint, destPoint);
    }
    else  if(Straight == m_type)
    {
        lineSRC.setPoints(sourcePoint, destPoint);
        lineDEST.setPoints(sourcePoint, destPoint);
    }
    qreal angleSRC = ::acos(lineSRC.dx() / (lineSRC.length() > 0 ? lineSRC.length() : 1) );
    qreal angleDEST = ::acos(lineDEST.dx() / (lineDEST.length() > 0 ? lineDEST.length() : 1));

    QPointF sourceArrowP1;
    QPointF sourceArrowP2;
    QPointF destArrowP1;
    QPointF destArrowP2;
    if((sourcePoint.y() < destPoint.y()) && (lineDEST.dy()!=0))
    {

        sourceArrowP1 = sourcePoint - QPointF(sin(angleSRC + Pi / 3) * m_arrowSize,
                                                  cos(angleSRC + Pi / 3) * m_arrowSize);
        sourceArrowP2 = sourcePoint - QPointF(sin(angleSRC + Pi - Pi / 3) * m_arrowSize,
                                                  cos(angleSRC + Pi - Pi / 3) * m_arrowSize);

        destArrowP1 = destPoint - QPointF(sin(angleDEST - Pi / 3) * m_arrowSize,
                                              cos(angleDEST - Pi / 3) * m_arrowSize);
        destArrowP2 = destPoint - QPointF(sin(angleDEST - Pi + Pi / 3) * m_arrowSize,
                                              cos(angleDEST - Pi + Pi / 3) * m_arrowSize);


    }
    else
    {
        sourceArrowP1 = sourcePoint + QPointF(sin(angleSRC + Pi / 3) * m_arrowSize,
                                                  cos(angleSRC + Pi / 3) * m_arrowSize);
        sourceArrowP2 = sourcePoint + QPointF(sin(angleSRC + Pi - Pi / 3) * m_arrowSize,
                                                  cos(angleSRC + Pi - Pi / 3) * m_arrowSize);

        destArrowP1 = destPoint + QPointF(sin(angleDEST - Pi / 3) * m_arrowSize,
                                              cos(angleDEST - Pi / 3) * m_arrowSize);
        destArrowP2 = destPoint + QPointF(sin(angleDEST - Pi + Pi / 3) * m_arrowSize,
                                              cos(angleDEST - Pi + Pi / 3) * m_arrowSize);
    }

    painter->setBrush(Qt::black);
    QLineF lineFSrc(sourceArrowP1,sourceArrowP2);
    QLineF lineFDest(destArrowP1,destArrowP2);
    QPainterPath path;
    QPointF endPath;

    if((m_arrowHead == Edge::BOTH)||(m_arrowHead == Edge::END1))
    {
        painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
        path.moveTo(lineFSrc.pointAt(0.5));
    }
    else
    {
        path.moveTo(sourcePoint);
    }

    if((m_arrowHead == Edge::BOTH)||(m_arrowHead == Edge::END2))
    {
        painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
        endPath=lineFDest.pointAt(0.5);
    }
    else
    {
        endPath=destPoint;
    }
    painter->setBrush(Qt::NoBrush);

    QPen pen;
    if(m_showTanPoints)
    {
        pen.setWidth(10);
        painter->setPen(pen);
        painter->drawPoint(m_destTanPoint);
        painter->drawPoint(m_sourceTanPoint);
        pen.setWidth(1);
        painter->setPen(pen);
    }




    if(Straight == m_type)
    {
        painter->drawLine(line);
    }
    else if(Curve == m_type)
    {
        path.cubicTo(m_sourceTanPoint,m_destTanPoint,endPath);
    }



    m_path = path;
    painter->drawPath(path);
}
void Edge::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event )
{
    if(NULL==m_proxyw)
    {
        m_proxyw = scene()->addWidget(m_tempedit);
    }
    m_proxyw->setPos(event->pos().x(),event->pos().y());

    //QGraphicsProxyWidget* tmp = scene()->addWidget(m_tempedit);
    //tmp->setPos(pos().x()-boundingRect().width(),pos().y()-boundingRect().height());
    m_tempedit->setText(m_text);
    m_tempedit->selectAll();
    m_tempedit->setFocus(Qt::OtherFocusReason);
    m_tempedit->show();

}
void Edge::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsObject::mousePressEvent(event);
}

void Edge::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsObject::mouseReleaseEvent(event);
}
void Edge::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsObject::mouseMoveEvent(event);
}
void Edge::setDestinationPoint(QPointF pos)
{
    destPoint = pos;
    update();
}
void Edge::setGrap(GraphWidget* m_graph)
{
    m_graph = m_graph;
}


void Edge::writeToData(QJsonObject& obj)
{
    obj["arrowSize"]=m_arrowSize;
    obj["text"]=m_text;
    obj["arrowHead"]=m_arrowHead;
    obj["SrcId"]=m_source->getUuid();
    obj["dstId"]=m_dest->getUuid();
/*    out << m_arrowSize;
    out << m_text;
    out << (int)m_endkind;
    out << m_source->getUuid();
    out << m_dest->getUuid();*/
}
void Edge::readFromData(QJsonObject& in)
{

    m_arrowSize = in["arrowSize"].toInt();
    m_text=in["text"].toString();
    m_arrowHead=(ArrowHead)in["arrowHead"].toInt();
    m_uuidSrc = in["SrcId"].toString();
    m_uuidDst = in["dstId"].toString();

    lookUpPoint();
    adjust();
    update();

  /*  in >> m_arrowSize;
    in >> m_text;
    setText(m_text);
    QString uuid;
    int i;
    in >> i;
    setKind((Edge::EndKind)i);
    in >>m_uuidSrc;

    in >>m_uuidDst;


    adjust();
    update();*/
}
QString Edge::getDestId() const
{
    return m_uuidDst;
}

void Edge::lookUpPoint()
{
    //m_source= m_graph->getEdgableItemFromUuid(m_uuidSrc);
   // m_dest=  m_graph->getEdgableItemFromUuid(m_uuidDst);
/*    if(NULL!=m_source)
    {
        m_source->addEdge(this);
    }
    if(NULL!=m_dest)
    {
        m_dest->addEdge(this);
    }*/
}

QString Edge::getText() const
{

    return m_text;
}
void Edge::updatePainting()
{
    update();
}
void Edge::setText(QString t)
{
    m_text = t;
    QFont font;
    QFontMetrics info(font);
    m_textRect = info.boundingRect(m_text);
}
void Edge::setKind(Edge::ArrowHead kind)
{
    m_arrowHead = kind;
    updatePainting();
}

Edge::ArrowHead  Edge::getKind()
{
    return m_arrowHead;
}
QPainterPath  Edge::shape() const
{
//    QPainterPath path;


//    QLineF line(sourcePoint, destPoint);

//    // Draw the arrows
//    double angle = ::acos(line.dx() / line.length());
//    if (line.dy() >= 0)
//        angle = TwoPi - angle;
//    path.moveTo(sourcePoint);
//    QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * m_arrowSize,
//                                                  cos(angle + Pi / 3) * m_arrowSize);

//    path.lineTo(sourceArrowP1);
//    QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * m_arrowSize,
//                                                  cos(angle + Pi - Pi / 3) * m_arrowSize);

//    QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * m_arrowSize,
//                                              cos(angle - Pi / 3) * m_arrowSize);
//    QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * m_arrowSize,
//                                              cos(angle - Pi + Pi / 3) * m_arrowSize);
//    path.lineTo(destArrowP1);
//    path.lineTo(destArrowP2);
//    path.lineTo(sourceArrowP2);
//    path.closeSubpath ();

    return m_path;
}
void Edge::setName(QString&)
{

}

QString Edge::getName() const
{
    return tr("Edge");
}
QPixmap Edge::getIcon() const
{
    QPixmap pixmap(30,20);
    QPainter painter(&pixmap);
    painter.fillRect(pixmap.rect(),Qt::white);
    QLineF line(0,10,30,10);

    int arrowSize = 4;



    // Draw the line itself
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(line);




    // Draw the arrows
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwoPi - angle;

    QPointF sourceArrowP1 = line.p1() + QPointF(sin(angle + Pi / 3) * arrowSize,
                                                cos(angle + Pi / 3) * arrowSize);
    QPointF sourceArrowP2 = line.p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                                cos(angle + Pi - Pi / 3) * arrowSize);
    QPointF destArrowP1 = line.p2() + QPointF(sin(angle - Pi / 3) * arrowSize,
                                              cos(angle - Pi / 3) * arrowSize);
    QPointF destArrowP2 = line.p2() + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                                              cos(angle - Pi + Pi / 3) * arrowSize);

    painter.setBrush(Qt::black);
    if((m_arrowHead == Edge::BOTH)||(m_arrowHead == Edge::END1))
    {
        painter.drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    }
    if((m_arrowHead == Edge::BOTH)||(m_arrowHead == Edge::END2))
    {
        painter.drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
    }



    painter.end();
    return pixmap;
}



EdgableItem* Edge::getDestination()
{
    return m_dest;
}

EdgableItem* Edge::getSource()
{
    return m_source;
}
void Edge::setGeometry(int w,int h)
{

}
