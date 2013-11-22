#pragma once
#include <Message.h>
#include <List.h>
#include <stdio.h>
#include <String.h>

class BlogPositivePlugin;
const uint32 kBlogMessage = 'BPBL';
class BlogPositiveBlog {
public:
	BlogPositiveBlog() {
		fName = new BString();
		fAuthentication = new BString();
	}
	static BList *DeserializeList(BMessage *message, const char *blogName) {
		BMessage msg;
		BList *Listy = new BList();
		int i = 0;
		printf("Deserializing list!\n");
		while(message->FindMessage(blogName, i++, &msg) == B_OK) {
			BlogPositiveBlog *blog = new BlogPositiveBlog();
			blog->Unserialize(&msg);
			printf("    Blog added! %s %s %d\n", blog->GetName(), blog->GetAuthentication(), blog->GetBlogHandler());
			Listy->AddItem(blog);
		}
		printf("Done!\n");
		return Listy;
	}
	static BMessage *SerializeList(BList *blist, const char *blogName) {
		BMessage *bm = new BMessage();
		printf("Serializing! %s\n", blogName);
		bm->AddString("temp", blogName);
		printf("count: %d\n", blist->CountItems());
		blist->DoForEach((bool (*)(void *, void *))BlogPositiveBlog::SerializeOneItem, bm);
		bm->RemoveName("temp");
		printf("Done!\n");
		return bm;
	}
	static bool SerializeOneItem(BlogPositiveBlog *dingA, BMessage *bmessage) {
		const char *blogName = bmessage->GetString("temp", "");
		printf("Serializing one! %s %s %s %d\n", blogName, dingA->GetName(), dingA->GetAuthentication(), dingA->GetBlogHandler());
		bmessage->AddMessage(blogName, dingA->Serialize());
		return false;
	}
	inline void SetName(const char *name) {
		printf("Set name %s to %s\n", fName->String(), name);
		fName->SetTo(name);
	}
	inline const char *GetName() {
		return fName->String();
	}
	inline void SetAuthentication(const char *authentication) {
		printf("Set auth %s to %s\n", fAuthentication->String(), authentication);
		fAuthentication->SetTo(authentication);
	}
	inline const char *GetAuthentication() {
		return fAuthentication->String();
	}
	inline void SetBlogHandler(int32 blogHandler) {
		fBlogHandler = blogHandler;
	}
	inline int32 GetBlogHandler() {
		return fBlogHandler;
	}
	inline BMessage *Serialize() {
		BMessage *message = new BMessage();
		message->what = kBlogMessage;
		message->AddString("name", fName->String());
		message->AddString("auth", fAuthentication->String());
		message->AddInt32("handler", fBlogHandler);
		return message;
	}
	inline void Unserialize(BMessage *message) {
		fName->SetTo(message->GetString("name", ""));
		fAuthentication->SetTo(message->GetString("auth", ""));
		fBlogHandler = message->GetInt32("handler", 0);
	}
	inline void SetPlugin(BlogPositivePlugin *plugin) {
		fPlugin = plugin;
	}
	inline BlogPositivePlugin *GetPlugin() {
		return fPlugin;
	}
private:
	BString *fName;
	BString *fAuthentication;
	int32 fBlogHandler;
	BlogPositivePlugin *fPlugin;
};