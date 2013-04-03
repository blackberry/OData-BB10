/*
 * Atom_constants.h
 *
 *  Created on: 2013-03-20
 *      Author: cmartin
 */
#ifndef ATOM_CONSTANTS_H_
#define ATOM_CONSTANTS_H_

//////////////////////////////////////
//constants for HTTPRequestHeader
extern const char* HttpRequestHeader_Accept;
extern const char* HttpRequestHeader_AcceptCharset;
extern const char* HttpRequestHeader_AcceptEncoding;
extern const char* HttpRequestHeader_AcceptLanguage;
extern const char* HttpRequestHeader_Allow;
extern const char* HttpRequestHeader_Authorization;
extern const char* HttpRequestHeader_CacheControl;
extern const char* HttpRequestHeader_Connection;
extern const char* HttpRequestHeader_ContentDisposition;
extern const char* HttpRequestHeader_ContentLength;
extern const char* HttpRequestHeader_Content_Type;
extern const char* HttpRequestHeader_ContentEncoding;
extern const char* HttpRequestHeader_ContentLanguage;
extern const char* HttpRequestHeader_ContentLocation;
extern const char* HttpRequestHeader_ContentMd5;
extern const char* HttpRequestHeader_ContentRange;
extern const char* HttpRequestHeader_Cookie;
extern const char* HttpRequestHeader_Date;
extern const char* HttpRequestHeader_Expect;
extern const char* HttpRequestHeader_Expires;
extern const char* HttpRequestHeader_From1;
extern const char* HttpRequestHeader_Host;
extern const char* HttpRequestHeader_IfMatch;
extern const char* HttpRequestHeader_IfModifiedSince;
extern const char* HttpRequestHeader_IfNoneMatch;
extern const char* HttpRequestHeader_IfRange;
extern const char* HttpRequestHeader_IfUnmodifiedSince;
extern const char* HttpRequestHeader_KeepAlive;
extern const char* HttpRequestHeader_LastModified;
extern const char* HttpRequestHeader_Location;
extern const char* HttpRequestHeader_MaxForwards;
extern const char* HttpRequestHeader_Pragma;
extern const char* HttpRequestHeader_ProxyAuthorization;
extern const char* HttpRequestHeader_Referer;
extern const char* HttpRequestHeader_Range;
extern const char* HttpRequestHeader_Slug;
extern const char* HttpRequestHeader_Te;
extern const char* HttpRequestHeader_Translate;
extern const char* HttpRequestHeader_Trailer;
extern const char* HttpRequestHeader_TransferEncoding;
extern const char* HttpRequestHeader_Upgrade;
extern const char* HttpRequestHeader_UserAgent;
extern const char* HttpRequestHeader_Via;
extern const char* HttpRequestHeader_Warn;
extern const char* HttpRequestHeader_Warning;
extern const char* HttpRequestHeader_XHTTPMethod;

extern const char* HttpRequestHeader_DataService;
extern const char* HttpRequestHeader_MaxDataService;

/////////////////////////////////////
//constants for HTTPVerb
extern const char* HttpVerb_DELETE;
extern const char* HttpVerb_GET;
extern const char* HttpVerb_POST;
extern const char* HttpVerb_PUT;
extern const char* HttpVerb_MERGE;

/////////////////////////////////////
//constant for Atom  OData

extern const char* Entry_ContentType;

extern const char* OData_MS_link_DataSvc;
extern const char* OData_MS_link_DataSvc_Scheme;
extern const char* OData_MS_link_DataSvc_Meta;

extern const char* OData_atom_link;
extern const char* OData_app_link;
extern const char* OData_xml_link;

extern const char* EDM2006_Link ;
extern const char* EDM2007_Link ;
extern const char* EDM2008_1_Link ;
extern const char* EDM2008_9_Link ;
extern const char* EDM2009_Link  ;
extern const char* EDMX_Link ;
extern const char* EDMANNOTATION_Link ;

/////////////////////////////////////

extern const char* OData_ns_dataservices_Prefix;
extern const char* OData_ns_metadata_Prefix;

/////////////////////////////////////

//definition of all erro messages thrown by context 	tracking and request generation logic
extern const char* Resource_AddInvalidObject;
extern const char* Resource_UpdateInvalidObject;
extern const char* Resource_DeleteInvalidObject;
extern const char* Resource_AddLinkInvalidObject;
extern const char* Resource_SetLinkInvalidObject;
extern const char* Resource_DeleteLink;
extern const char* Resource_LoadPropertyInvalidObject;
extern const char* Resource_InvalidObject;
extern const char* Resource_EntityAlreadyContained;
extern const char* Resource_EntityNotContained;

extern const char* Resource_NoRelationWithDeleteEnd;
extern const char* Resource_RelationAlreadyContained;
extern const char* Resource_NoRelationWithInsertEnd;
extern const char* Resource_RelationNotRefOrCollection;
extern const char* Resource_SetLinkReferenceOnly;
extern const char* Resource_AddLinkCollectionOnly;
extern const char* Resource_InCorrectLinking;
extern const char* Resource_CountNotPresent;
extern const char* Resource_MissingEditMediaLinkInResponseBody;
extern const char* Resource_ExpectedEmptyMediaLinkEntryContent;
extern const char* Resource_ExpectedValidHttpResponse;
extern const char* Resource_InvalidArgumentForGetStream;
extern const char* Resource_EntityNotMediaLinkEntry;
extern const char* Resource_SetSaveStreamWithoutEditMediaLink;
extern const char* Resource_LinkResourceInsertFailure;
extern const char* Resource_MLEWithoutSaveStream;
extern const char* Resource_ArgumentNotNull;
extern const char* Resource_NoLocationHeader;
extern const char* Resource_InvalidSaveChangesOptions;
extern const char* Resource_NullValueNotAllowedForKey;
extern const char* Resource_NoLoadWithInsertEnd;
extern const char* Resource_NoLoadWithUnknownProperty;
extern const char* Resource_AttachLocationFailedDescRetrieval;
extern const char* Resource_UnexpectdEntityState;
extern const char* Resource_InvalidEntityClassName;
extern const char* Resource_InvalidExecuteArg;
extern const char* Resource_NoEmptyQueryOption;
extern const char* Resource_ReservedCharNotAllowed;
extern const char* Resource_NoDuplicateOption;
extern const char* Resource_NoCountAndInLineCount;
extern const char* Resource_CollectionNotBelongsToQueryResponse;
extern const char* Resource_FCTargetPathMissing;
extern const char* Resource_FCKeepInContentMissing;
extern const char* Resource_FCContentKindMissing;
extern const char* Resource_FCNsPrefixMissing;
extern const char* Resource_FCNsUriMissing;
extern const char* Resource_EntityHeaderCannotAppy;
extern const char* Resource_EntityHeaderOnlyArray;


//definition for data service specific headers
extern const char* Resource_MaxDataServiceVersion;
extern const char* Resource_DefaultDataServiceVersion;
extern const char* Resource_DataServiceVersion_1;
extern const char* Resource_DataServiceVersion_2;

extern const char* const_UTF_8;
extern const char* const_cISO_8859_1;
extern const char* const_ISO_8859_15 ;

extern const char* const_lower_UTF_8;
extern const char* const_lower_ISO_8859_1;
extern const char* const_lower_ISO_8859_15;

extern const char* const_CR_LF;

//definition of possible Accept and Content-Types headers
extern const char* ATOM_Resource_Accept;
extern const char* ATOM_Resource_Content_Type;


// definition of primitive Types in Atom
extern const char* EDM;
extern const char* EDM_BINARY;
extern const char* EDM_BOOLEAN;
extern const char* EDM_BYTE;
extern const char* EDM_DATETIME;
extern const char* EDM_DATETIMEOFFSET;
extern const char* EDM_DECIMAL;
extern const char* EDM_DOUBLE;
extern const char* EDM_GUID;
extern const char* EDM_INT16;
extern const char* EDM_INT32;
extern const char* EDM_INT64;
extern const char* EDM_SBYTE;
extern const char* EDM_SINGLE;
extern const char* EDM_STRING;
extern const char* EDM_TIME;

extern const char* EDM_GEOGRAPHY;
extern const char* EDM_GEOGRAPHY_POINT;
extern const char* EDM_GEOGRAPHY_LINESTRING;
extern const char* EDM_GEOGRAPHY_POLYGON;
extern const char* EDM_GEOGRAPHY_COLLECTION;
extern const char* EDM_GEOGRAPHY_MULTIPOLYGON;
extern const char* EDM_GEOGRAPHY_MULTILINESTRING;
extern const char* EDM_GEOGRAPHY_MULTIPOINT;

extern const char* EDM_GEOMETRY;
extern const char* EDM_GEOMETRY_POINT;
extern const char* EDM_GEOMETRY_LINESTRING;
extern const char* EDM_GEOMETRY_POLYGON;
extern const char* EDM_GEOMETRY_COLLECTION;
extern const char* EDM_GEOMETRY_MULTIPOLYGON;
extern const char* EDM_GEOMETRY_MULTILINESTRING;
extern const char* EDM_GEOMETRY_MULTIPOINT;

/*
extern const char* GEOJSON_POINT = "Point";
extern const char* GEOJSON_LINESTRING = "LineString";
extern const char* GEOJSON_POLYGON = "Polygon";
extern const char* GEOJSON_MULTIPOINT = "MultiPoint";
extern const char* GEOJSON_MULTILINESTRING = "MultiLineString";
extern const char* GEOJSON_MULTIPOLYGON = "MultiPolygon";
extern const char* GEOJSON_GEOMETRYCOLLECTION = "GeometryCollection";
*/

// definition of Property keywords
extern const char* PROPERTY_NAME;
extern const char* PROPERTY_TYPE;
extern const char* PROPERTY_MAXLENGTH;
extern const char* PROPERTY_MINLENGTH;
extern const char* PROPERTY_FIXEDLENGTH;
extern const char* PROPERTY_NULLABLE;
extern const char* PROPERTY_UNICODE;
extern const char* PROPERTY_CONCURRENCYMODE;
extern const char* PROPERTY_FC_TARGETPATH;
extern const char* PROPERTY_FC_KEEPINCONTENT;
extern const char* PROPERTY_FC_SOURCEPATH;
extern const char* PROPERTY_FC_CONTENTKIND;
extern const char* PROPERTY_FC_NSPREFIX;
extern const char* PROPERTY_FC_NSURI;

//error message for version mismatch
extern const char* Resource_VersionMisMatch;

#endif /* ATOM_CONSTANTS_H_ */
