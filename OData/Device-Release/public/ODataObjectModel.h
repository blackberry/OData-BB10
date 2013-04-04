/*
 * ODataObjectModel.h
 *
 *  Created on: 2013-03-28
 *      Author: Daniel Baxter
 */

#ifndef ODATAOBJECTMODEL_H_
#define ODATAOBJECTMODEL_H_

#include <qobject.h>
#include <qvariant.h>

class ODataObjectModel : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString source READ getSource WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QVariant model READ getModel WRITE setModel NOTIFY modelChanged)

public:
    ODataObjectModel();
    ODataObjectModel(QString source);
    virtual ~ODataObjectModel();

    QString getSource();
    void setSource(QString newSource);

    QVariant getModel();
    void setModel(QVariant newModel);

    Q_INVOKABLE void deleteModel();

signals:
    void modelReady();
    void sourceChanged();
    void modelChanged();

    void modelDeleted();

public slots:
    void jsonReadComplete(QVariant response);
    void atomReadComplete(QVariant response);

    void deleteComplete();

private:
    QString mSource;
    QVariant mModel;

    void loadData();
};

#endif /* ODATAOBJECTMODEL_H_ */
