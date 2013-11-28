/*
 * Copyright 2013 Puck Meerburg, puck@puckipedia.nl
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef BP_WP_PLUGIN_H
#define BP_WP_PLUGIN_H

#include "../../API/BlogPositivePlugin.h"
#include "../../API/BlogPositivePost.h"

class BlogPositiveBlog;
class WordpressPlugin : public BlogPositivePlugin
{
public:
    uint32 Version(); //kBlogPositiveBlogApi
    char *Name();
    int32 Type();
    uint32 MainHandler();
    bool Supports(int32 Code);
    BList *GetBlogPosts(BlogPositiveBlog *aBlog);
    void SavePost(BlogPositivePost *aPost);
    BlogPositivePost *CreateNewPost(BlogPositiveBlog *aBlog, const char *aName);
    void RemovePost(BlogPositivePost *aPost);
};

#endif