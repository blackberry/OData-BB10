/*
 * cProperty.h
 *
 *  Created on: 2013-03-27
 *      Author: cmartin
 */

#ifndef CPROPERTY_H_
#define CPROPERTY_H_

#include <QObject>
#include <bb/cascades/CustomControl>

#include <QString>
#include <QVariant>
#include <QByteArray>

class cProperty : public bb::cascades::CustomControl {
	Q_OBJECT

	Q_PROPERTY(int edmType READ edmType CONSTANT)
	Q_PROPERTY(bool nullable READ nullable CONSTANT)
	Q_PROPERTY(QString fieldName READ fieldName CONSTANT)
	Q_PROPERTY(QVariant value READ value WRITE setValue)

public:
	cProperty();
	virtual ~cProperty();

	// property for QML access
    int edmType() const;
    bool nullable() const;
    QString fieldName() const;
    QVariant value() const;

	void setValue(const QVariant& v) { m_value = v; };
	QByteArray serialize();

	static cProperty* createProperty(const QVariant& vt);

private:
	QVariant m_value;

	QString m_sEdmType;
	QString m_sMaxLength;
	QString m_sMinLength;
	QString m_sName;

	bool m_bFixedLength;
	bool m_bNullable;
	bool m_bUnicode;

	// See OData document (http://www.odata.org/documentation/atom-format) in section
	// 2.4.4. Customizing the Representation of an Entry for the latter
	QString m_sConcurrencyMode;
	QString m_sFC_TargetPath;
	bool m_bFC_KeepInContent;
	QString m_sFC_SourcePath;
	QString m_sFC_ContentKind;
	QString m_sFC_NsPrefix;
	QString m_sFC_NsUri;

	//-(id)initWithEdmType:(NSString *)anEdmType MaxLength:(NSString *)theMaxLength MinLength:(NSString *)theMinLength FixedLength:(bool)isFixedLength Nullable:(bool)isNullable
	//			 Unicode:(bool)isUnicode ConcurrencyMode:(NSString *)aConcurrencyMode FC_TargetPath:(NSString *)aFC_TargetPath FC_KeepInContent:(bool)isFC_KeepInContent
	//	   FC_SourcePath:(NSString *)aFC_SourcePath FC_ContentKind:(NSString *)aFC_ContentKind FC_NsPrefix:(NSString *)aFC_NsPrefix FC_NsUri:(NSString *)aFC_NsUri;
};
#endif /* CPROPERTY_H_ */
