meta:
	ADDON_NAME = ofxSquash
	ADDON_DESCRIPTION = Compress and decompress data using a large range of compression codecs supplied by libsquash
	ADDON_AUTHOR = Elliot Woods
	ADDON_URL = https://github.com/elliotwoods/ofxSquash

common:
	ADDON_INCLUDES = libs/squash/include
	ADDON_INCLUDES += src
	ADDON_LIBS_EXCLUDE = libs/squash/bin/%
