/*
 * ODataQuery.h
 *
 *  Created on: 20/03/2013
 *      Author: adrian
 */

#ifndef ODATAQUERY_H_
#define ODATAQUERY_H_

#include <QObject>

class ODataQuery : public QObject {

    Q_OBJECT

    Q_PROPERTY(QString uri READ uri WRITE setUri)

private:
    QString requestUri;

public:
    ODataQuery(QObject *parent = NULL);
    virtual ~ODataQuery();

    Q_INVOKABLE ODataQuery* setUri(const QString &baseUri);
    QString uri( ) const;

    Q_INVOKABLE ODataQuery* addQuery(const QString& queryField, const QString& queryValue);


    Q_INVOKABLE ODataQuery* format(const QString& formatValue);
    Q_INVOKABLE ODataQuery* filter(const QString& filterQuery);
    Q_INVOKABLE ODataQuery* orderBy(const QString& fieldAndOrientation);

    Q_INVOKABLE QString getQueryText();

};

#endif /* ODATAQUERY_H_ */
