/*
 * cAtomManager.cpp
 *
 *  Created on: 2013-03-18
 *      Author: cmartin
 */
#include <QDateTime>
#include <QList>
#include <bb/data/XmlDataAccess>

#include "Atom_constants.h"
#include "cAtomManager.h"
#include "cProperty.h"

#include "LOGGER.h"

// Incomplete const char* declarations
const char* atomEntry = "entry";
const char* atomLink = "link";
const char* atomId = "id";
const char* atomODataDocumentTypeKey = "xmlns:";

// Incomplete const char* declarations
const char* entryProperties = "m:properties";

using namespace bb::cascades;
using namespace bb::data;


cAtomManager::~cAtomManager() {
}

QVariantMap cAtomManager::buildAtomElements(const QByteArray& arrBytes) {

	// Then we have XML
	XmlDataAccess xda;
	QVariantMap ret;
	QVariant xmlContent = xda.loadFromBuffer(arrBytes);

	// LOGGER::log("Byte Array : ", QString(arrBytes));

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

bool cAtomManager::isNamespaceForApp(QVariantMap& map) {
	QVariant vtObject = map[atomODataDocumentTypeKey];
	QString s = vtObject.value<QString>();

	return s.contains(OData_app_link, Qt::CaseInsensitive);
}

bool cAtomManager::isNamespaceForAtom(QVariantMap& map) {
	QVariant vtObject = map[atomODataDocumentTypeKey];
	QString s = vtObject.value<QString>();

	return s.contains(OData_atom_link, Qt::CaseInsensitive);
}

cAtomManager::atomDocumentType cAtomManager::findType(QVariantMap& map) {

	// Two sources of information where used for this method
	// 1- OData.org Atom specs file. see link
	//       [1] http://www.odata.org/media/30002/OData%20Atom%20Format.html#primitivetypesinatom
	// 2- Datajs library source code. see link
	//       [2] http://datajs.codeplex.com/SourceControl/changeset/view/23844#16631
	//    it is the odata-atom.js file
	//
	//   ----------
	//   Nota Bene:
	//   ----------
	//   Note that the OData Atom format is more complex the original described Atom XML.
	//		As described in the link above [1]:
	//    	OData builds on RFC4287 and RFC5023 by defining additional conventions and extensions for representing and querying entity data.

	// XmlDataAccess object creates a ".root" entry at the top
	QVariant vtObject = map[".root"];

	// =============================================================================================
	// According to datajs library, it first determine if there is a namespace defined called
	// appXmlNs. This refers to "http://www.w3.org/2007/app". It also look for "atomXmlNs" which is
	// "http://www.w3.org/2005/Atom".
	// =============================================================================================

	// ==============================================================================================
	// In section 13 of the link above [1]
	// Atom defines the concept of a Service Document to represent the set of available collections.
	// OData uses Service Documents to describe the set of entity sets available through the service.
    // So this call attempts to evaluate this
	// Note that "service" is a reserved word in OData Atom XML

	// Addendum info: we validate as well for the namespace  "http://www.w3.org/2007/app"..
	if (isNamespaceForApp(map))
		if ((vtObject.type() == QVariant::String) && (vtObject.toString().contains("service")))
			return eAtomService;

	// Otherwise, we are facing a collection of entries called "feed" or a simple "entry"
	// In section 5 of the link above [1]

	// [2] According to datajs library; the method atomReadDocument, we need to check for
	//     "http://www.w3.org/2005/Atom". Then validate if we have a feed or an entry
	if (isNamespaceForAtom(map))
		return eAtomFeed;

	// TODO:we should have: if (some validation ...) return eAtomEntry;

	return eAtomEntry;
}

int cAtomManager::fillDataModelItems(QByteArray& result, ArrayDataModel& dataModel) {

	QVariantMap mapEntries = buildAtomElements(result);
	cAtomManager::atomDocumentType tp = findType(mapEntries);

	QVariantList parsed = parseToODataAtomElements(result, mapEntries, tp);

	return parseElementsforDataModel(parsed, dataModel, tp, cAtomManager::eProperties);
}

QVariantList cAtomManager::parseToODataAtomElements(QByteArray& arrBytes, QVariantMap& mapEntries, atomDocumentType eType) {

	QVariantList result;

	switch (eType)
	{
		case eAtomFeed:
			{
				// TODO: Should review code for other cases
			    const char* tmp = getString(eFeedEntry);
				result = mapEntries[tmp].value<QVariantList>();
			}
			break;
		case eAtomService:
			{
				// TODO: Should review code for other cases
				QVariantMap subResult = mapEntries["workspace"].value<QVariantMap>();
				result = subResult["collection"].value<QVariantList>();
			}
			break;

		case eAtomEntry:
				// TODO: Should review code for other cases
				// Need to implement this one
			break;

		default:

			break;
	}

	return result;
}

int cAtomManager::parseElementsforDataModel(QVariantList& entries, bb::cascades::ArrayDataModel& dt, atomDocumentType eAtomDocType, entryType eEntryType) {
	QVariantList listForQML;
	switch (eAtomDocType)
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

							// "d" refers to a prefix commonly used for namespacing.
							// It is defined in the Atom_constants as OData_ns_dataservices_Prefix
							// See documentation
							// http://www.odata.org/documentation/atom-format
							// 2.3. Representing Collections of Entries
							QString toCompare = QString(OData_ns_dataservices_Prefix) + QString(":");
							if (k.type() == QVariant::String)
								if (k.toString().startsWith(toCompare))
									tmp = k.toString().remove(toCompare);
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
			break;
		case eAtomService:
			{
				int sz = entries.size();
				const char* type = "atom:title";
				//sortList(entries, 1);

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
			break;
	}

	return listForQML.size();
}

const char* cAtomManager::getString(atomFeed e) {
	// this method is not completed
	const char* result = atomId;
	switch (e)
	{
		case eFeedEntry:
				result = atomEntry;
			break;
		case eFeedLink:
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

void cAtomManager::setHeaders(QNetworkRequest& request, const QByteArray& body) {
    // set the raw headers
    // QT has a definition for Content-Type... but not for the others...
    request.setRawHeader(HttpRequestHeader_Content_Type, "application/atom+xml"); // ATOM_Resource_Content_Type);  ///);
    // TODO: remove hard coding
    request.setRawHeader(HttpRequestHeader_Accept, "application/atom+xml"); // should it be ==> ATOM_Resource_Accept); //);
    request.setRawHeader(HttpRequestHeader_UserAgent, "BB10 ODataLib v0.1");
    request.setRawHeader(HttpRequestHeader_DataService, Resource_DataServiceVersion_1);
    request.setRawHeader(HttpRequestHeader_MaxDataService, Resource_MaxDataServiceVersion);
    request.setRawHeader(HttpRequestHeader_Host, "services.odata.org");

    QByteArray postDataSize = QByteArray::number(body.size());
    request.setRawHeader(HttpRequestHeader_ContentLength, postDataSize);
}

QVariant cAtomManager::buildPropertyList(const QByteArray& arrMetadata, const QString& sSchema, const QString& sEntityType) {

	QVariantList resultList;

	QVariantMap mapEntries = buildAtomElements(arrMetadata);

    // see line [2] above
    QVariant vt_tmp = mapEntries["edmx:DataServices"];
    QVariantMap vt_map = vt_tmp.value<QVariantMap>();

    // see line [3] above (this can be a list of different schema)
    vt_tmp = vt_map["Schema"];

    // if there is more than one schema
    if (vt_tmp.type() == QVariant::List)  {
    	QVariantList schemas = vt_tmp.value<QVariantList>();

    	// TODO: handle multiple schemas
    	// For the mean time, we will stick to the first schema
    	vt_tmp = schemas.at(0);
    }

	{
		if (vt_tmp.type() == QVariant::Map) {
			QVariantMap variantSchemaMap = vt_tmp.value<QVariantMap>();

			// see line [4] above
			vt_tmp = variantSchemaMap["EntityType"];

			// Somewhat puzzled here... "EntityType" returns a List...
			if (vt_tmp.type() == QVariant::List) {
				QVariantList entitiesList = vt_tmp.value<QVariantList>();

				// again for the purpose of the exercise, getting only the second "EntityType" Map
				// see line [2] above ...

				vt_tmp.clear();
				foreach (QVariant itm, entitiesList) {
					// Should be all type Map
					if (itm.type() == QVariant::Map) {

						QVariantMap vtMap = itm.value<QVariantMap>();
						QVariant v = vtMap["Name"];

						if (v.type() == QVariant::String) {
							if (v.toString().contains(sEntityType))
								vt_tmp = itm;
						}
					}
				}

				if (vt_tmp.isNull()) {
					LOGGER::log("Unable to find map for entity type");
					return resultList;
				}

				if (vt_tmp.type() == QVariant::Map) {
					QVariantMap variantEntityMap = vt_tmp.value<QVariantMap>();

					vt_tmp = variantEntityMap["Property"];
					QVariantList list_ofProps = vt_tmp.value<QVariantList>();

					cProperty* oProp;
					foreach (QVariant vt_prop, list_ofProps) {
						oProp = cProperty::createProperty(vt_prop);
						QVariant vAddToList = qVariantFromValue((void *) oProp);
						resultList.append(vAddToList);
					}
				}
			}
		}
	}
	return resultList;
}

QByteArray cAtomManager::buildXMLfromPropertyList(QVariantList& propertiesList, const QString& sCollectionName, const QString& sNamespace, const QString& sEntityName) {

	QByteArray arrBody;
	arrBody.append(createBodyHeader());

	// TODO: remove hard coding and add parameter for this
	arrBody.append(createEntryHeader("http://services.odata.org/OData/OData.svc/"));
	arrBody.append(createTitleForEntryHeader(sCollectionName));

	arrBody.append(createXMLwithTag("updated", getAtomXMLCurrentDateTime()));
	// TODO: add a parameter for this
	arrBody.append(createXMLwithTag("author", createXMLwithTag("name", "")));

	// We use Schema Namespace and EntityType Name to create the <category term> xml string
	//
	// So, for example, we have a $metadata xml like this one:
	//		[...]
	// 			<Schema Namespace="ODataDemo" xmlns:d="http://schemas.microsoft.com/ado/2007/08/dataservices" xmlns:m="http://schemas.microsoft.com/ado/2007/08/dataservices/metadata" xmlns="http://schemas.microsoft.com/ado/2008/09/edm">
	//  			<EntityType Name="Category">
	//
	// in this case, it would give you "ODataDemo.Category"

	// TODO: remove hard coding and add parameter for this
	// might have add a parameter to the for OData_MS_link_DataSvc_Scheme
	arrBody.append(createCategoryTermHeader(sNamespace + QString(".") + sEntityName, OData_MS_link_DataSvc_Scheme));
	arrBody.append(createContentTypeHeader("application/xml"));

	arrBody.append(createPropertyHeader());

	foreach (QVariant vt, propertiesList) {
	  cProperty* prop = (cProperty *) vt.value<void *>();
	  arrBody.append(prop->serialize());
	  arrBody.append(const_CR_LF);
	}

	arrBody.append(createClosureXMLwithTag(QString(OData_ns_metadata_Prefix) + QString(":") + QString("properties"), true));
	arrBody.append(createClosureXMLwithTag("content", true));
	arrBody.append(createClosureXMLwithTag("entry", true));

	return arrBody;
}

QString cAtomManager::getAtomXMLCurrentDateTime() {
	// according to Atom/XML specs see link:
	//
	//   http://tools.ietf.org/html/rfc4287
	//
	//	3.3.  Date Constructs
	//
	//	   A Date construct is an element whose content MUST conform to the
	//	   "date-time" production in [RFC3339].  In addition, an uppercase "T"
	//	   character MUST be used to separate date and time, and an uppercase
	//	   "Z" character MUST be present in the absence of a numeric time zone
	//	   offset.
	//

	QDateTime dt = QDateTime::currentDateTime();
	dt.setTimeSpec(Qt::UTC);
	//QDateTime local = dt.toLocalTime();

	QString sDateTime = dt.toString(Qt::ISODate); // YYYY-MM-DDTHH:MM:SSZ    // TODO add this SS+SSTZD

	// TODO review for time zone
	// Not clear how to get time zone info
	return sDateTime;
}

QByteArray cAtomManager::createBodyHeader() {
	// creates :
	// <?xml version="1.0" encoding="utf-8" standalone="yes"?>

	QByteArray arrByteBody;

	arrByteBody.append("<?xml version=\"1.0\" encoding=\"");
	arrByteBody.append(const_UTF_8);
	arrByteBody.append("\" standalone=\"yes\"?>");
	arrByteBody.append(const_CR_LF);

	return arrByteBody;
}

QByteArray cAtomManager::createEntryHeader(const QString& sBase) {
	// creates :
	//<entry xml:base="http://services.odata.org/OData/OData.svc/"
	//			xmlns:d="http://schemas.microsoft.com/ado/2007/08/dataservices"
	//			xmlns:m="http://schemas.microsoft.com/ado/2007/08/dataservices/metadata"
	//			xmlns="http://www.w3.org/2005/Atom">

	QByteArray arrByteBody;

	arrByteBody.append("<entry xml:base=\"");
	arrByteBody.append(sBase); // "http://services.odata.org/OData/OData.svc/");
	arrByteBody.append("\"");
	arrByteBody.append(const_CR_LF);

	arrByteBody.append(" xmlns");
	arrByteBody.append(":");
	arrByteBody.append(OData_ns_dataservices_Prefix);
	arrByteBody.append("=\"");
	arrByteBody.append(OData_MS_link_DataSvc);
	arrByteBody.append("\"");
	arrByteBody.append(const_CR_LF);

	arrByteBody.append(" xmlns");
	arrByteBody.append(":");
	arrByteBody.append(OData_ns_metadata_Prefix);
	arrByteBody.append("=\"");
	arrByteBody.append(OData_MS_link_DataSvc_Meta);
	arrByteBody.append("\"");
	arrByteBody.append(const_CR_LF);

	arrByteBody.append(" xmlns");
	arrByteBody.append("=\"");
	arrByteBody.append(OData_atom_link);
	arrByteBody.append("\"");
	arrByteBody.append(">");
	arrByteBody.append(const_CR_LF);

	return arrByteBody;
}

QByteArray cAtomManager::createClosureXMLwithTag(const QString& sTag, bool bAdd_CR_LF) {
	QByteArray arrByteBody;

	arrByteBody.append("</");
	arrByteBody.append(sTag);
	arrByteBody.append("> ");
	if (bAdd_CR_LF)
		arrByteBody.append(const_CR_LF);

	return arrByteBody;
}

QByteArray cAtomManager::createXMLwithTag(const QString& sTag, const QString& sValue) {
	QByteArray arrByteBody;

	// creates:
	arrByteBody.append("<");
	arrByteBody.append(sTag);
	arrByteBody.append(">");

	arrByteBody.append(sValue);

	arrByteBody.append(createClosureXMLwithTag(sTag));
	arrByteBody.append(const_CR_LF);

	return arrByteBody;
}

QByteArray cAtomManager::createTitleForEntryHeader(const QString& sTitle) {
	// creates:
	// <title type="text">some title</title>

	QByteArray arrByteBody;

	// creates:
	arrByteBody.append("<title type=\"text\">");
	arrByteBody.append(sTitle);
	arrByteBody.append("</title>");
	arrByteBody.append(const_CR_LF);

	return arrByteBody;
}


QByteArray cAtomManager::createCategoryTermHeader(const QString& sCategory, const QString& sSchema) {
	// creates:
	// <category term="ODataDemo.Category" scheme="http://schemas.microsoft.com/ado/2007/08/dataservices/scheme" />

	QByteArray arrByteBody;

	arrByteBody.append("<category term=");

	arrByteBody.append("\"");
	arrByteBody.append(sCategory);
	arrByteBody.append("\"");

	arrByteBody.append(" scheme=\"");
	arrByteBody.append(sSchema);
	arrByteBody.append("\" />");
	arrByteBody.append(const_CR_LF);

	return arrByteBody;
}

QByteArray cAtomManager::createContentTypeHeader(const QString& sContentType) {
	// creates:
	// <content type="application/xml">

	QByteArray arrByteBody;
	arrByteBody.append("<");
	arrByteBody.append(Entry_ContentType);
	arrByteBody.append("=");
	arrByteBody.append("\"");
	arrByteBody.append(sContentType);  // ATOM_Resource_Content_Type);
	arrByteBody.append("\"");

	arrByteBody.append(">");
	arrByteBody.append(const_CR_LF);

	return arrByteBody;
}

QByteArray cAtomManager::createPropertyHeader() {
	// creates:
	// <m:properties>

	QByteArray arrByteBody;

	arrByteBody.append("<");
	arrByteBody.append(OData_ns_metadata_Prefix);
	arrByteBody.append(":");
	arrByteBody.append("properties");
	arrByteBody.append(">");
	arrByteBody.append(const_CR_LF);

	return arrByteBody;
}
