#include "ExamplePlugin.h"
#include <Window.h>
#include <Directory.h>
#include <TextView.h>
#include <GroupLayout.h>
#include <GroupLayoutBuilder.h>
#include <ListView.h>
#include <Entry.h>
#include <Path.h>
#include <ScrollView.h>
#include <malloc.h>
#include <File.h>
#include "../API/BlogPositivePost.h"

bool
ExamplePlugin::Supports(int32 Code) {
	return Code == 'BACN';
}
BList *
ExamplePlugin::GetBlogPosts(BlogPositiveBlog *blog) {
  BList *list = new BList();
  BDirectory Dir(blog->GetAuthentication());
  BEntry entry;
  
  while(Dir.GetNextEntry(&entry) == B_OK) {
    char name[B_FILE_NAME_LENGTH];
    entry.GetName(name);
    BFile file(&entry, B_READ_ONLY);
    off_t size;
    file.GetSize(&size);
    const char *pointer = (const char *)malloc(size);
    file.Read((void *)pointer, size);
    BlogPositivePost *post = new BlogPositivePost(blog);
    post->SetName((const char *)name);
    post->SetPage(pointer);
    list->AddItem(post);
    printf("%s\n",name);
  }
  return list;
}

void
ExamplePlugin::SavePost(BlogPositivePost *post) {
  BPath path(post->Blog()->GetAuthentication());
  path.Append(post->Name());
  BFile file(path.Path(), B_WRITE_ONLY | B_ERASE_FILE);
  file.Write((void *)post->Page(), post->Pagesize()+2);
}