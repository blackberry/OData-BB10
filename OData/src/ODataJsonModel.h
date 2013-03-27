/*
 * ODataJsonModel.h
 *
 *  Created on: 2013-03-26
 *      Author: Daniel Baxter
 */

#ifndef ODATAJSONMODEL_H_
#define ODATAJSONMODEL_H_

#include <qobject.h>
#include <qvariant.h>
#include <bb/cascades/DataModel>

class ODataJsonModel : public bb::cascades::DataModel {
    Q_OBJECT

    Q_PROPERTY(QString source READ getSource WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(int pageSize READ getPageSize WRITE setPageSize NOTIFY pageSizeChanged)
public:
    ODataJsonModel();
    virtual ~ODataJsonModel();

    QString getSource();
    void setSource(QString newSource);

    int getPageSize();
    void setPageSize(int newSize);

    Q_INVOKABLE QString itemType(const QVariantList &indexPath);
    Q_INVOKABLE QVariant data(const QVariantList& indexPath);
    Q_INVOKABLE int childCount(const QVariantList& indexPath);
    Q_INVOKABLE bool hasChildren(const QVariantList& indexPath);

signals:
    void sourceChanged();
    void pageSizeChanged();

public slots:
    void readCompleted(QVariant response);

private:
    QVariantList mDataList;
    QString mSource;
    int mPageSize;

    void loadData();
};

#endif /* ODATAJSONMODEL_H_ */
