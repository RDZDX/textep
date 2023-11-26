#ifndef _VRE_APP_WIZARDTEMPLATE_
#define	_VRE_APP_WIZARDTEMPLATE_

#include "vmio.h"
#include "vmgraph.h"
#include "vmstdlib.h"
#include <string.h>
#include "vmchset.h"
//#include "vmmm.h"
#include "vmpromng.h"
//#include "vmvideo.h"
//#include "vmcamera.h"
#include "stdint.h"
#include "vmsys.h"
//#include "vmeditor.h"
#include <time.h>
//#include "vmsim.h"

VMINT		layer_hdl[1];				////layer handle array.

void handle_sysevt(VMINT message, VMINT param);
void checkFileExist(void);
void save_text(VMINT state, VMWSTR text);
VMINT file(VMWCHAR *FILE_PATH, VMINT wlen);
void create_auto_filename(VMWSTR text);
void create_auto_full_path_name(VMWSTR result, VMWSTR fname);
void create_app_txt_filename(VMWSTR text);
void read_file_to_input(VMWSTR path);

#endif

