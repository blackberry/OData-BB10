/*
 * cProperty.cpp
 *
 *  Created on: 2013-03-27
 *      Author: cmartin
 */

#include "Atom_constants.h"
#include "cProperty.h"
#include "LOGGER.h"


void logMapXtra(QVariantMap & map, int level);

void logListXtra(QVariantList & list, int level)
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
			//if (level < g_Depth)
				logMapXtra(toPass, level+1);
		}
		else if (vt.type() == QVariant::List)
		{
			QVariantList toPass = vt.value<QVariantList>();
			//if (level < g_Depth)
				logListXtra(toPass, level+1);
		}
		else if (vt.type() == QVariant::String)
		{
			LOGGER::log_indent("list value : ", vt.toString(), level);
		}
		else
		{
			LOGGER::log_indent("** list type : ", (int) vt.type(), level); // TODO
		}
    }
	LOGGER::log_indent("exiting LIST level:  ", level, level);
}

void logMapXtra(QVariantMap & map, int level)
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

			//if (level < g_Depth)
			logMapXtra(toPass, level+1);
		}

		if (vt_val.type() == QVariant::List) {
			QVariantList toPass = vt_val.value<QVariantList>();
			//if (level < g_Depth)
				logListXtra(toPass, level+1);
		}
		else if (vt_val.type() == QVariant::String)
		{
			LOGGER::log_indent("** map value : ", vt_val.toString(), level); // TODO
		}
		else
		{
			LOGGER::log_indent("map type : ", (int) vt_val.type(), level);
		}
	}
	LOGGER::log_indent("exiting MAP level:  ", level, level);
}

cProperty::cProperty() {
	// TODO Auto-generated constructor stub
	m_bFixedLength = false;
	m_bNullable = false;
	m_bUnicode = false;
	m_bFC_KeepInContent = false;
}

cProperty::~cProperty() {
	// TODO Auto-generated destructor stub
}

int cProperty::edmType() const {
	return 0;
}

bool cProperty::nullable() const {
	return m_bNullable;
}

QString cProperty::fieldName() const {
	return m_sName;
}

QVariant cProperty::value() const {
	return m_value;
}

cProperty* cProperty::createProperty(const QVariant& vt){

	cProperty* oNewObj = new cProperty();
	if (vt.type() == QVariant::Map)
	{
		QVariantMap map = vt.value<QVariantMap>();

		// We could use keys to directly access data but to make sure we cover it all
		// we use this approach

		// TODO
		// Code re-write like this would be better
		//
		// QVariant v = map[PROPERTY_TYPE];
		// oNewObj->m_sEdmType = v.toString();
		// ...etc...

		int nAcknowledged = 0;
		for(QVariantMap::const_iterator iter = map.begin(); iter != map.end(); ++iter) {
			QVariant vt_key = iter.key();

			if (vt_key.toString().contains(PROPERTY_TYPE)) {
				oNewObj->m_sEdmType = iter.value().toString();
				nAcknowledged++;
			}
			else if (vt_key.toString().contains(PROPERTY_NAME)) {
				oNewObj->m_sName = iter.value().toString();
				nAcknowledged++;
			}
			else if (vt_key.toString().contains(PROPERTY_MAXLENGTH)) {
				oNewObj->m_sMaxLength = iter.value().toString();
				nAcknowledged++;
			}
			else if (vt_key.toString().contains(PROPERTY_MINLENGTH)) {
				oNewObj->m_sMinLength = iter.value().toString(); //v.toString();
				nAcknowledged++;
			}
			else if (vt_key.toString().contains(PROPERTY_FIXEDLENGTH)) {
				QVariant v = map[PROPERTY_FIXEDLENGTH];
				if (v.toString().contains("true"))
					oNewObj->m_bFixedLength = true;
				nAcknowledged++;
			}
			else if (vt_key.toString().contains(PROPERTY_NULLABLE)) {
				QVariant v = map[PROPERTY_NULLABLE];
				if (v.toString().contains("true"))
					oNewObj->m_bNullable = true;
				nAcknowledged++;
			}
			else if (vt_key.toString().contains(PROPERTY_UNICODE)) {
				QVariant v = map[PROPERTY_UNICODE];
				if (v.toString().contains("true"))
					oNewObj->m_bUnicode = true;
				nAcknowledged++;
			}
			// TODO : how do we handle case like this :
			// [1] <Property xmlns:p8="http://schemas.microsoft.com/ado/2009/02/edm/annotation"
			// [2] p8:StoreGeneratedPattern="Identity"/>
			else if (vt_key.toString().contains(PROPERTY_CONCURRENCYMODE)) {
				oNewObj->m_sConcurrencyMode = iter.value().toString();
				nAcknowledged++;
			}
			else if (vt_key.toString().contains(PROPERTY_FC_TARGETPATH)) {
				oNewObj->m_sFC_TargetPath = iter.value().toString();
				nAcknowledged++;
			}
			else if (vt_key.toString().contains(PROPERTY_FC_KEEPINCONTENT)) {
				QVariant v = map[PROPERTY_FC_KEEPINCONTENT];
				if (v.toString().contains("true"))
					oNewObj->m_bFC_KeepInContent = true;
				nAcknowledged++;
			}
			else if (vt_key.toString().contains(PROPERTY_FC_SOURCEPATH)) {
				oNewObj-> m_sFC_SourcePath = iter.value().toString();
				nAcknowledged++;
			}
			else if (vt_key.toString().contains(PROPERTY_FC_CONTENTKIND)) {
				oNewObj->m_sFC_ContentKind = iter.value().toString();
				nAcknowledged++;
			}
			else if (vt_key.toString().contains(PROPERTY_FC_NSPREFIX)) {
				oNewObj->m_sFC_NsPrefix = iter.value().toString();
				nAcknowledged++;
			}
			else if (vt_key.toString().contains(PROPERTY_FC_NSURI)) {
				oNewObj->m_sFC_NsUri = iter.value().toString();
				nAcknowledged++;
			}
			else {
				LOGGER::log("cProperty::createProperty - unmatched pair", vt_key.toString());
			}
		}
		if (nAcknowledged != map.size())
            LOGGER::log("cProperty::createProperty - cProperty object creation from mao failed - not all pairs were matched");

		// logMapXtra(map, 1);
	}
	return oNewObj;
}

QByteArray cProperty::serialize() {

	QByteArray arrByteBody;
	// entry body
	// e.g. <d:ID
	arrByteBody.append("<"); arrByteBody.append(OData_ns_dataservices_Prefix); arrByteBody.append(":"); arrByteBody.append(m_sName);

	// e.g.  m:type=
	// TODO "Type" in metadata is changed to "type" in xml CRUD operation. Is this correct ?
	arrByteBody.append(" "); arrByteBody.append(OData_ns_metadata_Prefix); arrByteBody.append(":"); arrByteBody.append("type"); arrByteBody.append("=");

	// e.g. "Edm.Int32">
	arrByteBody.append("\""); arrByteBody.append(m_sEdmType); arrByteBody.append("\""); arrByteBody.append(">");

	if (m_value.isNull()) {
		if (m_bNullable) {
			arrByteBody.append(OData_ns_metadata_Prefix); arrByteBody.append(":null="); arrByteBody.append("\"true\"");
		}
	}
	else {
		// e.g. 99
		arrByteBody.append(m_value.toString());
		// e.g. </d:ID
		arrByteBody.append("<"); arrByteBody.append("/"); arrByteBody.append(OData_ns_dataservices_Prefix); arrByteBody.append(":"); arrByteBody.append(m_sName);
	}
	arrByteBody.append(">");

	// TODO: Must handle other cases ??
	return arrByteBody;
}

