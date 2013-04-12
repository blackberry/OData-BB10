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

    Q_INVOKABLE QVariantList getModelStructure(QString modelKey, QVariant metadata);

    Q_INVOKABLE void createModel(QString postUrl);
    Q_INVOKABLE void updateModel(QString putUrl);

    Q_INVOKABLE void deleteModel();

signals:
    void modelReady();
    void sourceChanged();
    void modelChanged();

    void modelDeleted();

    void modelError(int code, QString message);
public slots:
    void jsonReadComplete(QVariant response);
    void atomReadComplete(QVariant response);

    void deleteComplete();

    void error(int code, QString message);
private:
    QString mSource;
    QVariant mModel;

    QVariantList mStructure; // the data structure of the object (used for creates and updates)

    void loadModel();
};

#endif /* ODATAOBJECTMODEL_H_ */
