#include "httpd.h"


enum{
	MINET_HTML,	MINET_HTM,	MINET_TXT,	MINET_CSS,	MINET_ICO,	MINET_GIF,	MINET_JPG,	MINET_JPEG,
	MINET_PNG,	MINET_SVG,	MINET_TORRENT,	MINET_WAV,	MINET_MP3,	MINET_MID,	MINET_M3U,	MINET_RAM,
	MINET_RA,	MINET_DOC,	MINET_EXE,	MINET_ZIP,	MINET_XLS,	MINET_TGZ,	MINET_TARGZ,	MINET_TAR,
	MINET_GZ,	MINET_ARJ,	MINET_RAR,	MINET_RTF,	MINET_PDF,	MINET_SWF,	MINET_MPG,	MINET_MPEG,
	MINET_ASF,	MINET_AVI,	MINET_BMP
};

struct mine_type
builtin_mime_types[] = {
	{"html",		MINET_HTML,	4,	"text/html"			},
	{"htm",		MINET_HTM,	3,	"text/html"			},
	{"txt",		MINET_TXT,	3,	"text/plain"			},
	{"css",		MINET_CSS,	3,	"text/css"			},
	{"ico",		MINET_ICO,	3,	"image/x-icon"			},
	{"gif",		MINET_GIF,	3,	"image/gif"			},
	{"jpg",		MINET_JPG,	3,	"image/jpeg"			},
	{"jpeg",		MINET_JPEG,	4,	"image/jpeg"			},
	{"png",		MINET_PNG,	3,	"image/png"			},
	{"svg",		MINET_SVG,	3,	"image/svg+xml"			},
	{"torrent",	MINET_TORRENT,	7,	"application/x-bittorrent"	},
	{"wav",		MINET_WAV,	3,	"audio/x-wav"			},
	{"mp3",		MINET_MP3,	3,	"audio/x-mp3"			},
	{"mid",		MINET_MID,	3,	"audio/mid"			},
	{"m3u",		MINET_M3U,	3,	"audio/x-mpegurl"		},
	{"ram",		MINET_RAM,	3,	"audio/x-pn-realaudio"		},
	{"ra",		MINET_RA,	2,	"audio/x-pn-realaudio"		},
	{"doc",		MINET_DOC,	3,	"application/msword",		},
	{"exe",		MINET_EXE,	3,	"application/octet-stream"	},
	{"zip",		MINET_ZIP,	3,	"application/x-zip-compressed"	},
	{"xls",		MINET_XLS,	3,	"application/excel"		},
	{"tgz",		MINET_TGZ,	3,	"application/x-tar-gz"		},
	{"tar.gz",		MINET_TARGZ,	6,	"application/x-tar-gz"		},
	{"tar",		MINET_TAR,	3,	"application/x-tar"		},
	{"gz",		MINET_GZ,	2,	"application/x-gunzip"		},
	{"arj",		MINET_ARJ,	3,	"application/x-arj-compressed"	},
	{"rar",		MINET_RAR,	3,	"application/x-arj-compressed"	},
	{"rtf",		MINET_RTF,	3,	"application/rtf"		},
	{"pdf",		MINET_PDF,	3,	"application/pdf"		},
	{"swf",		MINET_SWF,	3,	"application/x-shockwave-flash"	},
	{"mpg",		MINET_MPG,	3,	"video/mpeg"			},
	{"mpeg",		MINET_MPEG,	4,	"video/mpeg"			},
	{"asf",		MINET_ASF,	3,	"video/x-ms-asf"		},
	{"avi",		MINET_AVI,	3,	"video/x-msvideo"		},
	{"bmp",		MINET_BMP,	3,	"image/bmp"			},
	{NULL,		-1,			0,	NULL				}
};
#define	DELIM_CHARS	","		/* Separators for lists		*/
struct mine_type* Mine_Type(char *uri, int len)
{
	int i = 0;
	char *ext = memchr(uri, '.', len);
	struct mine_type *mine = NULL;
	int found = 0;
	ext++;

	//printf("uri:%s,len:%d,ext is %s\n",uri,len,ext);

	for(mine = &builtin_mime_types[i]; mine->extension != NULL; i++)
	{
        mine = &builtin_mime_types[i];
        //printf("%s %d\n%s %d\n", mine->extension, strlen(mine->extension), ext, strlen(ext));
		if(strncmp(mine->extension, ext, mine->ext_len) == 0)
		{
			found = 1;
			//printf("found it, ext is %s\n",mine->extension);
			break;
		}

	}

	if(!found)
	{
		mine = &builtin_mime_types[2];
        printf("cannot found ext!\n");
	}	

	return mine;
}