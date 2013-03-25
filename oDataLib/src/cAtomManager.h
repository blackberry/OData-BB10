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
//using namespace bb::data;

class cAtomManager {

public:
	enum atomType { eAtomFeed, eAtomService, eAtomUndefined  };
	enum atomFeed { eAtomId, eAtomEntry, eAtomLink, eAtomTitle };
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

private:
	cAtomManager() {};
	cAtomManager(const cAtomManager&); // prevent use of copy constructor
	cAtomManager& operator=(const cAtomManager&); // prevent assignement
	virtual ~cAtomManager();

	const char* getString(atomFeed e);
	const char* getString(entryType e);

	atomType findType(QVariantMap& map);

	QVariantMap buildAtomElements(QByteArray& arrBytes);
	QVariantList parseAtomElements(QByteArray& arrBytes, QVariantMap& mapEntries, atomType eType = eAtomUndefined);
	int parseElementsforDataModel(QVariantList& entries, bb::cascades::ArrayDataModel& dt, atomType eAtomType, entryType eEntryType);
};

#endif /* CATOMMANAGER_H_ */
