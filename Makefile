TOOL_NAME = xattrlist
xattrlist_FILES = xattrlist.c

ADDITIONAL_CFLAGS = -std=c99 -fomit-frame-pointer
OPTFLAG = -Os
TARGET_IPHONEOS_DEPLOYMENT_VERSION = 3.0

include framework/makefiles/common.mk
include framework/makefiles/tool.mk
