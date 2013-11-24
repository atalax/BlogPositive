#include "BlogPositiveMainView.h"

#include <TextView.h>
#include <ListView.h>
#include <GroupLayoutBuilder.h>
#include <ScrollView.h>
#include <List.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <stdio.h>
#include <Point.h>
#include <PopUpMenu.h>
#include <String.h>
#include <Rect.h>

#include "../API/BlogPositiveBlog.h"
#include "../BlogPositiveSettings.h"
#include "../BlogPositivePostList/BlogPositivePostListWindow.h"
#include "BlogPositiveBlogListItem.h"
#include "../API/BlogPositivePlugin.h"
#include "../API/BlogPositivePluginLoader.h"

const int32 kBlogSelected = 'BPBS';
const int32 kCreateNewBlog = 'BPCB';
const int32 kRemoveCurrentBlog = 'BPRC';
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
    switch(message->what)
    {
    case kRemoveCurrentBlog:
	RemoveBlog();
	break;
    case kBlogSelected:
    {
	if(fListView->CurrentSelection() == -1)
	    break;
	BlogPositiveBlog *aBlog =
	    ((BlogPositiveBlogListItem *)fListView->ItemAt(fListView->CurrentSelection()))->Blog();
	BlogPositivePluginLoader::FindPlugin(aBlog);
	BlogPositivePostListWindow *aWindow =
	    new BlogPositivePostListWindow(aBlog, BRect(100, 100, 400, 600));
	aWindow->Show();
	break;
    }
    case kCreateNewBlog:
    {
	int32 m = message->GetInt32("ding", 0);
	BList *pluginList = BlogPositivePluginLoader::fList;
	BlogPositivePlugin *pl = (BlogPositivePlugin *)pluginList->ItemAt(m);
	pl->OpenNewBlogWindow();
	break;
    }
    default:
	BView::MessageReceived(message);
    }
}

void
BlogPositiveMainView::RemoveBlog()
{
    if(fListView->CurrentSelection() == -1)
	return;
    BlogPositiveSettings *settings = new BlogPositiveSettings("bloglist"); 
    BList *lis = BlogPositiveBlog::DeserializeList(settings, "blogs");
    BlogPositiveBlog *blog = ((BlogPositiveBlogListItem *)fListView->ItemAt(fListView->CurrentSelection()))->Blog();
for(int i = 0; i < lis->CountItems(); i++)
{
    if(blog->NameString()->Compare(((BlogPositiveBlog *)lis->ItemAt(i))->Name()) == 0)
    {
	lis->RemoveItem(i);
	break;
    }
}
BlogPositiveSettings::SaveOther(BlogPositiveBlog::SerializeList(lis, "blogs"), "bloglist");

Reload(lis);

}
void
BlogPositiveMainView::Reload(BList *lis)
{
    fListView->MakeEmpty();
    fListView->Reload(lis);
}
void
BlogPositiveMainView::AttachedToWindow()
{
    fListView->SetTarget(this);
    for(int i = 0; i < fNewMenu->CountItems(); i++)
    {
	fNewMenu->ItemAt(i)->SetTarget(this);
    }
    fRemoveMenuItem->SetTarget(this);
}

BlogPositiveMainView::BlogPositiveMainView(const char *name)
    : BView(name, 0)
{
    fListView = new BlogPositiveBlogListView();
    fListView->SetInvocationMessage(new BMessage(kBlogSelected));
    
    fMenuBar = new BMenuBar("MenuBar");
    
    fNewMenu = new BMenu("New");
    fMenuBar->AddItem(fNewMenu);

    BList *pluginList = BlogPositivePluginLoader::fList;
    BlogPositiveSettings *settings = new BlogPositiveSettings("bloglist"); 
    
    BList *lis = BlogPositiveBlog::DeserializeList(settings, "blogs");
    fListView->Reload(lis);

    for(int i = 0; i < pluginList->CountItems(); i++)
    {
	BlogPositivePlugin *pl = (BlogPositivePlugin *)pluginList->ItemAt(i);
	if(pl->Type() == kBlogPositiveBlogApi)
	{
	    BMessage *msg = new BMessage(kCreateNewBlog);
	    msg->SetInt32("ding", i);	
	    msg->SetString("sendToView", Name());
	    BMenuItem *mi = new BMenuItem(pl->Name(), msg);
	    fNewMenu->AddItem(mi);
	}
    }
    
    BMessage *aMenuItemMessage = new BMessage(kRemoveCurrentBlog);
    fRemoveMenuItem = new BMenuItem("Remove blog", aMenuItemMessage);
    fMenuBar->AddItem(fRemoveMenuItem);

    SetLayout(new BGroupLayout(B_VERTICAL));
    AddChild(
	BGroupLayoutBuilder(B_VERTICAL, 0)
	.Add(fMenuBar)
	.Add(new BScrollView("scroll_view", fListView, 0, false, true)));
}