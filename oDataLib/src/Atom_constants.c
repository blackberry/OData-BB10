/*
 * Atom_constants.c
 *
 *  Created on: 2013-03-21
 *      Author: cmartin
 */


// ************************* NOTE *******************************************
// ** This code was taken from ODataLib for iOS orginally written Objective-c
//
//    All this can be reviewed....
//
//    Not much taught was put into except cut/paste for quick access to constants
//

//////////////////////////////////////
//constants for HTTPRequestHeader
const char* HttpRequestHeader_Accept					= "Accept";
const char* HttpRequestHeader_AcceptCharset				= "Accept-Charset";
const char* HttpRequestHeader_AcceptEncoding			= "Accept-Encoding";
const char* HttpRequestHeader_AcceptLanguage			= "Accept-Language";
const char* HttpRequestHeader_Allow						= "Allow";
const char* HttpRequestHeader_Authorization				= "Authorization";
const char* HttpRequestHeader_CacheControl				= "Cache-Control";
const char* HttpRequestHeader_Connection				= "Connection";
const char* HttpRequestHeader_ContentDisposition		= "Content-Disposition";
const char* HttpRequestHeader_ContentLength				= "Content-Length";
const char* HttpRequestHeader_Content_Type				= "Content-Type";
const char* HttpRequestHeader_ContentEncoding			= "Content-Encoding";
const char* HttpRequestHeader_ContentLanguage			= "Content-Language";
const char* HttpRequestHeader_ContentLocation			= "Content-Location";
const char* HttpRequestHeader_ContentMd5				= "Content-MD5";
const char* HttpRequestHeader_ContentRange				= "Content-Range";
const char* HttpRequestHeader_Cookie					= "Cookie";
const char* HttpRequestHeader_Date						= "Date";
const char* HttpRequestHeader_Expect					= "Expect";
const char* HttpRequestHeader_Expires					= "Expires";
const char* HttpRequestHeader_From1						= "From";
const char* HttpRequestHeader_Host						= "Host";
const char* HttpRequestHeader_IfMatch					= "If-Match";
const char* HttpRequestHeader_IfModifiedSince			= "If-Modified-Since";
const char* HttpRequestHeader_IfNoneMatch				= "If-None-Match";
const char* HttpRequestHeader_IfRange					= "If-Range";
const char* HttpRequestHeader_IfUnmodifiedSince			= "If-Unmodified-Since";
const char* HttpRequestHeader_KeepAlive					= "";
const char* HttpRequestHeader_LastModified				= "Last-Modified";
const char* HttpRequestHeader_Location					= "Location";
const char* HttpRequestHeader_MaxForwards				= "Max-Forwards";
const char* HttpRequestHeader_Pragma					= "Pragma";
const char* HttpRequestHeader_ProxyAuthorization		= "Proxy-Authorization";
const char* HttpRequestHeader_Referer					= "Referer";
const char* HttpRequestHeader_Range						= "Range";
const char* HttpRequestHeader_Slug						= "Slug";
const char* HttpRequestHeader_Te						= "TE";
const char* HttpRequestHeader_Translate					= "Translate";
const char* HttpRequestHeader_Trailer					= "Trailer";
const char* HttpRequestHeader_TransferEncoding			= "Transfer-Encoding";
const char* HttpRequestHeader_Upgrade					= "Upgrade";
const char* HttpRequestHeader_UserAgent					= "User-Agent";
const char* HttpRequestHeader_Via						= "Via";
const char* HttpRequestHeader_Warn						= "Warn";
const char* HttpRequestHeader_Warning					= "Warning";
const char* HttpRequestHeader_XHTTPMethod				= "X-HTTP-Method";

const char* HttpRequestHeader_DataService				= "DataServiceVersion";
const char* HttpRequestHeader_MaxDataService			= "MaxDataServiceVersion";

/////////////////////////////////////
//constants for HTTPVerb
const char* HttpVerb_DELETE								= "DELETE";
const char* HttpVerb_GET								= "GET";
const char* HttpVerb_POST								= "POST";
const char* HttpVerb_PUT								= "PUT";
const char* HttpVerb_MERGE								= "MERGE";

/////////////////////////////////////
//constant for Atom  OData

const char* Entry_ContentType							= "content type";

const char* OData_MS_link_DataSvc						= "http://schemas.microsoft.com/ado/2007/08/dataservices";
const char* OData_MS_link_DataSvc_Scheme				= "http://schemas.microsoft.com/ado/2007/08/dataservices/scheme";
const char* OData_MS_link_DataSvc_Meta					= "http://schemas.microsoft.com/ado/2007/08/dataservices/metadata";

const char* OData_atom_link								= "http://www.w3.org/2005/Atom";
const char* OData_app_link								= "http://www.w3.org/2007/App";
const char* OData_xml_link								= "http://www.w3.org/XML/1998/namespace";

const char* EDM2006_Link 								= "http://schemas.microsoft.com/ado/2006/04/edm"; // edm 1.0
const char* EDM2007_Link 								= "http://schemas.microsoft.com/ado/2007/05/edm"; // edm 1.1
const char* EDM2008_1_Link 								= "http://schemas.microsoft.com/ado/2008/01/edm"; // edm 1.2
const char* EDM2008_9_Link 								= "http://schemas.microsoft.com/ado/2008/09/edm"; // edm 2.0
const char* EDM2009_Link  								= "http://schemas.microsoft.com/ado/2009/08/edm"; // edm ???
const char* EDMX_Link 									= "http://schemas.microsoft.com/ado/2007/06/edmx";
const char* EDMANNOTATION_Link 							= "http://schemas.microsoft.com/ado/2009/02/edm/annotation";

/////////////////////////////////////

const char* OData_Prefix								= "d";
const char* OData_Meta_Prefix							= "m";

/////////////////////////////////////

//definition of all erro messages thrown by context tracking and request generation logic
const char* Resource_AddInvalidObject					= "Trying to Add Invalid Object";
const char* Resource_UpdateInvalidObject				= "Trying to Update Invalid Object";
const char* Resource_DeleteInvalidObject				= "Trying to Delete Invalid Object";
const char* Resource_AddLinkInvalidObject				= "Trying to Add Link between Objects, where one or both objects are invalid";
const char* Resource_SetLinkInvalidObject				= "Trying to Set Link between Objects, where one or both objects are invalid";
const char* Resource_DeleteLink							= "Trying to Delete Link between Objects, where one or both objects are invalid";
const char* Resource_LoadPropertyInvalidObject			= "Trying to call LoadProperty in an invalid object";
const char* Resource_InvalidObject						= "Object is not valid";
const char* Resource_EntityAlreadyContained				= "The context is already tracking the entity.";
const char* Resource_EntityNotContained					= "The context is not currently tracking the entity.";

const char* Resource_NoRelationWithDeleteEnd			= "One or both of the ends of the relationship is in the deleted state.";
const char* Resource_RelationAlreadyContained			= "The context is already tracking the relationship.";
const char* Resource_NoRelationWithInsertEnd			= "One or both of the ends of the relationship is in the added state.";
const char* Resource_RelationNotRefOrCollection			= "The sourceProperty is not a reference or collection of the target's object type.";
const char* Resource_SetLinkReferenceOnly				= "Incorrect Linking. SetLink method only works when the relationship with the sourceProperty is one to one.";
const char* Resource_AddLinkCollectionOnly				= "Incorrect Linking. AddLink and DeleteLink methods only work when relationship with the sourceProperty is a one to many.";
const char* Resource_InCorrectLinking					= "Incorrect Linking. AddLink will work only when relationship is one to many and setlink will work only when relationship is one to one";
const char* Resource_CountNotPresent					= "Count value is not part of the response stream";
const char* Resource_MissingEditMediaLinkInResponseBody	= "Error processing response stream. Missing href attribute in the edit-Media link element in the response";
const char* Resource_ExpectedEmptyMediaLinkEntryContent	= "Error processing response stream. The ATOM content element is expected to be empty if it has a source attribute.";
const char* Resource_ExpectedValidHttpResponse			= "DataServiceStreamResponse constructor requires valid HttpResponse object";
const char* Resource_InvalidArgumentForGetStream		= "Second argument of GetStream API should be null, string or object of type DataServiceRequestArgs";
const char* Resource_EntityNotMediaLinkEntry			= "This operation requires the specified entity to represent a Media Link Entry";
const char* Resource_SetSaveStreamWithoutEditMediaLink	= "The binary property on the entity cannot be modified as a stream because the corresponding entry in the response does not have an edit-media link. Ensure that the entity has a binary property that is accessible as a stream in the data model.";
const char* Resource_LinkResourceInsertFailure			= "One of the link\'s resources failed to insert.";
const char* Resource_MLEWithoutSaveStream				= "Media Link Entry, but no save stream was set for the entity";
const char* Resource_ArgumentNotNull					= " Argument cannot be null";
const char* Resource_NoLocationHeader					= "The response to this POST request did not contain a \'location\' header. That is not supported by this client.";
const char* Resource_InvalidSaveChangesOptions			= "The specified SaveChangesOptions is not valid, use SaveChangesOptions::Batch or SaveChangesOptions::None";
const char* Resource_NullValueNotAllowedForKey			= "The serialized resource has a null value in key member, Null values are not supported in key members - [Check you have set value of any key memeber to null by mistake or if you have used select query option, make sure this member variable is selected] Key Name: ";
const char* Resource_NoLoadWithInsertEnd				= "The context cannot load the related collection or reference for objects in the added state";
const char* Resource_NoLoadWithUnknownProperty			= "The context cannot load the related collection or reference to the unknown property - ";
const char* Resource_AttachLocationFailedDescRetrieval	= "Internal Error: AttachLocation Failed to retrieve the descriptor";
const char* Resource_UnexpectdEntityState				= "Unexpected Entity State while trying to generate changeset body for resource";
const char* Resource_InvalidEntityClassName				= "Failed to find entity class with name -";
const char* Resource_InvalidExecuteArg					= "Execute API receives only uri or DataServiceQueryContinuation";
const char* Resource_NoEmptyQueryOption					= "Error in DataService Query: Can\'t add empty Query option";
const char* Resource_ReservedCharNotAllowed				= "Error in DataService Query: Can\'t add query option because it begins with reserved character \'$\' - ";
const char* Resource_NoDuplicateOption					= "Error in DataService Query: Can\'t add duplicate query option - ";
const char* Resource_NoCountAndInLineCount				= "Cannot add count option to the resource set because it would conflict with existing count options";
const char* Resource_CollectionNotBelongsToQueryResponse = "GetContinuation API: The collection is not belonging to the QueryOperationResponse";
const char* Resource_FCTargetPathMissing				= "Invalid Proxy File failed to retrieve \'FC_TargetPath\' for the property - ";
const char* Resource_FCKeepInContentMissing				= "Invalid Proxy File failed to retrieve \'FC_KeepInContent\' for the property - ";
const char* Resource_FCContentKindMissing				= "Invalid Proxy File failed to retrieve \'FC_ContentKind\' for the property - ";
const char* Resource_FCNsPrefixMissing					= "Invalid Proxy File failed to retrieve \'FC_NsPrefix\' for the property - ";
const char* Resource_FCNsUriMissing						= "Invalid Proxy File failed to retrieve \'FC_NsUri\' for the property - ";
const char* Resource_EntityHeaderCannotAppy				= "Entity header can be applied only if entity is in added or modified state";
const char* Resource_EntityHeaderOnlyArray				= "Second argument to SetEntityHeader must be an array";


//definition for data service specific headers
const char* Resource_MaxDataServiceVersion				= "2.0";
const char* Resource_DefaultDataServiceVersion			= "1.0";
const char* Resource_DataServiceVersion_1				= "1.0";
const char* Resource_DataServiceVersion_2				= "2.0";

const char* const_UTF_8 								= "UTF-8";
const char* const_cISO_8859_1 							= "ISO-8859-1"; // latin1
const char* const_ISO_8859_15 							= "ISO-8859-15"; // latin9

const char* const_lower_UTF_8 							= "utf-8";
const char* const_lower_ISO_8859_1 						= "iso-8859-1";
const char* const_lower_ISO_8859_15 					= "iso-8859-15";

const char* const_CR_LF								= "\r\n";

//definition of possible Accept and Content-Types headers
const char* ATOM_Resource_Accept						= "application/atom+xml,application/xml";
const char* ATOM_Resource_Content_Type					= "application/atom+xml,application/xml";

//error message for version mismatch
const char* Resource_VersionMisMatch					= "Response version mismatch. Client Library Expect version 2.0, but service returns response with version ";
