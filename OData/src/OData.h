/*
 * OData.h
 *
 *  Created on: 2013-03-26
 *      Author: Daniel Baxter
 */

#ifndef ODATA_H_
#define ODATA_H_

#include <qobject.h>

class OData : public QObject {
    Q_OBJECT

public:
    OData();
    virtual ~OData();
};

#endif /* ODATA_H_ */
