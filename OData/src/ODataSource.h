/*
 * ODataSource.h
 *
 *  Created on: 2013-03-26
 *      Author: Daniel Baxter
 */

#ifndef ODATASOURCE_H_
#define ODATASOURCE_H_

#include <qobject.h>

class ODataSource : public QObject {
    Q_OBJECT

public:
    ODataSource();
    virtual ~ODataSource();
};

#endif /* ODATASOURCE_H_ */
