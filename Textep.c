#include "Textep.h"

VMFILE f_read;

VMWCHAR file_pathw[100] = {0, 0};
VMWCHAR title_name[100] = {0, 0};

VMINT trigeris = 1;
VMBOOL trigeris1 = VM_FALSE;
VMBOOL trigeris2 = VM_FALSE;
VMBOOL trigeris3 = VM_FALSE;
VMBOOL trigeris4 = VM_FALSE;
VMBOOL trigerisX = VM_FALSE;

void vm_main(void) {

    vm_reg_sysevt_callback(handle_sysevt);
    vm_font_set_font_size(VM_SMALL_FONT); // ar kazka keicia ?
    checkFileExist();

}

void handle_sysevt(VMINT message, VMINT param) {

    switch (message) {
        case VM_MSG_CREATE:
        case VM_MSG_ACTIVE:
            break;

        case VM_MSG_PAINT:
            if (trigeris1 == VM_TRUE) {trigeris2 = VM_TRUE;}
            if (trigeris == 0) {trigeris1 = VM_TRUE;}
            if (trigeris2 == VM_TRUE && trigeris3 == VM_FALSE) { // jei paspaudem "back" sukuria nauja tuscia faila su autopavadinimu
               trigeris3 = VM_TRUE;
               create_auto_filename(title_name);
               create_auto_full_path_name(file_pathw, title_name);
               vm_input_set_editor_title((VMWSTR)title_name);
               vm_input_text3(NULL, 2000, 32, save_text);}
            else if (trigeris4 == VM_TRUE){
               read_file_to_input(file_pathw); //atidarom selektoriumi pasirinkta faila
               trigeris4 = VM_FALSE;
            }
            else{}
            break;

        case VM_MSG_INACTIVE:
            break;

        case VM_MSG_QUIT:
            break;
    }
}

void checkFileExist(void) {

    VMWCHAR appTxtName[100] = {0, 0};

    create_app_txt_filename(appTxtName); // sukuriame defaultinio failo pavadinima is programos pavadinimo
    create_auto_full_path_name(file_pathw, appTxtName);

    f_read = vm_file_open(file_pathw, MODE_READ, FALSE);

    if (f_read < 0) {
        vm_file_close(f_read);
        trigeris = vm_selector_run(0, 0, file);
    } else {
        vm_file_close(f_read);
        wstrcpy(title_name, appTxtName);
        vm_input_set_editor_title((VMWSTR)title_name);
        read_file_to_input(file_pathw);
    }
    return;
}

void save_text(VMINT state, VMWSTR text) {

    VMCHAR new_data[2000] = {0};
    VMWCHAR wAutoFileNamez[101] = {0, 0};
    VMWCHAR wAutoFullPathNamez[101] = {0, 0};
    VMINT lenght;
    VMUINT nwrite;

    lenght = wstrlen(text);

    if (state == VM_INPUT_OK) {
        if (trigerisX == VM_TRUE) {                                        // jei buvo paspausta "back" ir sugeneruotas naujas failas
            create_auto_filename(wAutoFileNamez);                          // ar nesidubliuoja ? 
            create_auto_full_path_name(wAutoFullPathNamez, wAutoFileNamez);
            vm_file_rename((VMWSTR)file_pathw, (VMWSTR)wAutoFullPathNamez); // ar reikia (VMWSTR) ?
        }

        if (lenght > 0) {                                                  // jei buvo atidarytas failas su tekstu
           f_read = vm_file_open(file_pathw, MODE_CREATE_ALWAYS_WRITE, FALSE);
           vm_chset_convert(VM_CHSET_UCS2, VM_CHSET_UTF8, (VMCHAR *)text, new_data, (lenght + 1) * 2); //src, dst
           //vm_ucs2_to_ascii(new_data, 2000, (VMWSTR)text); //+
           vm_file_write(f_read, new_data, strlen(new_data), &nwrite);
           vm_file_close(f_read);
        }
    }
    vm_exit_app();
}

VMINT file(VMWCHAR *FILE_PATH, VMINT wlen) {

    trigeris = 1;
    trigeris1 = VM_FALSE;
    trigeris2 = VM_FALSE;
    VMWCHAR appName[100] = {0, 0};

    vm_get_filename(FILE_PATH, appName);
    wstrcpy(title_name, appName);
    vm_input_set_editor_title((VMWSTR)title_name);
    vm_wstrcpy(file_pathw, FILE_PATH);
    trigeris4 = VM_TRUE;
    return 0;
}

void create_auto_filename(VMWSTR text) {

    struct vm_time_t curr_time;
    VMCHAR fAutoFileName[100];

    vm_get_time(&curr_time);
    sprintf(fAutoFileName, "%02d%02d%02d%02d%02d.txt", curr_time.mon, curr_time.day, curr_time.hour, curr_time.min, curr_time.sec);
    vm_ascii_to_ucs2(text, (strlen(fAutoFileName) + 1) * 2, fAutoFileName);

}

void create_auto_full_path_name(VMWSTR result, VMWSTR fname) {

    VMINT drv;
    VMCHAR fAutoFileName[100] = {0};
    VMWCHAR wAutoFileName[100] = {0, 0};

    if ((drv = vm_get_removable_driver()) < 0) {
       drv = vm_get_system_driver();
    }

    sprintf(fAutoFileName, "%c:\\", drv);
    vm_ascii_to_ucs2(wAutoFileName, (strlen(fAutoFileName) + 1) * 2, fAutoFileName);
    vm_wstrcat(wAutoFileName, fname);
    vm_wstrcpy(result, wAutoFileName);

}

void create_app_txt_filename(VMWSTR text) {

    VMWCHAR fullPath[100] = {0, 0};
    VMWCHAR appName[100] = {0, 0};
    VMCHAR asciiAppName[100] = {0};
    VMCHAR file_name[100] = {0};

    vm_get_exec_filename(fullPath);
    vm_get_filename(fullPath, appName);
    vm_ucs2_to_ascii(asciiAppName, wstrlen(appName) + 1, appName);
    memcpy(file_name, asciiAppName, strlen(asciiAppName) - 3);
    file_name[strlen(asciiAppName) - 3] = '\0';
    strcat(file_name, "txt");
    vm_ascii_to_ucs2(text, (strlen(file_name) + 1) * 2, file_name);

}

void read_file_to_input(VMWSTR path) {

   VMCHAR text_data[2000] = {0};
   void *tmp_data = NULL;
   VMUINT nread;
   VMINT lenght;

   trigerisX = VM_TRUE;

   f_read = vm_file_open(path, MODE_READ, FALSE);
   vm_file_read(f_read, text_data, 2000, &nread);
   text_data[nread] = '\0';
   vm_file_close(f_read);

   lenght = strlen(text_data) * 2 + 1;
   tmp_data = vm_malloc(lenght);
   vm_chset_convert(VM_CHSET_UTF8, VM_CHSET_UCS2, text_data, tmp_data, lenght);
   vm_input_text3((VMWSTR)tmp_data, lenght , 32, save_text);
   vm_free(tmp_data);

}