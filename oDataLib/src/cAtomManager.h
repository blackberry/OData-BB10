/*
 * cAtomManager.h
 *
 *  Created on: 2013-03-18
 *      Author: cmartin
 */

#ifndef CATOMMANAGER_H_
#define CATOMMANAGER_H_

#include <QtCore/QtGlobal>
#include <QObject>
#include <bb/cascades/ArrayDataModel>

using namespace bb::cascades;

class cAtomManager {

public:
	enum atomDocumentType { eAtomFeed, eAtomService, eAtomEntry  };

	// These enums needs to be reviewed
	enum atomFeed { eFeedId, eFeedEntry, eFeedLink, eFeedTitle };
	enum entryType { eId, eLink, eProperties, eAuthor };

public:
	static cAtomManager& Instance()
	{
		static cAtomManager singleton;
		return singleton;
	}

// Manager can...
	int fillDataModelItems(QByteArray& result, ArrayDataModel& dataModel);

	QByteArray createHTTP_request_test(const char* strHeaderType);

	QString getAtomXMLUpdatedDateTime();

private:
	cAtomManager() {};
	cAtomManager(const cAtomManager&); // prevent use of copy constructor
	cAtomManager& operator=(const cAtomManager&); // prevent assignement
	virtual ~cAtomManager();

	const char* getString(atomFeed e);
	const char* getString(entryType e);

	atomDocumentType findType(QVariantMap& map);

	QVariantMap buildAtomElements(QByteArray& arrBytes);
	QVariantList parseAtomElements(QByteArray& arrBytes, QVariantMap& mapEntries, atomDocumentType eType);
	int parseElementsforDataModel(QVariantList& entries, bb::cascades::ArrayDataModel& dt, atomDocumentType eAtomDocType, entryType eEntryType);
};

#endif /* CATOMMANAGER_H_ */
