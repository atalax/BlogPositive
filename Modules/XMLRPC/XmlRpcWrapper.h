/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */
#ifndef XML_RPC_WRAPPER_H
#define XML_RPC_WRAPPER_H


#include <String.h>
#include <ObjectList.h>

class BaseXmlValue
{
public:
	virtual void					PushContent(BString* string) {}
};

class XmlValue : public BaseXmlValue
{
public:
	void							PushContent(BString* string);
									XmlValue(BString string, BString aBoxType = "string");
									XmlValue(int fAs, BString aBoxType = "i4");
									XmlValue(double fDs, BString aBoxType = "string");
	BString							Value();
private:
	BString							fValue;
	BString							boxType;
};

class XmlArray : public BaseXmlValue
{
public:
	void							PushContent(BString* string);
									XmlArray();
	void							AddItem(BaseXmlValue* xmlValue);
	int32							Items();
	BaseXmlValue*					ItemAt(int32 itemLocation);
private:
	BObjectList<BaseXmlValue>*		fList;
};

class XmlNameValuePair : public BaseXmlValue
{
public:
									XmlNameValuePair() {}
									XmlNameValuePair(BString name,
										BaseXmlValue* value);
									XmlNameValuePair(BString name,
										BString value);
	void							PushContent(BString* string);
	BString							Name();
	BaseXmlValue*					Value();
private:
	BString 						fName;
	BaseXmlValue*					fValue;
};

class XmlStruct : public BaseXmlValue
{
public:
	void							PushContent(BString* string);
									XmlStruct();
	void 							AddItem(XmlNameValuePair* pair);
	void							AddItem(BString name, BString value);
	int32							Items();
	XmlNameValuePair*				ItemAt(int32 i);
	BaseXmlValue*					Get(BString name);
private:
	BObjectList<XmlNameValuePair>*	fList;
};

class XmlRpcRequest
{
public:
									XmlRpcRequest();
	void							AddItem(BaseXmlValue* val);
	BString							GetData();
	const char*						MethodName();
	void							SetMethodName(const char* name);
	int32							Items();
	BaseXmlValue*					ItemAt(int32 itemIndex);
private:
	BString							fMethodName;
	BObjectList<BaseXmlValue>*		fXmlValues;
};

class XmlNode;
XmlArray* ParseResponse(XmlNode* baseNode);

#endif
