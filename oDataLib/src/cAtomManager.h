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
	static cAtomManager& Instance() {
		static cAtomManager singleton;
		return singleton;
	}

// Manager can...
	int fillDataModelItems(QByteArray& result, ArrayDataModel& dataModel);

	QVariant buildPropertyList(const QByteArray& arrMetadata, const QString& sSchema, const QString& sEntityType);
	QByteArray buildXMLfromPropertyList(QVariantList& propertiesList, const QString& sCollectionName, const QString& sNamespace, const QString& sEntityName);
	int generateEntriesDefnFromMetadata(QByteArray& result, ArrayDataModel& dataModel);

	void setHeaders(QNetworkRequest& request, const QByteArray& body);

private:
	cAtomManager() {};
	cAtomManager(const cAtomManager&); // prevent use of copy constructor
	cAtomManager& operator=(const cAtomManager&); // prevent assignement
	virtual ~cAtomManager();

	const char* getString(atomFeed e);
	const char* getString(entryType e);

	atomDocumentType findType(QVariantMap& map);
	bool isNamespaceForApp(QVariantMap& map);
	bool isNamespaceForAtom(QVariantMap& map);

	// Build XML
	QByteArray createBodyHeader();
    QByteArray createEntryHeader(const QString& sBase);
    QByteArray createTitleForEntryHeader(const QString& sTitle);
    QByteArray createContentTypeHeader(const QString& sContentType);
    QByteArray createCategoryTermHeader(const QString& sCategory, const QString& sSchema);
    QByteArray createPropertyHeader();
    QByteArray createXMLwithTag(const QString& sTag, const QString& value);
    QByteArray createClosureXMLwithTag(const QString& sTag, bool bAdd_CR_LF = false);
	QString getAtomXMLCurrentDateTime();

	QVariantMap buildAtomElements(const QByteArray& arrBytes);
	QVariantList parseToODataAtomElements(QByteArray& arrBytes, QVariantMap& mapEntries, atomDocumentType eType);
	int parseElementsforDataModel(QVariantList& entries, bb::cascades::ArrayDataModel& dt, atomDocumentType eAtomDocType, entryType eEntryType);
};

#endif /* CATOMMANAGER_H_ */
