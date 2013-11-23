#include "BlogPositiveMainView.h"

#include <TextView.h>
#include <ListView.h>
#include <GroupLayoutBuilder.h>
#include <ScrollView.h>
#include <List.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <stdio.h>

#include "../API/BlogPositiveBlog.h"
#include "../BlogPositiveSettings.h"
#include "BlogPositiveBlogListItem.h"
#include "../API/BlogPositivePlugin.h"
#include "../API/BlogPositivePluginLoader.h"

const int32 kBlogSelected = 'BPBS';

class BlogPositiveBlogListView : public BListView {
public:
    BlogPositiveBlogListView()
	: BListView("BlogPositiveBlogListView", B_SINGLE_SELECTION_LIST)
    {
	
    }
    void Reload(BList *BlogItemList) {
	BlogItemList->DoForEach(&BlogPositiveBlogListView::createItem, this);
    }
    static bool createItem(void *item, void *p) {
	BlogPositiveBlogListItem *i = new BlogPositiveBlogListItem((BlogPositiveBlog *)item);
	((BListView *)p)->AddItem((BListItem *)i);
	return false;
    }
};

void
BlogPositiveMainView::MessageReceived(BMessage *message)
{
    if(message->what == kBlogSelected) {
	BlogPositivePluginLoader::LoadWindow(
	    ((BlogPositiveBlogListItem *)fListView->ItemAt(fListView->CurrentSelection()))->Blog());
	return;
    }
    if(message->what == 'ABCD') {
	int32 m = message->GetInt32("ding", 0);
	BList *pluginList = BlogPositivePluginLoader::fList;
	BlogPositivePlugin *pl = (BlogPositivePlugin *)pluginList->ItemAt(m);
	pl->OpenNewBlogWindow();
	return;
    }
    BView::MessageReceived(message);
}

BlogPositiveMainView::BlogPositiveMainView(const char *name)
    : BView(name, 0)
{
    
    BMessage *invocationMessage = new BMessage(kBlogSelected);
    BlogPositiveBlogListView *aView;
    BScrollView *sView;
    BRect aRect( Bounds() );
    aView = new BlogPositiveBlogListView();
    aView->SetInvocationMessage(invocationMessage);
    fListView = aView;
    
    aRect.bottom = 20;
    BMenuBar *menuBar = new BMenuBar("MenuBar");
    BList *pluginList = BlogPositivePluginLoader::fList;
    BlogPositiveSettings *settings = new BlogPositiveSettings("bloglist");
    BList *lis = BlogPositiveBlog::DeserializeList(settings, "blogs");
    aView->Reload(lis);
    for(int i = 0; i < pluginList->CountItems(); i++) {
	BlogPositivePlugin *pl = (BlogPositivePlugin *)pluginList->ItemAt(i);
	if(pl->Type() == kBlogPositiveBlogApi) {
	    BMessage *msg = new BMessage('ABCD');
	    msg->SetInt32("ding", i);
	    BMenuItem *mi = new BMenuItem(pl->Name(), msg);
	    menuBar->AddItem(mi);
	}
    }	
    SetLayout(new BGroupLayout(B_VERTICAL));
    AddChild(
	BGroupLayoutBuilder(B_VERTICAL, 0)
	.Add(menuBar)
	.Add(new BScrollView("scroll_view", aView, 0, false, true)));
}
