/*
 * cAtomManager.cpp
 *
 *  Created on: 2013-03-18
 *      Author: cmartin
 */
#include <QDateTime>
#include <bb/data/XmlDataAccess>

#include "Atom_constants.h"
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

void logMap(QVariantMap & map, int level);

void logList(QVariantList & list, int level)
{
	// add space to make the level indented
	LOGGER::log_indent("LIST level:  ", level, level);

	int sz = list.size();

    /*
     *  Traverse the list... just for debug purposes
     */
    foreach (QVariant vt, list)
    {
		if (vt.type() == QVariant::Map)
		{
			QVariantMap toPass = vt.value<QVariantMap>();
			logMap(toPass, level+1);
		}
		else if (vt.type() == QVariant::List)
		{
			QVariantList toPass = vt.value<QVariantList>();
			logList(toPass, level+1);
		}
		else if (vt.type() == QVariant::String)
		{
			LOGGER::log_indent("list value                    : ", vt.toString(), level);
		}
		else
		{
			LOGGER::log_indent("list type (no output on value): ", (int) vt.type(), level);
		}
    }
	LOGGER::log_indent("exiting LIST level:  ", level, level);
}

void logMap(QVariantMap & map, int level)
{
	LOGGER::log_indent("MAP level:  ", level, level);

	int sz = map.size();

	for(QVariantMap::const_iterator iter = map.begin(); iter != map.end(); ++iter) {

		QVariant vt_key = iter.key();
		QString vt_string = vt_key.toString();

		LOGGER::log_indent("map key       : ", vt_string, level);
		LOGGER::log_indent("map key type  : ", (int)vt_key.type(), level);

		QVariant vt_val = iter.value();
		QString tvt_string2 = vt_val.toString();

		LOGGER::log_indent("map value     : ", tvt_string2, level);
		LOGGER::log_indent("map value type: ", (int)vt_val.type(), level);

		if (vt_val.type() == QVariant::Map) {
			QVariantMap toPass = vt_val.value<QVariantMap>();
			logMap(toPass, level+1);
		}

		if (vt_val.type() == QVariant::List) {
			QVariantList toPass = vt_val.value<QVariantList>();
			logList(toPass, level+1);
		}
	}
	LOGGER::log_indent("exiting MAP level:  ", level, level);
}

cAtomManager::~cAtomManager() {
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

cAtomManager::atomDocumentType cAtomManager::findType(QVariantMap& map) {

	// Two sources of information where used for this method
	// 1- OData.org Atom specs file. see link
	//       [1] http://www.odata.org/media/30002/OData%20Atom%20Format.html#primitivetypesinatom
	// 2- Datajs library source code. see link
	//       [2] http://datajs.codeplex.com/SourceControl/changeset/view/23844#16631
	//    it is the odata-atom.js file
	//
	//   --------------
	//   Addendum info:
	//   --------------
	//   Note that the OData Atom format is more complex the original described Atom XML.
	//		As described in the link above [1]:
	//    	OData builds on RFC4287 and RFC5023 by defining additional conventions and extensions for representing and querying entity data.

	// XmlDataAccess object creates a ".root" entry at the top
	QVariant vtObject = map[".root"];


	// According to datajs library, it first determine if there is a namespace defined called
	// appXmlNs. This refers to "http://www.w3.org/2007/app". It also look for "atomXmlNs" which is
	// "http://www.w3.org/2005/Atom". In this code, this is not handle (yet).


	// ==============================================================================================
	// In section 13 of the link above [1]
	// Atom defines the concept of a Service Document to represent the set of available collections.
	// OData uses Service Documents to describe the set of entity sets available through the service.
    // So this call attempts to evaluate this
	// Note that "service" is a reserved word in OData Atom XML

	// TO DO: Addendum info: we could validate as well for the namespace  "http://www.w3.org/2007/app"..
	if ((vtObject.type() == QVariant::String) && (vtObject.toString().contains("service")))
			return eAtomService;

	// Otherwise, we are facing a collection of entries called "feed" or a simple "entry"
	// In section 5 of the link above [1]
	// [2] According to datajs library; the method atomReadDocument, we need to check for
	//     "http://www.w3.org/2005/Atom". Then validate if we have a feed or an entry

	// we should have if (some validation ...) return eAtomEntry;

	return eAtomFeed;
}

QVariantList cAtomManager::parseAtomElements(QByteArray& arrBytes, QVariantMap& mapEntries, atomDocumentType eType) {

	QVariantList result;

	switch (eType)
	{
		case eAtomFeed:
			{
				// TO DO: Should review code for other cases
			    const char* tmp = getString(eFeedEntry);
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
	}

	logList(listForQML, 1);

	return listForQML.size();
}

int cAtomManager::fillDataModelItems(QByteArray& result, ArrayDataModel& dataModel) {

    QVariantMap mapEntries = buildAtomElements(result);
    cAtomManager::atomDocumentType tp = findType(mapEntries);

    QVariantList parsed = parseAtomElements(result, mapEntries, tp);

    return parseElementsforDataModel(parsed, dataModel, tp, cAtomManager::eProperties);
}

QString cAtomManager::getAtomXMLUpdatedDateTime() {
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

	QString sDateTime = dt.toString(Qt::ISODate); // YYYY-MM-DDTHH:MM:SSZ    // TO DO add this SS+SSTZD

	// TO DO review for time zone
	// Not clear how to get time zone info
	return sDateTime;
}

QByteArray cAtomManager::createHTTP_request_test(const char* strHeaderType) {

	QByteArray arrByteBody;
	if(strcmp(strHeaderType, "PUT") == 0)
	{

		// Trying to fill XML as shown below here
		/*
		<?xml version="1.0" encoding="utf-8" standalone="yes"?>
			<entry xml:base="http://services.odata.org/OData/OData.svc/"
    		xmlns:d="http://schemas.microsoft.com/ado/2007/08/dataservices"
    		xmlns:m="http://schemas.microsoft.com/ado/2007/08/dataservices/metadata"
    		xmlns="http://www.w3.org/2005/Atom">
  	  	  	  <title type="text" />
  	  	  	  	  <updated>2013-03-22T19:24:36Z</updated>
    				<author>
      	  	  	  	  <name />
    				</author>
    				<category term="ODataDemo.Category" scheme="http://schemas.microsoft.com/ado/2007/08/dataservices/scheme" />
    				<content type="application/xml">
      	  	  	  	  <m:properties>
        				<d:Name>no Food</d:Name>
      			  	  </m:properties>
    			</content>
		</entry>*/


		// ... into this arrByteBody
		// entry body
		arrByteBody.append("<?xml version=\"1.0\" encoding=\"");
		arrByteBody.append(const_UTF_8);
		arrByteBody.append("\" standalone=\"yes\"?>"); // .. not sure if standalone="yes" is essential

		// is this right ?
		arrByteBody.append(const_CR_LF);

		arrByteBody.append("<entry xml:base=\"");
		arrByteBody.append("http://services.odata.org/OData/OData.svc/");
		arrByteBody.append("\"");
		arrByteBody.append(const_CR_LF);

		arrByteBody.append("   xmlns");
		arrByteBody.append(":");
		arrByteBody.append(OData_Prefix);
		arrByteBody.append("=\"");
		arrByteBody.append(OData_MS_link_DataSvc);
		arrByteBody.append("\"");
		arrByteBody.append(const_CR_LF);

		arrByteBody.append("   xmlns");
		arrByteBody.append(":");
		arrByteBody.append(OData_Meta_Prefix);
		arrByteBody.append("=\"");
		arrByteBody.append(OData_MS_link_DataSvc_Meta);
		arrByteBody.append("\"");
		arrByteBody.append(const_CR_LF);

		arrByteBody.append("   xmlns");
		arrByteBody.append("=\"");
		arrByteBody.append(OData_atom_link);
		arrByteBody.append("\"");
		arrByteBody.append(">");
		arrByteBody.append(const_CR_LF);

		arrByteBody.append("   <title type=\"text\" />");
		arrByteBody.append(const_CR_LF);

		arrByteBody.append("   <updated>");

		// Should be using this method for date time
		QString sDtTm = getAtomXMLUpdatedDateTime();

		arrByteBody.append("2013-03-21T19:17:52Z");   //  arrByteBody.append(sDtTm);
		arrByteBody.append("</updated>");
		arrByteBody.append(const_CR_LF);

		// *********** Shud be parameterized ???? ******************
		arrByteBody.append("   <author>");
		arrByteBody.append(const_CR_LF);
		arrByteBody.append("     <name />");
		arrByteBody.append(const_CR_LF);

		arrByteBody.append("   </author>");
		arrByteBody.append(const_CR_LF);

		arrByteBody.append("   <category term=");

		// *********** Shud be parameterized ******************
		arrByteBody.append("\"ODataDemo.Category\"");

		arrByteBody.append(" scheme=\"");
		arrByteBody.append(OData_MS_link_DataSvc_Scheme);
		arrByteBody.append("\" />");
		arrByteBody.append(const_CR_LF);

		arrByteBody.append("   <");
		arrByteBody.append(Entry_ContentType);
		arrByteBody.append("=");
		arrByteBody.append("\"");
		arrByteBody.append("application/xml");         //ATOM_Resource_Content_Type);
		arrByteBody.append("\"");
		arrByteBody.append(">");
		arrByteBody.append(const_CR_LF);

		arrByteBody.append("   <");
		arrByteBody.append(OData_Meta_Prefix);

		// *********** Shud be parameterized ******************
		arrByteBody.append(":properties>");
		arrByteBody.append(const_CR_LF);

/*		arrByteBody.append("      <");
		arrByteBody.append(OData_Prefix);

		// *********** Shud be parameterized ******************
		arrByteBody.append(":");
		arrByteBody.append("ID ");
		arrByteBody.append(OData_Meta_Prefix);
		arrByteBody.append(":");
		arrByteBody.append("type=");
		arrByteBody.append("\"");
		arrByteBody.append("Edm.Int32");
		arrByteBody.append("\"");

		arrByteBody.append(">");

		// *********** Shud be parameterized ******************
		arrByteBody.append("14");

		arrByteBody.append("</");
		arrByteBody.append(OData_Prefix);
		arrByteBody.append(":");
		arrByteBody.append("ID");
		arrByteBody.append(">");
		arrByteBody.append(const_CR_LF);*/

		arrByteBody.append("      <");
		arrByteBody.append(OData_Prefix);
		arrByteBody.append(":");
		arrByteBody.append("Name");
		arrByteBody.append(">");

		// All this code to change this property !!??!!
		arrByteBody.append("No more food");

		arrByteBody.append("<");
		arrByteBody.append("/");
		arrByteBody.append(OData_Prefix);
		arrByteBody.append(":");
		arrByteBody.append("Name");
		arrByteBody.append(">");
		arrByteBody.append(const_CR_LF);

		arrByteBody.append("   </");
		arrByteBody.append(OData_Meta_Prefix);
		arrByteBody.append(":properties>");
		arrByteBody.append(const_CR_LF);

		arrByteBody.append(" </content>");
		arrByteBody.append(const_CR_LF);
		arrByteBody.append("</entry>");

		return arrByteBody;
	}
}

