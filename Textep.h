#ifndef _VRE_APP_WIZARDTEMPLATE_
#define	_VRE_APP_WIZARDTEMPLATE_

#define MRE_STR_SIZE_MAX            (100)
#define MRE_FILE_CREATE_FAILED  	(-5)
#define MRE_FILE_OPEN_FAILED        (-3)
#define MRE_FILE_NOT_WRITTEN        (-4)
#define MRE_TEXTBOX_CANCEL          (2)
#define MRE_ERROR                   (-1)
#define MRE_GET_FILE_TIME_FAILED    (-6)
#define MRE_DEF_STRING_SIZE	        (30)
#define MRE_INTIAL_VALUE		(0)
#define MRE_SCREEN_START_X 		(0)
#define MRE_SCREEN_START_Y 		(0)

#include "vmio.h"
#include "vmgraph.h"
#include "vmstdlib.h"
#include <string.h>
#include "vmchset.h"
#include "vmmm.h"
#include "vmpromng.h"
#include "vmvideo.h"
#include "vmcamera.h"
#include "stdint.h"
#include "vmsys.h"
#include "vmeditor.h"
#include <time.h>

void checkFileExist(void);
void save_text(VMINT state, VMWSTR text);

#endif

