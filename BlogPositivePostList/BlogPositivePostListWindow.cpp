#include "BlogPositivePostListWindow.h"

#include <Rect.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <String.h>

#include "../API/BlogPositiveBlog.h"
#include "BlogPositivePostListView.h"

BlogPositivePostListWindow::BlogPositivePostListWindow(
    BlogPositiveBlog *blog, BRect rect)
    : BWindow(rect, (BString("Blog: ") << blog->Name()).String(), B_TITLED_WINDOW, B_AUTO_UPDATE_SIZE_LIMITS)
{
    fListView = new BlogPositivePostListView("ListView", blog);
    
    SetLayout(new BGroupLayout(B_VERTICAL));
    AddChild(
	BGroupLayoutBuilder(B_VERTICAL, 0)
	.Add(fListView));
}
