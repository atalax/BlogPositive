NAME = BlogPositive
TYPE = APP
APP_MIME_SIG = x.vnd.blogpositive
SRCS =	API/BlogPositiveBlog.cpp API/BlogPositivePlugin.cpp \
	API/BlogPositivePluginLoader.cpp \
	API/BlogPositivePost.cpp \
	BlogPositiveMain/BlogPositiveBlogListItem.cpp BlogPositiveMain/BlogPositiveMainView.cpp \
	BlogPositiveMain/BlogPositiveMainWindow.cpp BlogPositiveSharedWindow.cpp \
	BlogPositivePlugins/WordpressPlugin/WordpressPlugin.cpp \
	BlogPositivePlugins/BloggerPlugin/BloggerPlugin.cpp \
	BlogPositivePlugins/LiveJournal/LiveJournalPlugin.cpp \
	BlogPositivePostEditor/BlogPositivePostEditorView.cpp \
	BlogPositivePostEditor/BlogPositivePostEditorWindow.cpp \
	BlogPositivePostList/BlogPositivePostListView.cpp \
	BlogPositivePostList/BlogPositivePostListWindow.cpp \
	BlogPositiveSettings.cpp Example/ExamplePlugin.cpp \
	Modules/JSON/Json.cpp Modules/XML/xmlnode.cpp \
	Modules/XMLRPC/XmlRpcWrapper.cpp main.cpp

LIBS = be root curl network z textencoding localestub $(STDCPPLIBS)
RDEFS = haiku.rdef
LOCALES = en nl
SYSTEM_INCLUDE_PATHS = /system/develop/headers/curl
## include the makefile-engine
DEVEL_DIRECTORY := \
	$(shell findpaths -r "makefile_engine" B_FIND_PATH_DEVELOP_DIRECTORY)
include $(DEVEL_DIRECTORY)/etc/makefile-engine
