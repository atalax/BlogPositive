#pragma once
#include "BlogPositiveBlog.h"
#include "BlogPositivePost.h"
#include "BlogPositivePluginWindow.h"
#include <Window.h>
#include <List.h>
#include <TextControl.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <Message.h>
#include "../BlogPositiveSettings.h"
#define kBlogPositiveBlogApi 1
#define kBlogPositiveBlogEditor 2

class BlogPositiveCreateBlog : public BWindow {
 public:
 BlogPositiveCreateBlog()
   : BWindow(BRect(100, 100, 400, 190), "Create Blog", B_MODAL_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL,0) {
    fNameControl = new BTextControl("NameControl", "Name: ", "", new BMessage('CBFA'));
    fAuthControl = new BTextControl("AuthControl", "Auth: ", "", new BMessage('CBNB'));
    SetLayout(new BGroupLayout(B_VERTICAL));
    AddChild(BGroupLayoutBuilder(B_VERTICAL, 10).Add(fNameControl).Add(fAuthControl));

    fNameControl->MakeFocus();
    fBlogHandler = 'BACN';
  }
  void SetBlogHandler(int32 bg) {
    fBlogHandler = bg;
  }
  void MessageReceived(BMessage *message) {
    switch(message->what) {
    case 'CBFA':
      fAuthControl->MakeFocus();
      break;
    case 'CBNB': {
      BlogPositiveSettings *settings = new BlogPositiveSettings("bloglist");
      BList *lis = BlogPositiveBlog::DeserializeList(settings, "blogs");
      BlogPositiveBlog *blog = new BlogPositiveBlog();
      blog->SetName(fNameControl->Text());
      blog->SetAuthentication(fAuthControl->Text());
      blog->SetBlogHandler(fBlogHandler);
      lis->AddItem(blog);
      
      BlogPositiveSettings::SaveOther(BlogPositiveBlog::SerializeList(lis, "blogs"), "bloglist");
      Hide();
    }
      break;
    default:
      BWindow::MessageReceived(message);
    }
  }
 private:
  int32 fBlogHandler;
  BTextControl *fNameControl;
  BTextControl *fAuthControl;
};

class BlogPositivePlugin {
public:
  virtual uint32 Version() {return 0;}
  virtual char *Name() {return "Unknown"; }
  inline virtual int32 Type() {return kBlogPositiveBlogApi; }
  inline virtual bool Supports(int32 Code) {return false;}
  inline virtual BlogPositivePost *TryGetPost(BWindow *window) {return 0; }
  inline virtual BWindow *InitializeBlogWindow(BlogPositiveBlog *blog) {return 0;}
  inline virtual BList *GetBlogPosts(BlogPositiveBlog *blog) {return new BList();}
  inline virtual BlogPositivePost *CreateNewPost(BlogPositiveBlog *blog, const char *name) {return NULL;}
  inline virtual void HookBlogList(BlogPositivePluginPostListWindow **window, BlogPositiveBlog *blog) {}
  inline virtual void HookEditor(BlogPositivePluginBlogPostWindow **window, BlogPositivePost *post) {}
  inline virtual void SavePost(BlogPositivePost *post) {}
  virtual void OpenNewBlogWindow() {
    (new BlogPositiveCreateBlog())->Show();
  }
};