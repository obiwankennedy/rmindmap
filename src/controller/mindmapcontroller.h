/***************************************************************************
 *	Copyright (C) 2019 by Renaud Guezennec                                 *
 *   http://www.rolisteam.org/contact                                      *
 *                                                                         *
 *   This software is free software; you can redistribute it and/or modify *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef MINDMAPCONTROLLER_H
#define MINDMAPCONTROLLER_H

#include <QObject>
#include <QPointer>
#include <QRectF>
#include <QUndoStack>
#include <memory>

class QAbstractItemModel;
class MindItemModel;
class MindNode;
class SpacingController;
class SelectionController;
class NodeStyleModel;
class NodeStyle;
class PositionedItem;
class MindItem;
class ImageModel;
class MindMapController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractItemModel* itemModel READ itemModel CONSTANT)
    Q_PROPERTY(QAbstractItemModel* styleModel READ styleModel CONSTANT)
    Q_PROPERTY(int defaultStyleIndex READ defaultStyleIndex WRITE setDefaultStyleIndex NOTIFY defaultStyleIndexChanged)
    Q_PROPERTY(QString filename READ filename WRITE setFilename NOTIFY filenameChanged)
    Q_PROPERTY(bool spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)
    Q_PROPERTY(SelectionController* selectionCtrl READ selectionController CONSTANT)
    Q_PROPERTY(bool canRedo READ canRedo NOTIFY canRedoChanged)
    Q_PROPERTY(bool canUndo READ canUndo NOTIFY canUndoChanged)
    Q_PROPERTY(QString errorMsg READ errorMsg WRITE setErrorMsg NOTIFY errorMsgChanged)
    Q_PROPERTY(QRectF contentRect READ contentRect NOTIFY contentRectChanged)
    Q_PROPERTY(int defaultStyleIndex READ defaultStyleIndex WRITE setDefaultStyleIndex NOTIFY defaultStyleIndexChanged)
    Q_PROPERTY(bool linkLabelVisibility READ linkLabelVisibility WRITE setLinkLabelVisibility NOTIFY
                   linkLabelVisibilityChanged)
    Q_PROPERTY(bool hasSelection READ hasSelection NOTIFY hasSelectionChanged)
    Q_PROPERTY(ImageModel* imgModel READ imgModel CONSTANT)
public:
    explicit MindMapController(QObject* parent= nullptr);
    ~MindMapController();

    QAbstractItemModel* itemModel() const;
    QAbstractItemModel* styleModel() const;
    ImageModel* imgModel() const;

    SelectionController* selectionController() const;
    const QString& filename() const;
    const QString& errorMsg() const;
    QRectF contentRect() const;

    bool spacing() const;
    bool canRedo() const;
    bool canUndo() const;
    int defaultStyleIndex() const;
    Q_INVOKABLE NodeStyle* style(int index) const;

    bool linkLabelVisibility() const;
    void setLinkLabelVisibility(bool newLinkLabelVisibility);

    bool hasSelection() const;

signals:
    void filenameChanged();
    void spacingChanged();
    void canRedoChanged();
    void canUndoChanged();
    void errorMsgChanged();
    void defaultStyleIndexChanged();
    void contentRectChanged();
    void linkLabelVisibilityChanged();

    void hasSelectionChanged();

public slots:
    void saveFile();
    void loadFile();
    void setFilename(const QString& path);
    void resetData();
    void setSpacing(bool b);
    void redo();
    void undo();
    void setErrorMsg(const QString& msg);
    void importFile(const QString& path);
    void setDefaultStyleIndex(int indx);

    void addBox(const QString& idparent);
    void addPackage(const QPointF& pos);
    void updatePackage(const QPointF& pos);
    void addLink(const QString& start, const QString& id);

    void reparenting(MindItem* parent, const QString& id);
    void removeSelection();
    void setCurrentPackage(PositionedItem* item);

    void centerItems(qreal w, qreal h);
    void addImageFor(const QString& idNode, const QString& path);
    void addItemIntoPackage(const QString& idNode, const QString& idPack);
    void refresh();

protected:
    void clearData();

private:
    QString m_filename;
    QString m_errorMsg;
    std::unique_ptr<SpacingController> m_spacingController;
    std::unique_ptr<SelectionController> m_selectionController;
    std::unique_ptr<ImageModel> m_imgModel;
    std::unique_ptr<MindItemModel> m_itemModel;
    std::unique_ptr<NodeStyleModel> m_styleModel;
    QThread* m_spacing= nullptr;
    QUndoStack m_stack;
    int m_defaultStyleIndex= 0;
    QPointer<PositionedItem> m_package;
    bool m_linkLabelVisibility;
};

#endif // MINDMAPCONTROLLER_H
