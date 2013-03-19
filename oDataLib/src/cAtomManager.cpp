/*
 * cAtomManager.cpp
 *
 *  Created on: 2013-03-18
 *      Author: cmartin
 */

#include <bb/data/XmlDataAccess>

#include "cAtomManager.h"
#include "LOGGER.h"

// Incomplete const char* declarations
const char* atomEntry = "entry";
const char* atomLink = "link";
const char* atomId = "id";

// Incomplete const char* declarations
const char* entryProperties = "m:properties";

using namespace bb::cascades;
using namespace bb::data;

void sortMap(QVariantMap & map, int level);

void sortList(QVariantList & list, int level)
{
	LOGGER::log("\n ========================= LIST level:      ", level);
    //list = resultVal.value<QVariantList>();

	int sz = list.size();
    /*
     *  Traverse the list... just for debug purposes
     */
    foreach (QVariant tm, list)
    {
		//LOGGER::log("\n            list type: ", (int)tm.type());

		if (tm.type() == QVariant::Map)
		{
			QVariantMap toPass = tm.value<QVariantMap>();
			sortMap(toPass, level+1);
		}
		else if (tm.type() == QVariant::List)
		{
			QVariantList toPass = tm.value<QVariantList>();
			sortList(toPass, level+1);
		}
		else if (tm.type() == QVariant::String)
		{
			LOGGER::log("list value: ", tm.toString());
		}
		else
		{
			LOGGER::log("list type (no output on value): ", (int) tm.type());
		}
		LOGGER::log("\n info LIST level:      ", level);
    }
}

void sortMap(QVariantMap & map, int level)
{
	LOGGER::log("\n ========================= MAP level:      ", level);

	for(QVariantMap::const_iterator iter = map.begin(); iter != map.end(); ++iter) {

		QVariant k = iter.key();
		QString tt = k.toString();

		LOGGER::log("\nmap key: ", tt);
		LOGGER::log("map key type: ", (int)k.type());

		int yy = tt.length();

		QVariant m = iter.value();
		QString tt2 = m.toString();

		LOGGER::log("map value: ", tt2);
		LOGGER::log("map value type: ", (int)m.type());

		if (m.type() == QVariant::Map) {
			QVariantMap toPass = m.value<QVariantMap>();
			sortMap(toPass, level+1);
		}

		if (m.type() == QVariant::List) {
			QVariantList toPass = m.value<QVariantList>();
			//sortList(toPass, level+1);
		}
		LOGGER::log("\n info MAP level:      ", level);
	}
}

cAtomManager::~cAtomManager() {
}

const char* cAtomManager::getString(atomFeed e) {
	// this method is not completed
	const char* result = atomId;
	switch (e)
	{
		case eAtomEntry:
				result = atomEntry;
			break;
		case eAtomLink:
				result = atomLink;
			break;

		default:
			break;
	}
	return result;
}

const char* cAtomManager::getString(entryType e) {
	// this method is not completed
	const char* result = entryProperties;
	switch (e)
	{
		case eAtomEntry:
				result = entryProperties;
			break;
		default:
			break;
	}
	return result;
}

QVariantMap cAtomManager::buildAtomElements(QByteArray& arrBytes) {

	// Then we have XML
	XmlDataAccess xda;
	QVariantMap ret;
	QVariant xmlContent = xda.loadFromBuffer(arrBytes);

	LOGGER::log("Byte Array : ", QString(arrBytes));

	if (xda.hasError()) {
		DataAccessError error = xda.error();
		LOGGER::log("failed to load featured list XML: ", error.errorType());
		LOGGER::log("failed to load featured list XML: ", error.errorMessage());

		return QVariantMap();
	}

	if (xmlContent.type() != QVariant::Map)
	{
		LOGGER::log("failed to parse byte array into QVarianMap: ");
		LOGGER::log("qvariant is of type : ", (int)xmlContent.type());
		return QVariantMap();
	}

	// We have a Map
	return xmlContent.value<QVariantMap>();
}

cAtomManager::atomType cAtomManager::findType(QVariantMap& map) {

	QVariant vtObject = map[".root"];

	if ((vtObject.type() == QVariant::String) && (vtObject.toString().contains("service")))
			return eAtomService;

	return eAtomFeed;
}

QVariantList cAtomManager::parseAtomElements(QByteArray& arrBytes, QVariantMap& mapEntries, atomType eType) {

	QVariantList result;

	switch (eType)
	{
		case eAtomFeed:
			{
				// TO DO: Should review code for other cases
			    const char* tmp = getString(eAtomEntry);
				result = mapEntries[tmp].value<QVariantList>();
			}
			break;
		case eAtomService:
			{
				// TO DO: Should review code for other cases
				QVariantMap subResult = mapEntries["workspace"].value<QVariantMap>();
				result = subResult["collection"].value<QVariantList>();
			}
			break;

		default:
			break;
	}
	return result;
}

int cAtomManager::parseElementsforDataModel(QVariantList& entries, bb::cascades::ArrayDataModel& dt, atomType eAtomType, entryType eEntryType) {
	QVariantList listForQML;
	switch (eAtomType)
	{
		case eAtomFeed:
			{
				const char* type = getString(eEntryType);
				QVariantMap toInsertInList;

				foreach (QVariant vt, entries)
				{
					if (vt.type() == QVariant::Map)
					{
						QVariantMap variantMap = vt.value<QVariantMap>();
						QVariantMap variantPropertiesMap = variantMap[type].value<QVariantMap>(); // used to be  "m:properties"

						for(QVariantMap::const_iterator iter = variantPropertiesMap.begin(); iter != variantPropertiesMap.end(); ++iter) {
							QVariant k = iter.key();
							QVariant tmp;

							// TO DO: insert a const char "d:" might need some review
							if (k.type() == QVariant::String)
								if (k.toString().startsWith("d:"))
									tmp = k.toString().remove("d:");
								else
									tmp = k.toString();

							// assuming here that all keys are strings
							toInsertInList.insert(tmp.toString(), iter.value());
						}

						listForQML.append(toInsertInList);
						toInsertInList.clear();
					}
				}
				// add the data to the model
				dt.append(listForQML);
			}
		case eAtomService:
			{
				int sz = entries.size();
				const char* type = "atom:title";


				sortList(entries, 1);

				foreach (QVariant vt, entries)
				{
					if (vt.type() == QVariant::Map)
					{
						QVariantMap variantMap = vt.value<QVariantMap>();
						QVariant vtTmp = variantMap[type];

						if (vtTmp.type() == QVariant::String) {
							listForQML.append(vtTmp);
						}
					}
				}
				// add the data to the model
				dt.append(listForQML);
			}
	}

	return listForQML.size();
}

int cAtomManager::fillDataModelItems(QByteArray& result, ArrayDataModel& dataModel) {
    QVariantMap mapEntries = buildAtomElements(result);
    cAtomManager::atomType tp = findType(mapEntries);

    QVariantList parsed = cAtomManager::Instance().parseAtomElements(result, mapEntries, tp);
    return parseElementsforDataModel(parsed, dataModel, tp, cAtomManager::eProperties);
}

