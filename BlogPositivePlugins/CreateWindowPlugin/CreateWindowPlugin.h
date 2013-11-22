#pragma once
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <OS.h>
#include "../../API/BlogPositivePluginWindow.h"
#include <ScrollView.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <ListView.h>
#include <TextView.h>
class CreateWindowPlugin;

class PostItem : public BStringItem {
public:
  inline PostItem(BlogPositivePost *post) : BStringItem(post->Name()) {
    fPost = post;
  }
  inline BlogPositivePost *Post() {
    return fPost;
  }
 private:
  BlogPositivePost *fPost;
};

class PluginAndWindowThing {
 public:
  BWindow *fWindow;
  BlogPositivePlugin *fPlugin;
  BlogPositiveBlog *fBlog;
  BListView *fView;
};

class CreateWindowPlugin : public BlogPositivePlugin {
 public:
  uint32 Version() {
    return 0;
  }
  inline int32 Type() {
    return kBlogPositiveBlogEditor;
  }
  inline bool Supports(int32 Code) {
    return false;
  }
  inline void HookBlogList(BlogPositivePluginPostListWindow **window, BlogPositiveBlog *blog) {
    BWindow *win = *window;
    BListView *aView = new BListView("ListView");
    BMessage *msg = new BMessage(kPostWindowGetSelection);
    aView->SetInvocationMessage(msg);
    win->SetLayout(new BGroupLayout(B_VERTICAL));
    win->AddChild(BGroupLayoutBuilder(B_VERTICAL, 10).Add(aView));
    PluginAndWindowThing *thing = new PluginAndWindowThing();
    thing->fWindow = *window;
    thing->fBlog = blog;
    thing->fView = aView;
    thing->fPlugin = blog->GetPlugin();
    thread_id readThread = spawn_thread(loadList, "load_posts", B_NORMAL_PRIORITY, thing);
    resume_thread(readThread);
  }
  inline BlogPositivePost *TryGetPost(BWindow *window) {
    BListView *aView = (BListView *)window->FindView("ListView");
    if(aView != NULL) {
      int32 ChosenOne = aView->CurrentSelection();
      if(ChosenOne == -1) {
	return NULL;
      }
      PostItem *item = (PostItem *)aView->ItemAt(ChosenOne);
      return item->Post();
    }

  }
  inline static int32 loadList(void *t) {
    PluginAndWindowThing *thing = (PluginAndWindowThing *)t;
    BList *list = thing->fPlugin->GetBlogPosts(thing->fBlog);
    if(thing->fView->LockLooper()) {
      for(int i = 0; i < list->CountItems(); i++) {
	BlogPositivePost *post = (BlogPositivePost *)list->ItemAt(i);
	thing->fView->AddItem(new PostItem(post));
      }
      thing->fView->UnlockLooper();
    }
  }
  inline void HookEditor(BlogPositivePluginBlogPostWindow **window, BlogPositivePost *post) {
    BlogPositivePluginBlogPostWindow *win = *window;
    BTextView *aView = new BTextView("TextView");
    aView->SetText(post->Page());
    win->SetLayout(new BGroupLayout(B_VERTICAL));
    win->AddChild(BGroupLayoutBuilder(B_VERTICAL, 10).Add(aView));
    BRect r(win->Bounds());
    r.bottom = 20;
    BMenuBar *menuBar = new BMenuBar(r, "MenuBar");
    win->AddChild(menuBar);
    BMenu *menu = new BMenu("File");
    menu->AddItem(new BMenuItem("Save", new BMessage(kPostWindowSavePost), 'S'));
    menuBar->AddItem(menu);
  }
  inline const char *GetPostContent(BlogPositivePluginBlogPostWindow *window) {
    BView *view = window->FindView("TextView");
    BTextView *textView = (BTextView *)view;
    return textView->Text();
  }
};