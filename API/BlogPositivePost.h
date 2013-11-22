#pragma once
#include "BlogPositiveBlog.h"
#include <String.h>

class BlogPositivePost {
public:
  BlogPositivePost(BlogPositiveBlog *blog) {
    fName = new BString();
    fPage = new BString();
    fBlog = blog;
  }
  BlogPositiveBlog *Blog() {
    return fBlog;
  }
  const char *Name() {
    return fName->String();
  }
  void SetName(const char *name) {
    fName->SetTo(name);
  }
  
  const char *Page() {
    return fPage->String();
  }
  void SetPage(const char *page) {
    fPage->SetTo(page);
  }
  uint32 Pagesize() {
    return fPage->Length();
  }
 private:
  BString *fName;
  BString *fPage;
  BlogPositiveBlog *fBlog;
};